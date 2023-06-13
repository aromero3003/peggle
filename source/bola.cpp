#include "bola.h"

//------------------------------------------ BOLA
//------------------------------------------

bola_t::bola_t(float cx, float cy, float radio, int resolucion)
    : forma(radio, resolucion) {
    forma.trasladar(cx, cy);
}

bool bola_t::dibujar(SDL_Renderer *renderer) { return forma.dibujar(renderer); }
