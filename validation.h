// Modul zawierajacy funkcje do weryfikacji czesci danych o labiryncie
#ifndef VALIDATION_H

#include <stdbool.h>
#include "dynamic_array.h"
#include "labyrinth_data.h"

#define VALIDATION_H

// Funkcja liczaca wielkosc labiryntu i sprawdzajaca, czy wielkosc ta 
// nie wykracza poza zakres arytmetyki komputera.
// array - tablica wielkosci wymiarow labirynty
// overflow - wskaznik na zmienna, w ktorej zostanie zapisana informacja,
//      czy nastapilo przepelnienie arytmetyki komputera.
extern size_t calculate_validate_labyrinth_size(Array *array, bool *overflow);

// Funkcja weryfikujaca poprawnosc danych wejsciowych opisujacych labirynt
// pod katem aspektow niesprawdzanych niezwlocznie po wczytaniu.
// labyrinth_data - wskaznik na strukture typu Labyrinth_data, przechowujaca
//      dane o labiryncie.
extern void validate_labyrinth_data(Labyrinth_data *labyrinth_data);

#endif