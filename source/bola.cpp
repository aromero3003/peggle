#include "bola.h"

#include "config.h"
#include "vec2.h"

//------------------------------------------ BOLA
//------------------------------------------

const aVec2 g(0, G);

bola_t::bola_t(float cx, float cy, float radio, int resolucion)
    : centro(cx, cy),
      velocidad(aVec2Zero),
      forma(radio, resolucion),
      radio(radio),
      cayendo(false) {
    forma.trasladar(centro);
}

aVec2 computar_velocidad(aVec2 vi, aVec2 a, double dt) { return a * dt + vi; }

aVec2 computar_posicion(aVec2 pi, aVec2 vi, double dt) { return vi * dt + pi; }

bool bola_t::eyectar(float angle) {
    if (cayendo) return false;
    velocidad.set(BOLA_VI * std::cos(angle), BOLA_VI * std::sin(angle));
    return true;
}

void bola_t::actualizar(double dt) {
    if (not cayendo) return;
    velocidad = ROZAMIENTO * computar_velocidad(velocidad, g, dt);
    centro = computar_posicion(centro, velocidad, dt);
    // checkear si la bola está estancada
}

bool bola_t::dibujar(SDL_Renderer *renderer) const {
    return forma.dibujar(renderer);
}
