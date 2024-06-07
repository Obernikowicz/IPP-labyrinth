// Modul zawierajacy funkcje do alokowania pamieci ze sprawdzeniem
// powodzenia tej operacji.
#include <stdlib.h>
#include <stdbool.h>
#include "error.h"

// Funkcja alokujaca pamiec wielkosci size, sprawdzajaca czy alokacja pamieci
// sie udala i ewentualnie konczaca dzialanie programu z odpowiednim bledem.
void *safe_malloc(size_t size) {
    void *ptr = malloc(size);

    if(ptr == NULL) {
        exit_with_error(0);
    }

    return ptr;
}

// Funkcja alokujaca pamiec wielkosci size, wypelniona poczatkowo zerami,
// sprawdzajaca czy alokacja pamieci sie udala i ewentualnie konczaca dzialanie
// programu z odpowiednim bledem.
void *safe_calloc(size_t elements, size_t element_size) {
    void *ptr = calloc(elements, element_size);

    if(ptr == NULL) {
        exit_with_error(0);
    }

    return ptr;
}

// Funkcja realokujaca pamiec wielkosci size, sprawdzajaca czy alokacja pamieci
// sie udala i ewentualnie konczaca dzialanie programu z odpowiednim bledem.
void *safe_realloc(void *ptr, size_t size) {
    void *new_ptr = realloc(ptr, size);

    if(new_ptr == NULL) {
        exit_with_error(0);
    }

    return new_ptr;
}