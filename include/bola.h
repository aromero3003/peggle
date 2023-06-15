#ifndef __BOLA_H__
#define __BOLA_H__

#include "poligono.h"
#include "vec2.h"

struct bola_t {
    bola_t(aVec2 c, float radio, unsigned int resolucion);
    bola_t(float radio, unsigned int resolucion);

    void reset();
    void actualizar(double dt);

    void set_position(aVec2 p);
    aVec2 get_velocidad();

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
