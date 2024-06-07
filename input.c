// Modul odpowowiadajacy za wczytywanie danych wejsciowych.
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <inttypes.h>
#include "dynamic_array.h"
#include "labyrinth_data.h"
#include "error.h"
#include "validation.h"
#include "converters.h"
#include "input.h"

#define NEW_LINE_CHARACTER '\n'

// Definicje najmniejszych wartosci, dla ktorych potrzeba minimum danej liczby
// bitow do ich zapisania.
#define ONE_BIT_VALUE 1
#define TWO_BIT_VALUE 2
#define THREE_BIT_VALUE 4
#define FOUR_BIT_VALUE 8

// Funkcja pomocnica zwracajaca pierwszy, niebedacy znakiem białym
// (z wylaczeniem znaku konca lini i EOF), znak.
static char get_first_non_whitespace_character() {
    int character;

    while (isspace(character = getchar()) 
           && character != NEW_LINE_CHARACTER && character != EOF) {

        ;
    }

    return character;
}

// Funkcja pomocnicza wczytujaca pojedyncza liczbe z wejscia, z zalozeniem,
// ze moze sie przed nia znajdowac dowolna liczba znakow bialych.
// is_error - wskaznik do zmiennej, w ktorej zostanie zapisana informacja
//      o ewentualnym bledzie przy odczycie
// is_EOF - wskaznik do zmiennej, w ktorej zostanie zapisana informacja, czy
//      napotkano znak EOF
// has_new_line - wskaznik do zmiennej, w ktorej zostanie zapisana informacja
//      czy napotkano znak nowej linii
// is_empty - wskaznik do zmiennej, w ktorej zostanie zapisana informacja, czy
//      wczytane dane majace zawierac liczbe byly puste
// max_size - maksymalna akceptowalna wielkosc liczby
static size_t get_number(bool *is_error, bool *has_EOF, bool *has_new_line,
                         bool *is_empty, size_t max_size) {

    int character = get_first_non_whitespace_character();

    // Jezeli napotkano na znak nowej linii lub EOF na samym poczatku, to
    // zadna liczba nie zostala wczytana.
    if (character == EOF || character == NEW_LINE_CHARACTER) {
        *has_EOF = character == EOF;
        *has_new_line = character == NEW_LINE_CHARACTER;
        *is_error = false;
        *is_empty = true;

        return 0;
    }

    // Jezeli nie jest spelniony ponizszy warunek, to wczytany znak nie jest
    // cyfra, czyli wystapil blad przy odczycie.
    if ((char) character < '0' || (char) character > '9') {
        *has_EOF = *has_new_line = false;
        *is_error = *is_empty = true;

        return 0;
    }

    // Konwersja znaku na reprezentowana przez niego cyfre.
    size_t result = character - '0';

    while (true) {
        character = getchar();

        // Jezeli trafiono na znak EOF lub konca linii, to zakonczono wczytywac
        // liczbe.
        if(character == EOF || character == NEW_LINE_CHARACTER) {
            *has_EOF = character == EOF;
            *has_new_line = character == NEW_LINE_CHARACTER;
            *is_error = *is_empty = false;

            return result;
        }
        
        // Kolejne cyfry wczytywane sa az do znaku bialego, poniewaz natrafienie
        // na znak bialy oznacza zakonczenie wczytywania liczby.
        if (!isspace(character)) {
            // Jezeli nie jest spelniony ponizszy warunek, to wczytany znak nie jest
            // cyfra, czyli wystapil blad przy odczycie.
            if ((char) character < '0' || (char) character > '9') {
                *has_EOF = *has_new_line = *is_empty = false;
                *is_error = true;

                return 0;
            }

            // Pierwsza czesc sprawdzania, czy wczytywana liczba nie przekroczy
            // maksymalnej akceptowalnej wartosci. 
            if(result >= (max_size / 10) + 1) {
                *has_EOF = *has_new_line = *is_empty = false;
                *is_error = true;

                return 0;
            }

            result *= 10;

            // Konwersja znaku na odpowiadajaca mu cyfre.
            int character_digit = character - '0';

            // Druga czesc sprawdzenia, czy wczytywana liczba nie przekroczy
            // maksymalnej akceptowalnej wartosci. Taka postac warunku zapobiega
            // przepelnienu arytmetyki.
            if (result > max_size - character_digit) {
                *has_EOF = *has_new_line = *is_empty = false;
                *is_error = true;

                return 0;
            }

            result += character_digit;
        }
        else {
            // Trafienie na znak bialy oznacza zakonczenie wczytywania liczby.
            *has_EOF = *has_new_line = *is_error = *is_empty = false;

            return result;
        }
    }
}

