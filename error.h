// Modul zawierajacy funkcje konczoca program wywolywana w przypadku napotkania
// bledu 

#ifndef ERROR_H

#define ERROR_H

// Funkcja konczaca dzialanie programu ze wzgledu
// na blad w linijce "error_code".
extern void exit_with_error(int error_code);

#endif