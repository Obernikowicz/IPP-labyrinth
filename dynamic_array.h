// Modul implementujacy dynamiczna tablice danych typu "size_t"

#ifndef DYNAMIC_ARRAY_H

#include <stdlib.h>

#define DYNAMIC_ARRAY_H

// Definicja struktury zawierajacej dynamiczna tablice.
typedef struct Array {
    size_t *data;
    size_t length;
    size_t allocated_data_size;
} Array;

// Funkcja tworzace nowa dynamiczna tablice i zwracajaca wskaznik do niej.
extern Array *array_init();

// Funkcja dopisujaca element na koniec tablicy.
extern void array_push_back(Array *array, size_t value);

// Funkcja ustawiajaca na indeksie "index" w tablicy "array" wartosc "value".
extern void array_set(Array *array, size_t index, size_t value);

// Funkcja uzwracajaca wartosc w tablicy "array" na indeksie "index"
extern size_t array_get(Array *array, size_t index);

// Funkcja zwracajaca liczbe elementow w tablicy.
extern size_t array_length(Array *array);

// Funkcja zwalniajaca pamiec zaalokowana przez tablice.
extern void array_free(Array* array);

#endif