#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <SDL2/SDL.h>

#include "poligono.h"
#include "tipos.h"

struct obstaculo_t {
    obstaculo_t() = default;
    obstaculo_t(poligono_t p, color_t c, movimiento_t m, geometria_t g,
                float parametros[3]);

    void imprimir_obstaculo(obstaculo_t *o) const;

    obstaculo_t *crear(poligono_t *puntos, color_t color, movimiento_t mov,
                       float parametros[3], geometria_t geo);
    void destruir(obstaculo_t *obs);

    void trasladar(obstaculo_t *obs, float dx, float dy);
    void rotar(obstaculo_t *obs, double rad);
    void rotar_centro(obstaculo_t *obstaculo, double rad, float cx, float cy);

    // FUNCIONES PARA MOVER EL OBSTACULO
    void mover_inmovil(obstaculo_t *obstaculo, double dt);
    void mover_horizontal(obstaculo_t *obstaculo, double dt);
    void mover_circular(obstaculo_t *obstaculo, double dt);

    void mover(obstaculo_t *obstaculo, double dt);

    bool dibujar(SDL_Renderer *renderer, obstaculo_t *obstaculo);

    bool es_gris(obstaculo_t *obstaculo) const;
    bool es_naranja(obstaculo_t *obstaculo) const;
    color_t get_color(obstaculo_t *obstaculo) const;

    bool get_tocado(obstaculo_t *obstaculo) const;
    void set_tocado(obstaculo_t *obstaculo, bool state);

    bool get_dibujar(obstaculo_t *obstaculo) const;
    void set_dibujar(obstaculo_t *obstaculo, bool state);

    double distancia(const obstaculo_t *poligono, float xp, float yp,
                     float *nor_x, float *nor_y);

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
