// Modul implementujacy dynamiczna tablice bitowa

#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include "memory.h"
#include "bitset.h"

// Funkcja tworzace pusta tablice bitowa o rozmiarze "size" bitow, poczatkowo
// o wartosciach wszystkich bitow rownym 0.
Bitset *bitset_init(size_t size) {
    Bitset *bitset = (Bitset*) safe_malloc(sizeof(Bitset));
    size_t bytes = (size / 8) + (size % 8 > 0);

    bitset->allocated_bits = bytes * 8;
    bitset->data = (uint8_t*) safe_calloc(bytes, sizeof(uint8_t));

    return bitset;
}

// Funckja zwracajaca wartosc o indeksie "index" w tablicy bitowej "bitset".
bool bitset_test(Bitset *bitset, size_t index) {
    size_t byte = index / 8;
    size_t bit = index - (byte * 8);

    return (bitset->data[byte] & (1 << bit)) > 0;
}

// Funckja ustawiajaca wartosc o indeksie "index" w tablicy bitowej "bitset".
void bitset_set(Bitset *bitset, size_t index, bool value) {
    size_t byte = index / 8;
    size_t bit = index - (byte * 8);
    bool current_value = bitset_test(bitset, index);

    if (value == 1 && current_value == 0) {
        bitset->data[byte] |= (1 << bit);

        return;
    }
    if(value == 0 && current_value == 1) {
        bitset->data[byte] -= (1 << bit);

        return;
    }
}

// Funkcja sluzaca wstawianiu bitow z liczby w systemie szesnatkowym.
// Ustawia w tablicy bitowej "bitset" wartosci w taki sposob, jakby w zapisie
// szesnatkowym liczby reprezentowanej przez bitset znajdowala sie na indeksie
// "index" wartosc "value".
void bitset_insert_hex(Bitset *bitset, size_t index, uint8_t value) {
    size_t byte = index / 8;

    if (8 * byte != index) {
        value <<= 4;
    }

    bitset->data[byte] |= value;
    
}

// Funkcja zwracajaca liczbe bitow pamieci zaalakowanej na dane znajdujace sie
// w tablicy bitowej "bitset".
size_t bitset_get_allocated_bits_size(Bitset *bitset) {
    return bitset->allocated_bits;
}

// Funkcja zwalniajaca pamiec zaalokowana dla tablicy bitowej "bitset"
void bitset_free(Bitset *bitset) {
    free(bitset->data);
    free(bitset);
}