// Program znajdujący najrótszą ścieżkę w wielowymiarowym labiryncie
// od zadanej pozycji początkowej do zadanej pozycji końcowej

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "input.h"
#include "dynamic_array.h"
#include "error.h"
#include "labyrinth_data.h"
#include "bitset.h"
#include "validation.h"
#include "bfs.h"

int main(void) {
    // Pobieranie danych o labiryncie z wejscia i czesciowa weryfikacja
    Labyrinth_data *labyrinth = get_input_data();
    
    // Weryfikacja danych w labiryncie pod katem przypadkow nie sprawdzonych
    // bezposrednio po wczytaniu
    validate_labyrinth_data(labyrinth);

    // Wywolanie funkcji przeszukujacej labirynt i wyswietlenie wyniuku
    bool does_way_exist = true;
    size_t result = bfs(labyrinth, &does_way_exist);

    if (does_way_exist) {
        printf("%lu\n", result);
    }
    else {
        printf("NO WAY\n");
    }

    // Zwolnienie zaalkowoanej pamieci
    labyrinth_free(labyrinth);

    return EXIT_SUCCESS;
}