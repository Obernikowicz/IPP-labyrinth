// Modul zawierajacy funkcje konwerujace dane

#ifndef CONVERTERS_H

#include "bitset.h"
#define CONVERTERS_H

// Struktura do bardziej zwiezlego przechowywania wartosci definiujacych
// polozenie scian w labiryncie.
typedef struct R_values {
    uint32_t a;
    uint32_t b;
    uint32_t m;
    uint32_t r;
    uint32_t s;
} R_values;

// Funkcja konwerujaca zapis inforamcji o scianach w labiryncie w postaci
// litery "R" i piecu liczb zapisanych w "r_values" do zapisu binarnego.
// Funkcja przyjmuje rowniez rozmiar labiryntu, niezbedny do poprawnego
// przekonwertowania danych.
extern Bitset *convert_r_values_to_walls_data_bitset(R_values r_values,
                                                     size_t labyrinth_size);

#endif