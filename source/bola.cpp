#include "bola.h"

#include <cmath>
#include <iomanip>

#include "config.h"
#include "utility.h"
#include "vec2.h"

//------------------------------------------ BOLA
//------------------------------------------

const aVec2 g(0, G);

Ball::Ball(aVec2 c, float radio, unsigned int resolucion)
    : centro(c), velocidad(aVec2Zero), radio(radio), cayendo(false) {}

Ball::Ball(float radio, unsigned int resolucion)
    : Ball(aVec2Zero, radio, resolucion) {}

bool Ball::eyectar(float angle) {
    if (cayendo) return false;
    cayendo = true;
    velocidad.set(BOLA_VI * std::sin(angle), BOLA_VI * std::cos(angle));
    return true;
}
#include <iostream>
void Ball::imprimir() {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Centro: (" << centro.x << ";" << centro.y << ") ";
}
void Ball::reset() {
    velocidad.setZero();
    cayendo = false;
    // TODO volver a posicion de origen
}

bool Ball::esta_cayendo() const { return cayendo; }

aVec2 Ball::position() const { return centro; }
aVec2 Ball::get_velocidad() const { return velocidad; }

void Ball::set_position(aVec2 p) {
    // forma.trasladar(-centro);
    // forma.imprimir();
    // forma.trasladar();
    centro = p;
    // forma.trasladar(p);
    // centro = p;
}

void Ball::actualizar(double dt) {
    if (not cayendo) return;
    velocidad = ROZAMIENTO * computar_velocidad(velocidad, g, dt);
    aVec2 nuevo_centro = computar_posicion(centro, velocidad, dt);
    // std::cout << "nuevo centro = " << nuevo_centro.x << " " << nuevo_centro.y
    //           << std::endl;
    if (std::fabs(velocidad.y) < 15 and aDistance(centro, nuevo_centro) < 0.5)
        iterations_stucked++;
    else {
        iterations_stucked = 0;
        stucked = false;
    }

    if (iterations_stucked > 120) {
        stucked = true;
    }
    set_position(nuevo_centro);
    // checkear si la bola está estancada
    rebotar_si_hay_pared();
}

void Ball::rebotar_si_hay_pared() {
    if (centro.x < MIN_X + BOLA_RADIO or centro.x > MAX_X - BOLA_RADIO)
        velocidad.x = -velocidad.x;
    if (centro.y < MIN_Y + BOLA_RADIO) velocidad.y = -velocidad.y;
}

void Ball::reflejar(aVec2 norm) {
    ::reflejar(norm, centro, velocidad);
    // float proy = aDot(norm, velocidad);
    //
    // if (proy >= 0) return;
    //
    // velocidad -= 2 * norm * proy;
    // centro += norm * 0.1;
    velocidad *= PLASTICIDAD;
}

bool Ball::esta_trabada() const { return stucked; }
