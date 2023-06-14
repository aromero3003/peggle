#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <SDL2/SDL.h>

#include "poligono.h"
#include "tipos.h"

struct obstaculo_t {
    obstaculo_t() = delete;
    obstaculo_t(poligono_t p, color_t c, movimiento_t m, geometria_t g,
                float parametros[3]);

    void imprimir_obstaculo() const;

    void trasladar(float dx, float dy);
    void rotar(double rad);
    void rotar_centro(double rad, float cx, float cy);

    // FUNCIONES PARA MOVER EL OBSTACULO
    void mover_inmovil(double dt);
    void mover_horizontal(double dt);
    void mover_circular(double dt);

    void mover(double dt);

    bool dibujar(SDL_Renderer *renderer) const;

    bool es_gris() const;
    bool es_naranja() const;
    color_t get_color() const;

    bool get_tocado() const;
    void set_tocado(bool state);

    bool get_dibujar() const;
    void set_dibujar(bool state);

    double distancia(float xp, float yp, float *nor_x, float *nor_y) const;

   private:
    poligono_t poligono;

    color_t color;

    geometria_t geometria;

    movimiento_t movimiento;
    float parametros[3];

    bool tocado;
    bool _dibujar;
};

#endif
