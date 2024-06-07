// Modul zawierajacy funkcje konczoca program wywolywana w przypadku napotkania
// bledu 

#include <stdlib.h>
#include <stdio.h>

// Funkcja konczaca dzialanie programu ze wzgledu
// na blad w linijce "error_code".
void exit_with_error(int error_code) {
    fprintf(stderr, "ERROR %d\n", error_code);

    exit(EXIT_FAILURE);
}