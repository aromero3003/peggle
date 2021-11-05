#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <SDL2/SDL.h>
#include <stdio.h>

#include "poligono.h"
#include "tipos.h"

struct obstaculo;
typedef struct obstaculo obstaculo_t;

bool obstaculo_leer_cantidad_de_obstaculos(FILE *f, int16_t *cantidad);

void imprimir_obstaculo(obstaculo_t *o);

obstaculo_t *obstaculo_crear(poligono_t *puntos, color_t color, movimiento_t mov, float parametros[3], geometria_t geo);
void obstaculo_destruir(obstaculo_t* obs); 

obstaculo_t *obstaculo_levantar_obstaculo(FILE *f); 

void obstaculo_trasladar(obstaculo_t* obs, float dx, float dy);
void obstaculo_rotar(obstaculo_t *obs, double rad);
void obstaculo_rotar_centro(obstaculo_t *obstaculo, double rad, float cx, float cy);



// FUNCIONES PARA MOVER EL OBSTACULO
void obstaculo_mover_inmovil(obstaculo_t *obstaculo, double dt);    
void obstaculo_mover_horizontal(obstaculo_t *obstaculo, double dt);
void obstaculo_mover_circular(obstaculo_t *obstaculo, double dt);

void obstaculo_mover(obstaculo_t *obstaculo, double dt);


bool obstaculo_dibujar(SDL_Renderer *renderer, obstaculo_t *obstaculo);

bool obstaculo_es_gris(obstaculo_t *obstaculo);
bool obstaculo_es_naranja(obstaculo_t *obstaculo);
color_t obstaculo_get_color(obstaculo_t *obstaculo);


bool obstaculo_get_tocado(obstaculo_t *obstaculo);
void obstaculo_set_tocado(obstaculo_t *obstaculo, bool state);

bool obstaculo_get_dibujar(obstaculo_t *obstaculo);
void obstaculo_set_dibujar(obstaculo_t *obstaculo, bool state);


double obstaculo_distancia(const obstaculo_t *poligono, float xp, float yp, float *nor_x, float *nor_y);


#endif