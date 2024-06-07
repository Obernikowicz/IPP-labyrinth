// Modul zawierajacy funkcje do weryfikacji czesci danych o labiryncie
#include <stdlib.h>
#include <stdbool.h>
#include "dynamic_array.h"
#include "labyrinth_data.h"
#include "error.h"
#include "validation.h"

// Funkcja liczaca wielkosc labiryntu i sprawdzajaca, czy wielkosc ta 
// nie wykracza poza zakres arytmetyki komputera.
// array - tablica wielkosci wymiarow labirynty
// overflow - wskaznik na zmienna, w ktorej zostanie zapisana informacja,
//      czy nastapilo przepelnienie arytmetyki komputera.
size_t calculate_validate_labyrinth_size(Array *array, bool *overflow) {
    size_t result = 1;

    for (size_t i = 0; i < array_length(array); i ++) {
        size_t multiplication_result = result * array_get(array, i);
        
        // W ponizszym warunku sprawdzane jest, czy odwrotnosc operacji 
        // mnozenia daje odpowiedni wynik. Wielkosci wymiarow sa niezerowe,
        // wiec dzielenie nie przepelni arytmetyki komputera ani nie skonczy
        // sie bledem, wiec jesli wynik dzielenia nie zgadza sie z 
        // oczekiwana, nastapilo przepelnienie arytmetyki komputera.
        if (multiplication_result / array_get(array, i) != result) {
            *overflow = true;
            
            return result;
        }

        result = multiplication_result;
    }
    
    *overflow = false;
    return result;
}

// Funkcja sprawdzajaca, czy kazda w kazdym z wymiarow pozycje poczatkowa
// i koncowa nie wykraczaja poza wymiary labiryntu oraz sprawdzajaca,
// czy pozycja poczatkowa i koncowa nie znajduja sie w scianie labiryntu.
// W przypadku bledu program jest konczony z wyswietleniem odpowiedniego
// komunikatu.
// labyrinth_data - wskaznik na strukture typu Labyrinth_data, przechowujaca
//      dane o labiryncie.
void validate_labyrinth_data(Labyrinth_data *labyrinth_data) {
    for (size_t i = 0; i < get_dimensions_size(labyrinth_data); i ++) {
        if (get_start_position_value(labyrinth_data, i)
            > get_dimensions_value(labyrinth_data, i)) {
            
            // Pozycja poczatkowa w wymiarze i wykracza poza wielkosc
            // tego wymiaru, czyli pozycja poczatkowa jest niepoprawna.
            labyrinth_free(labyrinth_data);

            exit_with_error(2);
        }

        if (get_end_position_value(labyrinth_data, i)
            > get_dimensions_value(labyrinth_data, i)) {
            
            // Pozycja koncowa w wymiarze i wykracza poza wielkosc
            // tego wymiaru, czyli pozycja koncowa jest niepoprawna.
            labyrinth_free(labyrinth_data);
            
            exit_with_error(3);
        }
    }

    // Liczenie pozycji w zapisie bitowym odpowiadajacych pozycji poczatkowej
    // i koncowej wedlug wzoru:
    // (z_1 - 1) + (z_2 - 1) * n_1 + (z_3 - 1) * n_1 * n_2 + ... +
    // + (z_k - 1) * n_1 * n_2 * ... * n_(k-1)
    size_t start_position_bit_index =
        get_start_position_value(labyrinth_data, 0) - 1;

    size_t end_position_bit_index =
        get_end_position_value(labyrinth_data, 0) - 1;

    size_t multipler = get_dimensions_value(labyrinth_data, 0);
    for (size_t i = 1; i < get_dimensions_size(labyrinth_data); i ++) {
        start_position_bit_index +=
            (get_start_position_value(labyrinth_data, i) - 1) * multipler;

        end_position_bit_index +=
            (get_end_position_value(labyrinth_data, i) - 1) * multipler;

        multipler *= get_dimensions_value(labyrinth_data, i);
    }

    if (get_walls_data_bit_value(labyrinth_data,
                                 start_position_bit_index) == 1) {
        
        // Jezeli pozycja poczatkowa znajduje sie w scianie, to dane
        // wejsciowe sa niepoprawne.
        labyrinth_free(labyrinth_data);

        exit_with_error(2);
    }

    if (get_walls_data_bit_value(labyrinth_data,
                                 end_position_bit_index) == 1) {

        // Jezeli pozycja koncowa znajduje sie w scianie, to dane
        // wejsciowe sa niepoprawne.                             
        labyrinth_free(labyrinth_data);

        exit_with_error(3);
    }
}
