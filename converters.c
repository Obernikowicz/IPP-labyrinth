// Modul zawierajacy funkcje konwerujace dane

#include <stdlib.h>
#include "bitset.h"
#include "converters.h"

// Roznica miedzy kolejnymi bitami, ktorych stan okreslaja reszty w_i, wyliczone
// zgodnie ze specyfikacja danych wejsciowych. Liczba jest rowna 2^32.
#define BIT_DIFFERENCE 4294967296

// Funkcja konwerujaca zapis inforamcji o scianach w labiryncie w postaci
// litery "R" i piecu liczb (zapisany w zmiennej r_values, bedaca struktura
// sluzaca bardziej zwiezwemu przechowywaniu ich) do zapisu binarnego.
Bitset *convert_r_values_to_walls_data_bitset(R_values r_values,
                                              size_t labyrinth_size) {

    // Inicjowanie tablicy bitowej, ktora bedzie zawierac binarny zapis danych
    // o scianach w labiryncie                                            
    Bitset *walls_data = bitset_init(labyrinth_size);
    
    // Zmienna pomocnicza do wyliczania kolejnych wartosci s_i. Zmienna jest
    // konieczna, poniewaz wartosci wyliczane sa rekurencyjnie.
    size_t last_s = r_values.s;

    for (size_t i = 1; i <= r_values.r; i ++) {
        // Wyliczanie, zgodnie ze wzorem w specyfikacji danych wejsciowych,
        // kolejnych wartosci s_i oraz w_i
        size_t last_s_mod = last_s % r_values.m;
        size_t a_mod = r_values.a % r_values.m;
        size_t b_mod = r_values.b % r_values.m;
        
        size_t current_s =
            (((last_s_mod * a_mod) % r_values.m) + b_mod) % r_values.m;
        last_s = current_s;

        // Wartosc przechowujaca, ktory bit, zgodnie ze wzorem, ma zostac
        // oznaczony jako zapalony.
        size_t current_bit = current_s % labyrinth_size;
        
        while (true) {
            bitset_set(walls_data, current_bit, 1);

            // Sprawdzanie, czy bit przesuniety o kolejne 2^32, bedzie dalej
            // miescil sie w zakresie, czyli czy nie bedzie wiekszy od rozmiaru
            // labiryntu. Taka postac drugiego warunku zapobiega przepelnienu
            // arytmetyki.
            if(labyrinth_size < BIT_DIFFERENCE
               || current_bit > labyrinth_size - BIT_DIFFERENCE - 1) {
                   
                break;
            }

            current_bit += BIT_DIFFERENCE;
        }
    }

    return walls_data;
}