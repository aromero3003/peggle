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

bool bola_t::esta_cayendo() const { return cayendo; }

aVec2 bola_t::position() const { return centro; }
aVec2 bola_t::get_velocidad() const { return velocidad; }

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

void bola_t::rebotar_si_hay_pared() {
    if (centro.x < MIN_X + BOLA_RADIO or centro.x > MAX_X - BOLA_RADIO)
        velocidad.x *= -1;
    if (centro.y < MIN_Y + BOLA_RADIO) velocidad.y *= -1;
}

void bola_t::reflejar(aVec2 norm) {
    float proy = aDot(norm, velocidad);

    if (proy >= 0) return;

    velocidad -= 2 * norm * proy;
    centro += norm * 0.1;
    velocidad *= PLASTICIDAD;
}

bool bola_t::dibujar(SDL_Renderer *renderer) const {
    return forma.dibujar(renderer);
}
