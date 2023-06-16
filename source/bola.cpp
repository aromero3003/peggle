#include "bola.h"

#include <iomanip>

#include "config.h"
#include "utility.h"
#include "vec2.h"

//------------------------------------------ BOLA
//------------------------------------------

const aVec2 g(0, G);

bola_t::bola_t(aVec2 c, float radio, unsigned int resolucion)
    : centro(c), velocidad(aVec2Zero), radio(radio), cayendo(false) {}

bola_t::bola_t(float radio, unsigned int resolucion)
    : bola_t(aVec2Zero, radio, resolucion) {}

bool bola_t::eyectar(float angle) {
    if (cayendo) return false;
    cayendo = true;
    velocidad.set(BOLA_VI * std::sin(angle), BOLA_VI * std::cos(angle));
    return true;
}
#include <iostream>
void bola_t::imprimir() {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Centro: (" << centro.x << ";" << centro.y << ") ";
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
    // forma.trasladar(-centro);
    // forma.imprimir();
    // forma.trasladar();
    centro = p;
    // forma.trasladar(p);
    // centro = p;
}

void bola_t::actualizar(double dt) {
    if (not cayendo) return;
    velocidad = ROZAMIENTO * computar_velocidad(velocidad, g, dt);
    aVec2 nuevo_centro = computar_posicion(centro, velocidad, dt);
    // std::cout << "nuevo centro = " << nuevo_centro.x << " " << nuevo_centro.y
    //           << std::endl;
    set_position(nuevo_centro);
    // checkear si la bola está estancada
    rebotar_si_hay_pared();
}

void bola_t::rebotar_si_hay_pared() {
    if (centro.x < MIN_X + BOLA_RADIO or centro.x > MAX_X - BOLA_RADIO)
        velocidad.x = -velocidad.x;
    if (centro.y < MIN_Y + BOLA_RADIO) velocidad.y = -velocidad.y;
}

void bola_t::reflejar(aVec2 norm) {
    ::reflejar(norm, centro, velocidad);
    // float proy = aDot(norm, velocidad);
    //
    // if (proy >= 0) return;
    //
    // velocidad -= 2 * norm * proy;
    // centro += norm * 0.1;
    velocidad *= PLASTICIDAD;
}
