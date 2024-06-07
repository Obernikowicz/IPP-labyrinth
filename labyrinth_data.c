// Modul implementujacy strukture do przechowywania danych o labiryncie
#include <stdlib.h>
#include "dynamic_array.h"
#include "memory.h"
#include "bitset.h"
#include "labyrinth_data.h"

// Zainicjowanie nowej struktury na podstawie przekazanych argumentow:
// dimensions - tablica wielkosci wymiarow labiryntu
// start_position - tablica przechowujaca pozycje poczatkowa w postaci
//      pozycji w poszczegolnych wymiarach
// end_position - tablica przechowujaca pozycje koncowa w postaci
//      pozycji w poszczegolnych wymiarach
// walls_data - tablica bitowa przechowujaca informacje o scianach labiryntu.
// labyrinth_size - rozmiar labiryntu (iloczyn wielkosci wymiarow).
Labyrinth_data *labyrinth_init(Array *dimensions, Array *start_position,
                               Array *end_position, Bitset *walls_data,
                               size_t labyrinth_size) {
                                   
    Labyrinth_data *labyrinth_data = safe_malloc(sizeof(Labyrinth_data));

    labyrinth_data->dimensions = dimensions;
    labyrinth_data->start_position = start_position;
    labyrinth_data->end_position = end_position;
    labyrinth_data->walls_data = walls_data;
    labyrinth_data->labyrinth_size = labyrinth_size;

    return labyrinth_data;
}

// Funkcja zwracajaca wielkosc wymiaru "index" w labiryncie "labyrinth_data".
size_t get_dimensions_value(Labyrinth_data *labyrinth_data, size_t index) {
    return array_get(labyrinth_data->dimensions, index);
}

// Funkcja zwracajaca pozycje poczatkowa w wymiarze "index"
// w labiryncie "labyrinth_data".
size_t get_start_position_value(Labyrinth_data *labyrinth_data, size_t index) {
    return array_get(labyrinth_data->start_position, index);
}

// Funkcja zwracajaca pozycje koncowa w wymiarze "index"
// w labiryncie "labyrinth_data".
size_t get_end_position_value(Labyrinth_data *labyrinth_data, size_t index) {
    return array_get(labyrinth_data->end_position, index);
}

// Funkcja zwracajaca wartosc bitu "indeks" w zapisie bitowym informajci
// o scianach labiryntu "labirynth_data".
bool get_walls_data_bit_value(Labyrinth_data *labyrinth_data, size_t index) {
    return bitset_test(labyrinth_data->walls_data, index);
}

// Funkcja zwracajaca liczbe wymiarow labiryntu
size_t get_dimensions_size(Labyrinth_data *labyrint_data) {
    return labyrint_data->dimensions->length;
}

// Funkcja zwracajaca wielkosc labiryntu (iloczyn wielkosci wymiarow)
size_t get_labyrinth_size(Labyrinth_data *labyrinth_data) {
    return labyrinth_data->labyrinth_size;
}

// Funkcja zwalniajaca pamiec przydzielona do struktury.
void labyrinth_free(Labyrinth_data *labyrinth_data) {
    array_free(labyrinth_data->dimensions);
    array_free(labyrinth_data->start_position);
    array_free(labyrinth_data->end_position);
    bitset_free(labyrinth_data->walls_data);
    
    free(labyrinth_data);
}