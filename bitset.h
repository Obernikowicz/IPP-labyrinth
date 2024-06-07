// Modul implementujacy dynamiczna tablice bitowa

#ifndef BITSET_H

#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#define BITSET_H

// Definicja struktury zawierajaca tablice bitowa
typedef struct Bitset {
    uint8_t *data;
    size_t allocated_bits;
} Bitset;

// Funkcja tworzace pusta tablice bitowa o rozmiarze "size" bitow, poczatkowo
// o wartosciach wszystkich bitow rownym 0.
extern Bitset *bitset_init(size_t size);

// Funckja zwracajaca wartosc o indeksie "index" w tablicy bitowej "bitset".
extern bool bitset_test(Bitset *bitset, size_t index);

// Funckja ustawiajaca wartosc o indeksie "index" w tablicy bitowej "bitset".
extern void bitset_set(Bitset *bitset, size_t index, bool value);

// Funkcja sluzaca wstawianiu bitow z liczby w systemie szesnatkowym.
// Ustawia w tablicy bitowej "bitset" wartosci w taki sposob, jakby w zapisie
// szesnatkowym liczby reprezentowanej przez bitset znajdowala sie na indeksie
// "index" wartosc "value".
extern void bitset_insert_hex(Bitset *bitset, size_t index, uint8_t value);

// Funkcja zwracajaca liczbe bitow pamieci zaalakowanej na dane znajdujace sie
// w tablicy bitowej "bitset".
extern size_t bitset_get_allocated_bits_size(Bitset *bitset);

// Funkcja zwalniajaca pamiec zaalokowana dla tablicy bitowej "bitset"
extern void bitset_free(Bitset *bitset);

#endif