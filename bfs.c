// Modul zawierajacy funkcje do przeszukiwania labiryntu

#include <stdlib.h>
#include <stdbool.h>
#include "labyrinth_data.h"
#include "bitset.h"
#include "queue.h"
#include "dynamic_array.h"
#include "bfs.h"

// Funkcja pomocnicza liczaca numer bitu w bitowym zapisie danych o scianach
// w labiryncie, odpowiadajacy podanej pozycji. Liczy to zgodnie ze wzorem:
// (z_1 - 1) + (z_2 - 1) * n_1 + (z_3 - 1) * n_1 * n_2 + ... +
// + (z_k - 1) * n_1 * n_2 * ... * n_(k-1)
// position - tablica zawierajaca pozycje punktu w kazdym z wymiarow
// dimensions - tablica zawierajaca rozmiary poszczegolnych wymiarow
static size_t get_walls_data_bit_number(Array *position, Array *dimensions) {
    size_t bit_index = array_get(position, 0) - 1;
    size_t multiplier = array_get(dimensions, 0);

    for (size_t i = 1; i < array_length(position); i ++) {
        bit_index += (array_get(position, i) - 1) * multiplier;

        multiplier *= array_get(dimensions, i);
    }

    return bit_index;
}

// Funkcja pomocnicza sluzaca do zwalniania pamieci zaalolowanej przez
// funckje przeszukujaca labirynt, czyli tablice bitowe "visited"
// oraz "states", jak rowniez kolejke "queue".
static void free_memory(Bitset *visited, Bitset *states, Queue *queue) {
    bitset_free(visited);
    bitset_free(states);
    queue_free(queue);
}

