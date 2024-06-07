// Modul implementujacy kolejke FIFO
#include <stdlib.h>
#include "memory.h"
#include "queue.h"

// Funkcja tworzaca nowa kolejke.
Queue *queue_init() {
    Queue *queue = safe_malloc(sizeof(Queue));

    // Poczatkowo kolejka jest pusta.
    queue->size = 0;
    queue->first_element = NULL;
    queue->last_element = NULL;

    return queue;
}

// Funkcja sprawdzajaca, czy kolejka "queue" jest pusta.
bool queue_is_empty(Queue *queue) {
    return queue->size == 0;
}

// Funkcja dodajacy nowy element "value" na koniec kolejki "queue".
void queue_insert(Queue *queue, size_t value) {
    struct Node *new_element_node = safe_malloc(sizeof(struct Node));

    new_element_node->value = value;
    new_element_node->next_element = NULL;

    // Jezeli kolejka jest pusta, to dodawany element bedzie jednoczesnie
    // na jej poczatku i koncu.
    if (queue->first_element == NULL) {
        queue->first_element = new_element_node;
        queue->last_element = new_element_node;
        queue->size ++;

        return;
    }

    // Zmiana wskaznika do ostatniego elementu kolejki.
    queue->last_element->next_element = new_element_node;
    queue->last_element = new_element_node;
    queue->size ++;
}

// Funkcja zwracajaca wartosc pierwszego elementu w kolejce "queue".
size_t queue_front(Queue *queue) {
    return queue->first_element->value;
}

// Funkcja usuwajaca pierwszy element z kolejki "queue".
void queue_pop(Queue *queue) {
    struct Node *popped_element = queue->first_element;

    queue->first_element = queue->first_element->next_element;

    // Podczas wywolywania tej funkcji, tracimy wskaznik do usunietego elementu,
    // wiec trzeba ja od razu zwolnic, by zapobiec wyciekom.
    free(popped_element);

    queue->size --;
}

// Funkjca zwalniajaca pamiec zaalkoowana przez kolejke "queue".
void queue_free(Queue *queue) {
    // Usuwanie elementow zwalnia zaalokowana dla nich pamiec, wiec w ponizszy
    // mozna zwolnic pamiec zaalokowana dla wszystkich elementow w kolejce.
    while (!queue_is_empty(queue)) {
        queue_pop(queue);
    }

    free(queue);
}
