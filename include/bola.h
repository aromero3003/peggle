#ifndef __BOLA_H__
#define __BOLA_H__

#include "poligono.h"
struct bola_t {
    bola_t(float cx, float cy, float radio, int resolucion);
    void actualizar(double dt);
    void set_position(aVec2 p);

    bool eyectar(float angle);

    bool dibujar(SDL_Renderer *renderer) const;

   private:
    aVec2 centro;
    aVec2 velocidad;
    poligono_t forma;
    float radio;
    bool cayendo;
};

#endif /* __BOLA_H__ */
