#include <stdlib.h>
#include <stdbool.h>

#include "lista.h"

struct nodo;

typedef struct nodo {
	void *elem;
	struct nodo *prox;
} nodo_t;

struct lista {
	nodo_t *prim;
};

struct lista_iter {
	nodo_t *act;
	nodo_t *ant;
};


static nodo_t *nodo_crear(nodo_t *prox, void *elem) {
	nodo_t *nodo = malloc(sizeof(nodo_t));
	if(nodo == NULL) return NULL;
	nodo->elem = elem;
	nodo->prox = prox;
	return nodo;
}

lista_t *lista_crear() {
	
	lista_t * lista = malloc(sizeof(lista_t));
	if(lista == NULL) 
		return NULL;

	lista->prim = NULL;

	return lista;
}

void lista_destruir(lista_t *l, void (*destruccion)(void *)) {
	nodo_t *act = l->prim;
	while(act != NULL) {
		nodo_t *prox = act->prox;

		if(destruccion != NULL) {
			destruccion(act->elem);
		}		
		free(act);

		act = prox;
	}
	free(l);
}


bool lista_es_vacia(const lista_t *l) { 
	return l->prim == NULL;
}


void *lista_primero(const lista_t *l) {
	return l->prim->elem;
}

size_t lista_largo(lista_t *lista) {
    size_t n = 0;
    nodo_t *act = lista->prim;
    while (act != NULL) {
        n++;
        act = act->prox;
    }
    return n;
}


bool lista_insertar_al_principio(lista_t *l, void *elem) {
	nodo_t *nodo = nodo_crear(l->prim, elem);
	if(nodo == NULL) return false;
	l->prim = nodo;
	return true;
}


bool lista_insertar_al_final(lista_t *l, void *elem) {
	nodo_t *nuevo = nodo_crear(NULL, elem);
	if(nuevo == NULL) return false;
	
	if(l->prim == NULL) {
		l->prim = nuevo;
		return true;
	}
	
	nodo_t *act = l->prim;
	while(act->prox != NULL) {
		act = act->prox;
	}

	act->prox = nuevo;
	return true;
}


void *lista_borrar_al_principio(lista_t *l) {
	if(l->prim == NULL) return NULL;
	
	void *dato = l->prim->elem;
	
	nodo_t *aux = l->prim;
	l->prim = l->prim->prox;

	free(aux);
	
	return dato;
}


void *lista_borrar_al_final(lista_t *l) {
	if(l->prim == NULL) return NULL;
	
	nodo_t *act = l->prim;
	nodo_t *ant = NULL;
	while(act->prox != NULL) {
		ant = act;
		act = act->prox;
	}
	void *dato = act->elem;
	if(ant != NULL)
		ant->prox = NULL;
	else
		l->prim = NULL;

	free(act);
	return dato;
}	


void lista_recorrer(const lista_t *l, bool (*f)(void *, void *), void *extra) {
	int i = 0;
	nodo_t *act = l->prim;
	while(act != NULL) {
//		printf("NUMERO %d\n", i);
		i++;
		if(! f(act->elem, extra))
			return;
		act = act->prox;
	}
}

lista_iter_t *lista_iter_crear(lista_t *lista) {
	lista_iter_t *i = malloc(sizeof(lista_iter_t));
	if(i == NULL) return NULL;

	i->act = lista->prim;
	i->ant = NULL;
	return i;
}

bool lista_iter_al_final(const lista_iter_t *i) {
	return i->act == NULL;
}

void *lista_iter_ver_actual(const lista_iter_t *i) {
	//if(lista_iter_al_final(i))
	//	return NULL;
	
	return i->act->elem;
}

bool lista_iter_avanzar(lista_iter_t *i) {
	if(lista_iter_al_final(i))
		return false;
	
	i->ant = i->act;
	i->act = i->act->prox;

	return true;
}

void lista_iter_destruir(lista_iter_t *i) {
	free(i);
}