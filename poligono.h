#ifndef POLIGONO_H
#define POLIGONO_H

#include <stddef.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

struct poligono;

typedef struct poligono poligono_t;

poligono_t *poligono_crear(float vertices[][2], size_t n);
poligono_t *poligono_crear_circular(float radio, int resolucion);

void poligono_destruir(poligono_t *poligono);

void imprimir(poligono_t *poligono); // Imprime por consola los puntos del poligono

size_t poligono_cantidad_vertices(const poligono_t *poligono);
bool poligono_obtener_vertice(const poligono_t *poligono, size_t pos, float *x, float *y);

poligono_t *poligono_clonar(const poligono_t *poligono);

bool poligono_agregar_vertice(poligono_t *poligono, float x, float y);


void poligono_trasladar(poligono_t *poligono, float dx, float dy);

void poligono_rotar(poligono_t *poligono, double rad);
void poligono_rotar2(poligono_t *poligono, double rad);

void poligono_rotar_centro(poligono_t *poligono, double rad, float centro_x, float centro_y);
void poligono_rotar_centro2(poligono_t *poligono, double rad, float centro_x, float centro_y);

bool poligono_punto_dentro(poligono_t *poligono, float px, float py);

bool poligono_dibujar(SDL_Renderer *renderer, poligono_t *poligono);
bool poligono_abierto_dibujar(SDL_Renderer *renderer, poligono_t *poligono);

double poligono_distancia(const poligono_t *p, float xp, float yp, float *nor_x, float *nor_y);

#endif