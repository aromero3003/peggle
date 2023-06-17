#include "obstaculo.h"

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#include <iostream>

#include "config.h"
#include "tipos.h"
#include "vec2.h"

Obstacle::Obstacle(Polygon p, color_t c, movimiento_t m, geometria_t g,
                   float parametros[3])
    : poligono(p),
      color(c),
      movimiento(m),
      geometria(g),
      tocado(false),
      _dibujar(true) {
    this->parametros[0] = parametros[0];
    this->parametros[1] = parametros[1];
    this->parametros[2] = parametros[2];
}

// Imprime por stdout los parametros de un obstaculo
void Obstacle::imprimir_obstaculo() const {
    // printf("Mov: %d For: %d Col: %d\n", movimiento, geometria, color);
    std::cout << "Mov :" << movimiento << " For: " << geometria
              << " Col: " << color << std::endl;
    poligono.imprimir();
    std::cout << std::endl;
    // putchar('\n');
}

bool obstaculo_cantidad_en_nivel(FILE *f, int16_t *cant) {
    if (f == NULL) return false;
    return fread(cant, sizeof(int16_t), 1, f) != 1;
}

bool Obstacle::es_gris() const { return color == COLOR_GRIS; }

bool Obstacle::es_naranja() const { return color == COLOR_NARANJA; }

color_t Obstacle::get_color() const { return color; }

void Obstacle::trasladar(float dx, float dy) {
    poligono.trasladar(aVec2(dx, dy));
}

void Obstacle::rotar(double rad) { poligono.rotar2(rad); }

void Obstacle::rotar_centro(double rad, aVec2 c) {
    poligono.rotar_centro2(rad, c);
}

// FUNCIONES PARA MOVER EL OBSTACULO

void Obstacle::mover_inmovil(double dt) { return; }

void Obstacle::mover_horizontal(double dt) {
    trasladar(parametros[2] * dt, 0);

    parametros[1] += (parametros[2] * dt);

    if (parametros[1] < 0 || parametros[1] > parametros[0])
        parametros[2] = -parametros[2];
}

void Obstacle::mover_circular(double dt) {
    rotar_centro(parametros[2] * dt * 60, aVec2(parametros[0], parametros[1]));
}

// static const void (*const funciones_obstaculo_mover[])(Obstacle
// *obstaculo,
//                                                        double dt) = {
//     [INMOVIL] = obs.mover_inmovil,
//     [HORIZONTAL] = obs.mover_horizontal,
//     [CIRCULAR] = obs.mover_circular};

// Mover al obstaculo segun su movimiento
void Obstacle::mover(double dt) {
    if (movimiento == INMOVIL)
        mover_inmovil(dt);
    else if (movimiento == HORIZONTAL)
        mover_horizontal(dt);
    else if (movimiento == CIRCULAR)
        mover_circular(dt);

    // funciones_obstaculo_mover[movimiento](obstaculo, dt);
}

// Dibuja un obstaculo sobre un SDL_Renderer
bool Obstacle::dibujar(SDL_Renderer *renderer) const {
    if (tocado)
        SDL_SetRenderDrawColor(renderer, 0xFF, 0XFF, 0x00, 0x00);
    else {
        switch (color) {
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
    return _dibujar ? poligono.dibujar(renderer) : true;
}

void Obstacle::set_tocado(bool state) { tocado = state; }

bool Obstacle::get_tocado() const { return tocado; }

void Obstacle::set_dibujar(bool state) { _dibujar = state; }

bool Obstacle::get_dibujar() const { return _dibujar; }

double Obstacle::distancia(aVec2 p, aVec2 &norma) const {
    return poligono.distancia(p, norma);
}
