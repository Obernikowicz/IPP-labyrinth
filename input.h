// Modul odpowowiadajacy za wczytywanie danych wejsciowych.
#ifndef INPUT_H

#include <inttypes.h>
#include "dynamic_array.h"
#include "labyrinth_data.h"
#include "bitset.h"

#define INPUT_H

// Funkcja wczytujaca dane wejsciowe i zwracajaca je w postaci wskaznika
// do struktury typu Labyrinth_data, umozliwiajacej zwiezle przechowywanie
// informacji o labiryncie. Funkcja jednoczesnie czesciowo weryfikuje
// wprowadzone dane.
extern Labyrinth_data *get_input_data();

#endif