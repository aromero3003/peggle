#include "cannon.h"

#include <SDL_render.h>

#include <cmath>

#include "ball.h"
#include "settings.h"
#include "vec2.h"

Cannon::Cannon(aVec2 p0, Ball &bala) : bottom(p0), ball(bala) {}

aVec2 Cannon::tip() const {
    aVec2 rotatedTip(std::sin(rotation), std::cos(rotation));
    rotatedTip *= CANON_LARGO;
    return bottom + rotatedTip;
}

float Cannon::angle() const { return rotation; }

bool Cannon::fire() { return ball.eyectar(rotation); }

bool Cannon::reload() {
    if (not ball.esta_cayendo()) return false;
    ball.reset();
    ball.set_position(tip());
    return true;
}

void Cannon::update(float angle) {
    if (angle <= CANON_MAX and angle >= -CANON_MAX) rotation = angle;
    if (not ball.esta_cayendo()) ball.set_position(tip());
}

void Cannon::draw(SDL_Renderer *r) const {
    SDL_RenderDrawLine(r, bottom.x, bottom.y, tip().x, tip().y);
}
