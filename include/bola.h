#ifndef __BOLA_H__
#define __BOLA_H__

#include "poligono.h"
struct bola_t {
    bola_t(float cx, float cy, float radio, int resolucion);
    bool dibujar(SDL_Renderer *renderer) const;

   private:
    poligono_t forma;
};

#endif /* __BOLA_H__ */
