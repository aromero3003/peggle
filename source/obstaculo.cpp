#include "obstaculo.h"

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#include "config.h"
#include "tipos.h"

poligono_t p(nullptr, 0);

obstaculo_t::obstaculo_t(poligono_t p, color_t c, movimiento_t m, geometria_t g,
                         float parametros[3])
    : poligono(p),
      color(c),
      movimiento(m),
      geometria(g),
      tocado(false),
      _dibujar(true) {
    parametros[0] = parametros[0];
    parametros[1] = parametros[1];
    parametros[2] = parametros[2];
}

// Imprime por stdout los parametros de un obstaculo
void obstaculo_t::imprimir_obstaculo(obstaculo_t *o) const {
    printf("Mov: %d For: %d Col: %d\n", o->movimiento, o->geometria, o->color);
    o->poligono.imprimir();
    putchar('\n');
}

obstaculo_t *obstaculo_t::crear(poligono_t *puntos, color_t color,
                                movimiento_t mov, float parametros[3],
                                geometria_t geo) {
    obstaculo_t *obstaculo = new obstaculo_t;
    if (obstaculo == NULL) return NULL;

    obstaculo->poligono = *puntos;
    obstaculo->color = color;
    obstaculo->movimiento = mov;
    obstaculo->geometria = geo;
    obstaculo->tocado = false;
    obstaculo->_dibujar = true;

    if (parametros != NULL) {
        obstaculo->parametros[0] = parametros[0];
        obstaculo->parametros[1] = parametros[1];
        obstaculo->parametros[2] = parametros[2];
    }
    return obstaculo;
}

bool obstaculo_cantidad_en_nivel(FILE *f, int16_t *cant) {
    if (f == NULL) return false;
    return fread(cant, sizeof(int16_t), 1, f) != 1;
}

void obstaculo_t::destruir(obstaculo_t *obstaculo) { delete obstaculo; }

bool obstaculo_t::es_gris(obstaculo_t *obstaculo) const {
    return obstaculo->color == COLOR_GRIS;
}

bool obstaculo_t::es_naranja(obstaculo_t *obstaculo) const {
    return obstaculo->color == COLOR_NARANJA;
}

color_t obstaculo_t::get_color(obstaculo_t *obstaculo) const {
    return obstaculo->color;
}

void obstaculo_t::trasladar(obstaculo_t *obstaculo, float dx, float dy) {
    obstaculo->poligono.trasladar(dx, dy);
}

void obstaculo_t::rotar(obstaculo_t *obstaculo, double rad) {
    obstaculo->poligono.rotar2(rad);
}

void obstaculo_t::rotar_centro(obstaculo_t *obstaculo, double rad, float cx,
                               float cy) {
    obstaculo->poligono.rotar_centro2(rad, cx, cy);
}

// FUNCIONES PARA MOVER EL OBSTACULO

void obstaculo_t::mover_inmovil(obstaculo_t *obstaculo, double dt) { return; }

void obstaculo_t::mover_horizontal(obstaculo_t *obstaculo, double dt) {
    obstaculo_t::trasladar(obstaculo, obstaculo->parametros[2] * dt, 0);

    obstaculo->parametros[1] += (obstaculo->parametros[2] * dt);

    if (obstaculo->parametros[1] < 0 ||
        obstaculo->parametros[1] > obstaculo->parametros[0])
        obstaculo->parametros[2] = -obstaculo->parametros[2];
}

void obstaculo_t::mover_circular(obstaculo_t *obstaculo, double dt) {
    obstaculo_t::rotar_centro(obstaculo, obstaculo->parametros[2] * dt * 60,
                              obstaculo->parametros[0],
                              obstaculo->parametros[1]);
}

// static const void (*const funciones_obstaculo_mover[])(obstaculo_t
// *obstaculo,
//                                                        double dt) = {
//     [INMOVIL] = obs.mover_inmovil,
//     [HORIZONTAL] = obs.mover_horizontal,
//     [CIRCULAR] = obs.mover_circular};

// Mover al obstaculo segun su movimiento
void obstaculo_t::mover(obstaculo_t *obstaculo, double dt) {
    if (obstaculo->movimiento == INMOVIL)
        obstaculo->mover_inmovil(obstaculo, dt);
    else if (obstaculo->movimiento == HORIZONTAL)
        obstaculo->mover_horizontal(obstaculo, dt);
    else if (obstaculo->movimiento == CIRCULAR)
        obstaculo->mover_circular(obstaculo, dt);

    // funciones_obstaculo_mover[obstaculo->movimiento](obstaculo, dt);
}

// Dibuja un obstaculo sobre un SDL_Renderer
bool obstaculo_t::dibujar(SDL_Renderer *renderer, obstaculo_t *obstaculo) {
    if (obstaculo->tocado)
        SDL_SetRenderDrawColor(renderer, 0xFF, 0XFF, 0x00, 0x00);
    else {
        switch (obstaculo->color) {
            case COLOR_GRIS:
                SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, 0x00);
                break;

            case COLOR_AZUL:
                SDL_SetRenderDrawColor(renderer, 0x5, 0x15, 0xFF, 0x00);
                break;

            case COLOR_VERDE:
                SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
                break;

            case COLOR_NARANJA:
                SDL_SetRenderDrawColor(renderer, 0xFF, 0x60, 0x00, 0x00);
                break;
        }
    }
    return obstaculo->_dibujar ? obstaculo->poligono.dibujar(renderer) : true;
}

void obstaculo_t::set_tocado(obstaculo_t *obstaculo, bool state) {
    obstaculo->tocado = state;
}

bool obstaculo_t::get_tocado(obstaculo_t *obstaculo) const {
    return obstaculo->tocado;
}

void obstaculo_t::set_dibujar(obstaculo_t *obstaculo, bool state) {
    obstaculo->_dibujar = state;
}

bool obstaculo_t::get_dibujar(obstaculo_t *obstaculo) const {
    return obstaculo->_dibujar;
}

double obstaculo_t::distancia(const obstaculo_t *obstaculo, float xp, float yp,
                              float *nor_x, float *nor_y) {
    return p.distancia(&obstaculo->poligono, xp, yp, nor_x, nor_y);
}
