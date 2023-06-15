#include "bola.h"

#include "config.h"
#include "utility.h"
#include "vec2.h"

//------------------------------------------ BOLA
//------------------------------------------

const aVec2 g(0, G);

bola_t::bola_t(aVec2 c, float radio, unsigned int resolucion)
    : centro(c),
      velocidad(aVec2Zero),
      forma(radio, resolucion),
      radio(radio),
      cayendo(false) {
    forma.trasladar(centro);
}

bola_t::bola_t(float radio, unsigned int resolucion)
    : bola_t(aVec2Zero, radio, resolucion) {}

bool bola_t::eyectar(float angle) {
    if (cayendo) return false;
    velocidad.set(BOLA_VI * std::cos(angle), BOLA_VI * std::sin(angle));
    return true;
}

void bola_t::reset() {
    velocidad.setZero();
    cayendo = false;
    // TODO volver a posicion de origen
}

aVec2 bola_t::get_velocidad() { return velocidad; }

void bola_t::set_position(aVec2 p) {
    forma.trasladar(p - centro);
    centro = p - centro;
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