// Funkcja przeszukujaca labirynt wszerz (korzystajac z algorytmu bazujacego
// na Breadth-first search). Algorytm zostal zmodyfikowany przez zastosowanie
// optymalizacji pamieciowej, pozwalajacej na oznaczanie stanow poszczegolnych
// wierzcholkow (pozycji w labiryncie) za pomoca jedynie dwoch bitow
size_t bfs(Labyrinth_data *labyrinth, bool *does_way_exist) {
    size_t dimensions = get_dimensions_size(labyrinth);
    size_t labyrinth_size = get_labyrinth_size(labyrinth);

    // Alokowanie pamieci dla struktur wykorzystanych w algorytmie

    // Stworzenie dwoch tablic bitowych do przechowywania informajci o stanach
    // pozycji w labiryncie. Tablica "visited" przechowuje informacje o tym, 
    // czy dana pozycja byla juz odwiedzona. Tablica states przechowuje
    // wartosci, pozwalajace wychwycic zmiane odleglosci od pozycji poczatkowej.
    // Algorytm przeszukuje labirynt wszerz, czyli warstwami. Stad, podczas
    // przeszukiwania, wierzcholkom sasiednim, do odwiedzenia podczas
    // przeszukiwania nastepnej warstwy, przypisuje sie stan odwrotny od tego w
    // obecnym wierzholku. Stad, jesli podczas przeszukiwania wierzholkow trafi
    // sie na stan inny od poprzedniego, mozemy uznac, ze zwiekszyla sie
    // odleglosc od pozycji poczatkowej
    Bitset *visited = bitset_init(labyrinth_size);
    Bitset *states = bitset_init(labyrinth_size);

    // Stworzenie kolejki sluzacej przechowywaniu w odpowiedniej kolejnosci
    // nastepnych pozycji do odwiedzenia podczas przeszukiwania labiryntu
    Queue *queue = queue_init();

    // Znalezienie bitowego zapisu pozycji poczatkowej i koncowej
    size_t start_position_bit_number = 
        get_walls_data_bit_number(labyrinth->start_position,
                                  labyrinth->dimensions);

    size_t end_position_bit_number = 
        get_walls_data_bit_number(labyrinth->end_position,
                                  labyrinth->dimensions);

    // Dodanie do kolejki poczatkowej pozycji i ustawienie jej, jako odwiedzona.
    // Poczatkowy stan, pozwalajacy okreslic odleglosc jest rowny 0, co jest
    // w uzywanej implementacji wartoscia domyslna, wiec nie wymaga ustawienia.
    queue_insert(queue, start_position_bit_number);
    bitset_set(visited, start_position_bit_number, 1);

    // Stworzenie zmiennej do przechowywania stanu z ostatniej pozycji
    bool last_state = 0;
    
    // Stworzenie zmiennej do przechowywania odleglosci od pozycji poczatkowej
    size_t result = 0;
    while (!queue_is_empty(queue)) {
        size_t position = queue_front(queue);
        // Usuniecie z kolejki obecnie obslugiwanego wierzcholka
        queue_pop(queue);

        // Oznaczenie wierzcholka jako odwiedzonego
        bitset_set(visited, position, 1);

        // Sprawdzenie, czy zmienila sie odleglosc od pozycji poczatkowej
        bool state = bitset_test(states, position);
        if (state != last_state) {
            result ++;
            last_state = state;
        }
        
        // Sprawdzenie, czy znaleziona zostala sciezka poprzez dotarcie do
        // zadanej pozycji koncowej
        if (position == end_position_bit_number) {
            *does_way_exist = true;

            free_memory(visited, states, queue);
            return result;
        }

        // Liczenie pozycji sasiednich. Majac liczbe reprezentujaca zapis bitowy
        // pozycji pozstaci: (z_1 - 1) + (z_2 - 1) * n_1 + (z_3 - 1) * n_1 * n_2
        // + ... + (z_k - 1) * n_1 * n_2 * ... * n_(k-1)
        // i dzielac ja calkowitoliczbowo przez n_1 * n_2 * ... * n_(k-1),
        // pozostanie jedynie wartosc (z_k - 1), pozwalajac okreslic pozycje
        // w ostatnim z wymiarow. Nastepnie, od poczotkowej wartosci
        // reprezentujacej pozycje, odejmujemy:
        // (z_k - 1) * n_1 * n_2 * ... * n_(k-1)
        // otrzymujac: (z_1 - 1) + (z_2 - 1) * n_1 + (z_3 - 1) * n_1 * n_2
        // + ... + (z_(k-1) - 1) * n_1 * n_2 * ... * n_(k-2), co pozwala
        // obliczyc pozycje w kolejnym wymiarze. Iterujac sie po wymiarach,
        // stosujac powyzsza zasade, wyznaczane sa pozycje w kazdym z wymiarow.

        // Zmienne pomocnicze przechowyjace wartosc liczby reprezentujacej zapis
        // bitowy obecnej pozycji po odejmowaniach oraz dzielnik, ktorego nalezy
        // uzyc przy liczeniu pozycji kolejnych wymiarow
        size_t current_divider = labyrinth_size;
        size_t current_position_value = position;

        // Poniewaz typ size_t przyjmuje wartosci jedynie dodatnie, to
        // warunek "iterator >= 0" bylby zawsze prawdziwy, bo dochodziloby
        // do przepelnienia arytmetyki komputera. Stad, petla przechodzi po
        // wartosciach o 1 wiekszych, a nastepnie korzysta z indeksu "i", ktory
        // jest iteratorem pomniejszonym o 1.
        for (size_t iterator = dimensions; iterator >= 1; iterator --) {
            size_t i = iterator - 1;

            size_t dimension_size = get_dimensions_value(labyrinth, i);
            current_divider /= dimension_size;

            size_t position_in_dimension_i =
                (current_position_value / current_divider) + 1;

            // Jezeli pozycja w danym wymiarze jest na skraju (i = 1), to nie
            // mozemy odwiedzic sasiedniej, odpowiadajacej tej mniejszej o 1.
            if (position_in_dimension_i != 1) {
                size_t next_position = position - current_divider;
                
                // Jezeli hipotetyczna nastepna pozycja byla juz odwiedzona
                // lub znajduje sie w scianie, to nie kolejkujemy jej
                // do odwiedzenia
                if (!bitset_test(visited, next_position) &&
                    !get_walls_data_bit_value(labyrinth, next_position)) {

                    bitset_set(visited, next_position, 1);
                    // Dodawany do kolejki wierzcholek jest w kolejnej warstwie,
                    // wiec przypisywany mu jest odwrotny stan.
                    bitset_set(states, next_position, !state);
                    queue_insert(queue, next_position);
                }
            }

            // Jezeli pozycja w danym wymiarze jest na skraju (i = n_i), to nie
            // mozemy odwiedzic sasiedniej, odpowiadajacej tej wiekszej o 1.
            if (position_in_dimension_i != dimension_size) {
                size_t next_position = position + current_divider;
                
                // Jezeli hipotetyczna nastepna pozycja byla juz odwiedzona
                // lub znajduje sie w scianie, to nie kolejkujemy jej
                // do odwiedzenia
                if (!bitset_test(visited, next_position) &&
                    !get_walls_data_bit_value(labyrinth, next_position)) {

                    bitset_set(visited, next_position, 1);
                    // Dodawany do kolejki wierzcholek jest w kolejnej warstwie,
                    // wiec przypisywany mu jest odwrotny stan.
                    bitset_set(states, next_position, !state);
                    queue_insert(queue, next_position);
                }
            }

            current_position_value -=
                (position_in_dimension_i - 1) * current_divider;
        }
    }

    // Zakonczenie petli while oznacza, ze przeszukane zostaly wszystkie
    // pozycje, do ktorych mozna dojsc z pozycji poczatkowej i nie zostala
    // odwiedzona pozycja koncowa, czyli nie istnieje droga laczaca te dwie
    // pozycje
    *does_way_exist = false;

    free_memory(visited, states, queue);
    return result;
}