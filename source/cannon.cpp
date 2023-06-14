#include "cannon.h"

#include <SDL_render.h>

#include <cmath>

#include "config.h"
#include "vec2.h"

Cannon::Cannon(aVec2 p0) : bottom(p0) {}

aVec2 Cannon::tip() const {
    aVec2 rotatedTip(std::sin(rotation), std::cos(rotation));
    rotatedTip *= CANON_LARGO;
    return bottom + rotatedTip;
}

float Cannon::angle() const { return rotation; }

void Cannon::update(float angle) {
    if (angle <= CANON_MAX or angle >= -CANON_MAX) rotation = angle;
}

void Cannon::draw(SDL_Renderer *r) const {
    SDL_RenderDrawLine(r, bottom.x, bottom.y, tip().x, tip().y);
}
