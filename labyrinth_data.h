// Modul implementujacy strukture do przechowywania danych o labiryncie
#ifndef LABYRINTH_DATA_H

#include "dynamic_array.h"
#include "bitset.h"

#define LABYRINTH_DATA_H

// Definicja strukruty sluzacej przechowywaniu danych o labiryncie
typedef struct Labyrinth_data {
    Array *dimensions;
    Array *start_position;
    Array *end_position;
    Bitset *walls_data;
    size_t labyrinth_size;
} Labyrinth_data;

// Zainicjowanie nowej struktury na podstawie przekazanych argumentow:
// dimensions - tablica wielkosci wymiarow labiryntu
// start_position - tablica przechowujaca pozycje poczatkowa w postaci
//      pozycji w poszczegolnych wymiarach
// end_position - tablica przechowujaca pozycje koncowa w postaci
//      pozycji w poszczegolnych wymiarach
// walls_data - tablica bitowa przechowujaca informacje o scianach labiryntu.
// labyrinth_size - rozmiar labiryntu (iloczyn wielkosci wymiarow).
extern Labyrinth_data *labyrinth_init(Array *dimensions, Array *start_position,
                                      Array *end_position, Bitset *walls_data,
                                      size_t labyrinth_size);

// Funkcja zwracajaca wielkosc wymiaru "index" w labiryncie "labyrinth_data".
extern size_t get_dimensions_value(Labyrinth_data *labyrinth_data,
                                   size_t index);

// Funkcja zwracajaca pozycje poczatkowa w wymiarze "index"
// w labiryncie "labyrinth_data".
extern size_t get_start_position_value(Labyrinth_data *labyrinth_data,
                                       size_t index);

// Funkcja zwracajaca pozycje koncowa w wymiarze "index"
// w labiryncie "labyrinth_data".
extern size_t get_end_position_value(Labyrinth_data *labyrinth_data,
                                     size_t index);

// Funkcja zwracajaca wartosc bitu "indeks" w zapisie bitowym informajci
// o scianach labiryntu "labirynth_data".
extern bool get_walls_data_bit_value(Labyrinth_data *labyrinth_data,
                                     size_t index);

// Funkcja zwracajaca liczbe wymiarow labiryntu
extern size_t get_dimensions_size(Labyrinth_data *labyrint_data);

// Funkcja zwracajaca wielkosc labiryntu (iloczyn wielkosci wymiarow)
extern size_t get_labyrinth_size(Labyrinth_data *labyrinth_data);

// Funkcja zwalniajaca pamiec przydzielona do struktury.
extern void labyrinth_free(Labyrinth_data *labyrinth_data);

#endif