// Funkcja pomocnicza wywolywana, gdy na koncu czwartej linii danych wejsciowych
// znajduje sie znak nowej linii. Sprawdza, czy piata linia jest pusta.
static bool is_fifth_line_empty() {
    return getchar() == EOF;
}

// Funkcja pomocnicza konwerdujaca znak odpowiadajacy wartosci szesnastkowej
// na jej wartosc liczbowa.
// character - znak odpowiadajacy wartosci szesnastkowej.
// error - wskaznik do zmiennej, w której zostanie zapisana informacja
// o ewentualnym bledzie.
static size_t get_hex_character_value(char character, bool *error) {
    size_t result;

    *error = false;
    if ((char) character >= '0' && (char) character <= '9') {
        result = (char) character - '0';

        return result;
    }

    if((char) character >= 'A' && (char) character <= 'F') {
        result = 10;
        result += character - 'A';

        return result;
    }

    if((char) character >= 'a' && (char) character <= 'f') {
        result = 10;
        result += character - 'a';

        return result;
    }

    // Jezeli zaden z powyzszych warunkow nie jest spelniony, to przekazany
    // do funkcji znak nie reprezentuje wartosci w systemie szesnastkowym.
    *error = true;
    return -1;
}

// Funkcja pomocnicza zwracajaca tablice wartosci liczbowych kolejnych znakow
// odpowiadajacym zapisowi szesnastkowemu na wejsciu.
// error - wskaznik do wartosci, w ktorej zostanie zapisany numer linii,
//      w ktorym wystapil blad. Jesli blad nie wystapi, zostanie wpisana
//      wartosc 0.
static Array *get_hex_data_array(int *error) {
    int character = getchar();
    
    bool is_error;
    size_t value = get_hex_character_value(character, &is_error);
    if (is_error) {
        // Znak na wejsciu nie odpowiada zapisowi szesnastkowemu, wiec dane
        // w czwartej linii wejscia sa niepoprawne.
        *error = 4;

        return NULL;
    }

    // Zainicjalizownaie tablicy przechowujacej wynik i wpisanie do niej
    // pierwszej wartosci.
    Array *result = array_init();
    array_push_back(result, value);

    *error = 0;

    while (true) {
        character = getchar();
        
        // Jezeli napotykany jest znak EOF, to dane zostaly bezblednie wczytane.
        if (character == EOF) {
            return result;
        }
        // Jezeli napotykany jest znak konca linii, to nalezy sprawdzic, 
        // czy piata linia jest pusta.
        if (character == NEW_LINE_CHARACTER) {
            if (is_fifth_line_empty()) {
                return result;
            }

            *error = 5;
            array_free(result);
            return NULL;
        }

        // Wczytywana liczba w zapisie szesnastkowym nie moze byc oddzielona
        // znakami bialymi, wiec nalezy sprawdzic, czy po znakach bialych linia
        // sie konczy.
        if (isspace(character)) {
            character = get_first_non_whitespace_character();

            if (character == EOF) {
                return result;
            }

            if (character == NEW_LINE_CHARACTER) {
                if(is_fifth_line_empty()) {
                    return result;
                }

                *error = 5;
            }

            *error = 4;

            array_free(result);
            return NULL;
        }

        value = get_hex_character_value(character, &is_error);
        if (is_error) {
            // Znak na wejsciu nie odpowiada zapisowi szesnastkowemu, wiec dane
            // w czwartej linii wejscia sa niepoprawne.
            *error = 4;

            array_free(result);
            return NULL;
        }

        // Dodanie do tablicy nastepnej wartosci odpowiadajacej zapisowi
        // szesnastkowemu.
        array_push_back(result, value);
    }
}

