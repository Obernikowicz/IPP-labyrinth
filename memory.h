// Modul zawierajacy funkcje do alokowania pamieci ze sprawdzeniem
// powodzenia tej operacji.
#ifndef MEMORY_H

#include <stdlib.h>
#include <stdbool.h>

#define MEMORY_H

// Funkcja alokujaca pamiec wielkosci size, sprawdzajaca czy alokacja pamieci
// sie udala i ewentualnie konczaca dzialanie programu z odpowiednim bledem.
extern void *safe_malloc(size_t size);

// Funkcja alokujaca pamiec wielkosci size, wypelniona poczatkowo zerami,
// sprawdzajaca czy alokacja pamieci sie udala i ewentualnie konczaca dzialanie
// programu z odpowiednim bledem.
extern void *safe_calloc(size_t elements, size_t element_size);

// Funkcja realokujaca pamiec wielkosci size, sprawdzajaca czy alokacja pamieci
// sie udala i ewentualnie konczaca dzialanie programu z odpowiednim bledem.
extern void *safe_realloc(void *ptr, size_t size);

#endif