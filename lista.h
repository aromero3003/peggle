#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>

struct lista;
typedef struct lista lista_t;

struct lista_iter;
typedef struct lista_iter lista_iter_t;



lista_t *lista_crear();
void lista_destruir(lista_t *t, void (*destruccion)(void *));




bool lista_es_vacia(const lista_t *l);
void *lista_primero(const lista_t *l);
size_t lista_largo(lista_t *lista);


bool lista_insertar_al_principio(lista_t *l, void *elem);
bool lista_insertar_al_final(lista_t *l, void *elem);

void * lista_borrar_al_principio(lista_t *l);
void *lista_borrar_al_final(lista_t *l);

//void *lista_buscar(const lista_t *l, const void *dato, int (*cmp)(const void *a, const void *b));,

//void *lista_borrar(lista_t *l, const void *dato, int (*cmp)(const void *a, const void *b));

void lista_recorrer(const lista_t *l, bool (*f)(void *dato, void *extra), void *extra);


lista_iter_t *lista_iter_crear(lista_t *lista);
void lista_iter_destruir(lista_iter_t *i);

bool lista_iter_al_final(const lista_iter_t *i);
bool lista_iter_avanzar(lista_iter_t *i);

void *lista_iter_ver_actual(const lista_iter_t *iter);

#endif
