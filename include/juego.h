#ifndef PEGGLE_H
#define PEGGLE_H

#include <SDL2/SDL.h>

#include "obstaculo.h"
#include "vec2.h"

// ESTRUCTURAS de los tdas

// struct bola;
// typedef struct bola bola_t;

// struct vidas;
// typedef struct vidas vidas_t;

// struct trayectoria;
// typedef struct trayectoria trayectoria_t;

// struct recuperador;
// typedef struct recuperador recuperador_t;

typedef enum {
    GAME_CLOSE,
    GAME_RUNNING,
    GAME_LEVEL_UP,
    GAME_LEVEL_FAILED,
    GAME_OVER
} game_state_t;

typedef size_t puntaje_t;

// Funciones generales para la lógica del juego

// double computar_posicion(double pi, double vi, double dt);
// double computar_velocidad(double vi, double a, double dt);

void reflejar(aVec2 norm, aVec2 &c, aVec2 &v);

// Funciones de la bola

// bola_t *bola_crear(float cx, float cy, float radio, int resolucion);
// void bola_destruir(bola_t *bola);
//
// bool bola_dibujar(SDL_Renderer *renderer, bola_t *bola);

// Funciones de la trayectoria

// trayectoria_t *trayectoria_crear();
// void trayectoria_destruir(trayectoria_t *tray);
//
// bool trayectoria_agregar_coordenada(trayectoria_t *tray, float x, float y);
// bool trayectoria_dibujar(SDL_Renderer *renderer, trayectoria_t *tray);
//
// trayectoria_t *trayectoria_calcular(float xi, float yi, float vxi, float vyi,
//                                     float a, float dt);

// Funciones para las vidas

// vidas_t *vidas_inicializar(size_t n, float cx, float cy);
// void vidas_destruir(vidas_t *vidas);
//
// // void vidas_agregar(vidas_t *vidas);
// void vidas_quitar(vidas_t *vidas);
//
// size_t vidas_restantes(vidas_t *vidas);
// bool vidas_estan_agotadas(vidas_t *vidas);
//
// bool vidas_dibujar(SDL_Renderer *renderer, vidas_t *vidas);
// void vidas_resetear(vidas_t *vidas);

// Funciones para el recuperador de bolas

// recuperador_t *recuperador_crear(float ancho, float alto, float velocidad);
// void recuperador_destruir(recuperador_t *recuperador);
//
// void recuperador_mover(recuperador_t *recuperador, float dt);
// void recuperador_dibujar(SDL_Renderer *renderer, recuperador_t *recuperador);
//
// bool recuperador_bola_recuperada(recuperador_t *recuperador, float cx,
// float cy);

// Funciones para manejar el puntaje

// bool puntaje_dibujar_caja(float x, float y);
void puntaje_actualizar(puntaje_t *puntos_totales, obstaculo_t &obstaculo,
                        int multiplicador);
void puntaje_actualizar_multiplicador(int *multiplicador,
                                      size_t naranjas_golpeados);

#endif