// Funkcja sprawdzajaca minimalna liczbe bitow wartosci "value"
// z zakresu 0-15, konieczna do zapisania jej w systemie binarnym.
static size_t get_hex_value_bit_size(size_t value) {
    if (value >= FOUR_BIT_VALUE) {
        return 4;
    }
    if (value >= THREE_BIT_VALUE) {
        return 3;
    }
    if (value >= TWO_BIT_VALUE) {
        return 2;
    }
    if (value >= ONE_BIT_VALUE) {
        return 1;
    }
    return 0;
}

// Funkcja pomocnicza wczytujaca dane o scianach w labiryncie w postaci
// liczby w systemie szesnatkowym, po wczytyaniu ciagu znakow "0x".
static Bitset *get_hex_walls_data(size_t labyrinth_size, int *error) {
    *error = 0;

    // Wczytanie wartosci liczbowych odpowiadajacych liczbom w systemie
    // szesnatkowym
    Array *hex_data = get_hex_data_array(error);
    if(*error) {
        return NULL;
    }
    
    // Stworzenie tablicy binarnej do przechowywania wartosci.
    Bitset *walls_data = bitset_init(labyrinth_size);
    // Licznik bitow, ktore mozna jeszcze zaalokowacm by nie przekroczyc
    // rozmiaru labiryntu.
    size_t bit_size_left = labyrinth_size;
    // Indeks w tablicy bitowej, w ktorym nalezy zapisac wartosc
    size_t bitset_index = 0;
    size_t array_size = array_length(hex_data);
    // Licznik bitow umiesczonych w tablicy bitowej
    size_t inserted_bits = 0;

    // Znalezienie najstarszej, niezerowej wartosci w zapisie szesnastkowym,
    // by uniknac dalszego procesowania zer wiadacych.
    size_t first_non_zero_value = 0;
    while (first_non_zero_value < array_size 
           && array_get(hex_data, first_non_zero_value) == 0) {

        first_non_zero_value ++;
    }

    // Konwersja na zapis binarny.

    // Poniewaz typ size_t przyjmuje wartosci jedynie dodatnie, to
    // warunek "iterator >= 0" bylby zawsze prawdziwy, bo dochodziloby
    // do przepelnienia arytmetyki komputera. Stad, petla przechodzi po
    // wartosciach o 1 wiekszych, a nastepnie korzysta z indeksu "i", ktory
    // jest iteratorem pomniejszonym o 1.
    for (size_t iterator = array_size; iterator >= first_non_zero_value + 1; iterator --) {
        size_t i = iterator - 1;

        size_t hex_value = array_get(hex_data, i);
        size_t bit_size = get_hex_value_bit_size(hex_value);
        
        // Jezeli minimalna liczba do zapisania wartosci w systemie bitowym
        // jest wieksza, od ilosci dostepnych bitow, to dane sa niepoprawne.
        if (bit_size > bit_size_left) {
            *error = 4;

            array_free(hex_data);
            bitset_free(walls_data);
            return NULL;
        }

        // Ostatnia wartosc, w przeciwienstwie do poprzednich, nie musi byc
        // reprezentowana przez 4 bity, wystarczy jedynie minimalna liczba
        // bitow potrzebnych do jej zapisnaia.
        if (bit_size_left < 4) {
            inserted_bits += bit_size_left;
            bit_size_left = 0;
        }
        else {
            inserted_bits += 4;
            bit_size_left -= 4;
        }

        // Jezeli wpisanie liczby przekroczy rozmiar tablicy bitowej
        // odpowiadajacy rozmiarowi labiryntu, dane sa niepoprawne.
        if (inserted_bits > labyrinth_size) {
            *error = 4;

            array_free(hex_data);
            bitset_free(walls_data);
            return NULL;
        }
        
        bitset_insert_hex(walls_data, bitset_index, (uint8_t) hex_value);
        bitset_index += 4;
    }

    array_free(hex_data);
    return walls_data;
}

