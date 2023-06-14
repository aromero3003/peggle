#include "bola.h"

#include "vec2.h"

//------------------------------------------ BOLA
//------------------------------------------

bola_t::bola_t(float cx, float cy, float radio, int resolucion)
    : forma(radio, resolucion) {
    forma.trasladar(aVec2(cx, cy));
}

bool bola_t::dibujar(SDL_Renderer *renderer) const {
    return forma.dibujar(renderer);
}
