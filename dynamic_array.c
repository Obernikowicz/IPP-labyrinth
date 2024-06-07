// Modul implementujacy dynamiczna tablice danych typu "size_t"

#include <stdlib.h>
#include "memory.h"
#include "dynamic_array.h"

#define INITIAL_SIZE 4

// Funkcja tworzace nowa dynamiczna tablice i zwracajaca wskaznik do niej.
Array *array_init() {
    Array *array = (Array*) safe_malloc(sizeof(Array));

    array->length = 0;
    array->allocated_data_size = INITIAL_SIZE;
    array->data = (size_t*) safe_malloc(INITIAL_SIZE * sizeof(size_t));

    return array;
}

// Funkcja dopisujaca element na koniec tablicy.
void array_push_back(Array *array, size_t value) {
    array->data[array->length] = value;
    array->length ++;

    // Jezeli skonczyla sie pamiec zaalokowana na tablice, to nalezy
    // zaalokowac jej wiecej. By uniknac wykonywania tej operacji przy kazdym
    // zwiekszaniu liczby elementow w tablicy, allokujemy zawsze dwukrotnosc
    // poprzedniego rozmiaru.
    if (array->length >= array->allocated_data_size) {
        array->allocated_data_size *= 2;
        array->data =
            (size_t*) safe_realloc(array->data,
                                   array->allocated_data_size * sizeof(size_t));
    }
}

// Funkcja ustawiajaca na indeksie "index" w tablicy "array" wartosc "value".
void array_set(Array *array, size_t index, size_t value) {
    array->data[index] = value;
}

// Funkcja uzwracajaca wartosc w tablicy "array" na indeksie "index"
size_t array_get(Array *array, size_t index) {
    return array->data[index];
}

// Funkcja zwracajaca liczbe elementow w tablicy.
size_t array_length(Array *array) {
    return array->length;
}

// Funkcja zwalniajaca pamiec zaalokowana przez tablice.
void array_free(Array *array) {
    free(array->data);
    free(array);
}
