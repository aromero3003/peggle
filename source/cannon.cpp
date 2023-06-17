#include "cannon.h"

#include <SDL_render.h>

#include <cmath>

#include "bola.h"
#include "config.h"
#include "vec2.h"

Cannon::Cannon(aVec2 p0, bola_t &bala) : bottom(p0), bullet(bala) {}

aVec2 Cannon::tip() const {
    aVec2 rotatedTip(std::sin(rotation), std::cos(rotation));
    rotatedTip *= CANON_LARGO;
    return bottom + rotatedTip;
}

float Cannon::angle() const { return rotation; }

bool Cannon::fire() { return bullet.eyectar(rotation); }

bool Cannon::reload() {
    if (not bullet.esta_cayendo()) return false;
    bullet.reset();
    bullet.set_position(tip());
    bullet.imprimir();
    return true;
}

void Cannon::update(float angle) {
    if (angle <= CANON_MAX and angle >= -CANON_MAX) rotation = angle;
    if (not bullet.esta_cayendo()) bullet.set_position(tip());
}

void Cannon::draw(SDL_Renderer *r) const {
    SDL_RenderDrawLine(r, bottom.x, bottom.y, tip().x, tip().y);
}
