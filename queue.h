// Modul implementujacy kolejke FIFO
#ifndef QUEUE_H

#include <stdlib.h>
#include <stdbool.h>

#define QUEUE_H

// Definicja struktury przechowujacej pojedynczy element w kolejce.
struct Node {
    size_t value;
    struct Node *next_element;
};

// Definicja struktury zawierajacej kolejke.
typedef struct Queue {
    size_t size;
    struct Node *first_element;
    struct Node *last_element;
} Queue;

// Funkcja tworzaca nowa kolejke.
extern Queue *queue_init();

// Funkcja sprawdzajaca, czy kolejka "queue" jest pusta.
extern bool queue_is_empty(Queue *queue);

// Funkcja dodajacy nowy element "value" na koniec kolejki "queue".
extern void queue_insert(Queue *queue, size_t value);

// Funkcja zwracajaca wartosc pierwszego elementu w kolejce "queue".
extern size_t queue_front(Queue *queue);

// Funkcja usuwajaca pierwszy element z kolejki "queue".
extern void queue_pop(Queue *queue);

// Funkjca zwalniajaca pamiec zaalkoowana przez kolejke "queue".
extern void queue_free(Queue *queue);

#endif