// Funkcja pomocnicza wczytujaca dane o scianach w labiryncie w postaci
// pieciu liczb, po wczytyaniu litery "R".
// labyrinth_size - rozmiar labiryntu
// is_error - wskaznik na zmienna, w ktorej zostanie zapisany numer linii
//      wejscia, w ktorej ewentualnei wystapil blad. Jesli blad nie wystapil,
//      ustawiana jest na 0.
static Bitset *get_r_walls_data(size_t labyrinth_size, int *is_error) {
    bool error, has_EOF, has_new_line, is_empty;
    error = has_EOF = has_new_line = is_empty = false;

    // Wczytanie do struktury typu R_values pieciu liczb okreslajacych
    // dane o scianach w labiryncie i sprawdzenie ich zgodnosci z zalozeniami
    // ze specyfikajci dancyh wejsciowych.
    R_values r_values;
    r_values.a = (uint32_t) get_number(&error, &has_EOF, &has_new_line,
                                       &is_empty, UINT32_MAX);

    if (error || has_EOF || has_new_line) {
        *is_error = 4;

        return NULL;
    }

    r_values.b = (uint32_t) get_number(&error, &has_EOF, &has_new_line,
                                       &is_empty, UINT32_MAX);

    if (error || has_EOF || has_new_line) {
        *is_error = 4;

        return NULL;
    }

    r_values.m = (uint32_t) get_number(&error, &has_EOF, &has_new_line,
                                       &is_empty, UINT32_MAX);

    if (error || has_EOF || has_new_line || r_values.m < 1) {
        *is_error = 4;

        return NULL;
    }

    r_values.r = (uint32_t) get_number(&error, &has_EOF, &has_new_line,
                                       &is_empty, UINT32_MAX);

    if (error || has_EOF || has_new_line) {
        *is_error = 4;

        return NULL;
    }

    r_values.s = (uint32_t) get_number(&error, &has_EOF, &has_new_line,
                                       &is_empty, UINT32_MAX);

    if (error) {
        *is_error = 4;

        return NULL;
    }

    // Jezeli po wczytaniu 5 liczb napotykamy na znak nowej linii, to, zgodnie
    // ze specyfikacja danych wejsciowych, piata linia musi byc pusta.
    if (has_new_line) {
        if (!is_fifth_line_empty()) {
            *is_error = 5;

            return NULL;
        }
    }

    // Jezeli nie napotkamy po ostatniej wczytanej liczbie na znak EOF,
    // to dalej, przed znakiem konca lini lub EOF, moga wystepowac jedynie
    // znaki biale
    if (!has_EOF) {
        int character = get_first_non_whitespace_character();

        // Jezeli po wczytaniu 5 liczb napotykamy na znak nowej linii, to, zgodnie
        // ze specyfikacja danych wejsciowych, piata linia musi byc pusta.
        if (character == NEW_LINE_CHARACTER) {
            if (!is_fifth_line_empty()) {
                *is_error = 5;

                return NULL;
            }
        }
        // Jezeli natrafimy na znak inny niz OEF i znak nowej linii, to dane
        // w czwartej linii wejscia sa niepoprawne.
        else if (character != EOF) {
            *is_error = 4;

            return NULL;
        }
    }

    // Konwerowanie reprezentacji skladajaca sie z pieciu liczb do reprezentacji
    // binarnej, w celu ulatwienia dalszego procesowania danych.
    Bitset *walls_data =
        convert_r_values_to_walls_data_bitset(r_values, labyrinth_size);
    
    return walls_data;
}

// Funkcja pomocnicza wczytujaca dane z jednej linii 1-3.
// is_error - wskaznik do zmiennej, w ktorej zostanie zapisana informacja
//      o ewentualnym bledzie.
Array *get_numbers_line(bool *is_error) {
    Array *data = array_init();

    while (true) {
        bool error, has_EOF, has_new_line, is_empty;
        error = has_EOF = has_new_line = is_empty = false;

        // Wczytanie pojedynczej liczby.
        size_t number = get_number(&error, &has_EOF, &has_new_line,
                                   &is_empty, SIZE_MAX);

        // Jesli wczytanie pojedynczej liczby zakonczylo sie bledem,
        // to dane wejsciowe sa niepoprawne.
        if (error) {
            *is_error = true;

            return data;
        }

        // Jesli napotkano znak "EOF", dane wejsciowe sa niekompletne.
        if (has_EOF) {
            *is_error = true;

            return data;
        }

        // Jesli linia jest pusta, to dane wejsciowe sa niepoprawne.
        if (is_empty && array_length(data) == 0 && has_new_line) {
            *is_error = true;

            return data;
        }

        // Przypadek, gdy linia konczy sie bialymi znakami zakonczonymi
        // znakiem nowej linii
        if (is_empty && has_new_line) {
            *is_error = false;
            
            return data;
        }

        // Jesli wczytana liczba to 0, to dane wejsciowe sa niezgodne
        // ze specyfikacja
        if (number == 0) {
            *is_error = true;

            return data;
        }

        array_push_back(data, number);

        // Jesli natrafiono na znak nowej linii, to wczytywanie linii
        // zostalo zakonczone.
        if (has_new_line) {
            *is_error = false;
            
            return data;
        }
    }
}

