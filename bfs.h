// Modul zawierajacy funkcje do przeszukiwania labiryntu

#ifndef BFS_H

#include <stdbool.h>
#include "labyrinth_data.h"
#define BFS_H

// Funcka przeszukujaca labirynt w poszukiwaniu najktotszej sciezki.
// labyrinth - wskaznik na strukture zawierajaca dane o labiryncie
// does_way_esists - wskaznik na zmienna, w ktorej zostanie zapisana informacja,
// czy istnieje droga laczaca pozycje poczatkowa i koncowa.
extern size_t bfs(Labyrinth_data *labyrinth, bool *does_way_exist);

#endif