// Funkcja pomocnicza wczytujaca czwarta linie danych wejsciowych.
// labyrinth_size - rozmiar labiryntu
// is_error - wskaznik na zmienna, w ktorej zostanie zapisany numer linii,
//      w ktorej ewentualnie wystapil blad. Jesli blad niewystapi, zmienna
//      ustawiana jest na 0.
static Bitset *get_walls_data(size_t labyrinth_size, int *is_error) {
    int character = get_first_non_whitespace_character();

    // Jezeli linia jest pusta lub zawiera jedynie znaki biale, to dane
    // wejsciowe sa niepoprawne.
    if (character == EOF || character == NEW_LINE_CHARACTER) {
        *is_error = 4;

        return NULL;
    }

    // Jezeli dane w czwartej linii zaczynaja sie od znakow "0x", to wczytujemy
    // dane w postaci szesnastkowej.
    if ((char) character == '0') {
        character = getchar();
        // Jezeli miedzy znakami "0" a "x" wystepuje jakikolwiek inny znak,
        // to dane wejsciowe sa niepoprawne.
        if ((char) character != 'x') {
            *is_error = 4;
            return NULL;
        }

        return get_hex_walls_data(labyrinth_size, is_error);
    }

    // Jezeli dane w czwartej linii zaczynaja sie od znaku "R", to wczytujemy
    // dane w postaci pieciu liczb reprezentujacy dane o scianach labiryntu.
    if ((char) character == 'R') {
        return get_r_walls_data(labyrinth_size, is_error);
    }

    *is_error = 4;
    return NULL;
}

// Funkcja wczytujaca dane wejsciowe i zwracajaca je w postaci wskaznika
// do struktury typu Labyrinth_data, umozliwiajacej zwiezle przechowywanie
// informacji o labiryncie. Funkcja jednoczesnie czesciowo weryfikuje
// wprowadzone dane.
Labyrinth_data *get_input_data() {
    bool is_error;

    // Wczytujemy pierwsze 3 linie wejscia, sprawdzamy czy zawieraja po tyle
    // samo liczb oraz weryfikujemy, czy rozmiar labiryntu miesci sie
    // w zakresie arytmeryki komputera.
    Array *dimensions = get_numbers_line(&is_error);
    if (is_error) {
        array_free(dimensions);

        exit_with_error(1);
    }

    size_t labyrinth_size =
        calculate_validate_labyrinth_size(dimensions, &is_error);

    if (is_error) {
        array_free(dimensions);

        exit_with_error(0);
    }

    Array *start_position = get_numbers_line(&is_error);
    if (is_error || array_length(start_position) != array_length(dimensions)) {
        array_free(dimensions);
        array_free(start_position);

        exit_with_error(2);
    }

    Array *end_position = get_numbers_line(&is_error);
    if (is_error || array_length(end_position) != array_length(dimensions)) {
        array_free(dimensions);
        array_free(start_position);
        array_free(end_position);

        exit_with_error(3);
    }

    // Wczytujemy czwarta linie wejscia.
    int error_code = 0;
    Bitset *walls_data = get_walls_data(labyrinth_size, &error_code);
    if(error_code != 0) {
        array_free(dimensions);
        array_free(start_position);
        array_free(end_position);

        exit_with_error(error_code);
    }

    // Zwracamy dane o labiryncie w postaci struktury, co sluzy wiekszej
    // zwiezlosci ich zapisu.
    return labyrinth_init(dimensions, start_position, end_position,
                          walls_data, labyrinth_size);
}