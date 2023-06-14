#include "cannon.h"

#include <cmath>

#include "config.h"

Cannon::Cannon(float x0, float y0)
    : reference_tip(0, CANON_LARGO), bottom(x0, y0) {}

void Cannon::tip(float &x, float &y) {
    x = bottom.x + reference_tip.x;
    y = bottom.y + reference_tip.y;
}

void Cannon::update(float angle) {
    if (angle > CANON_MAX or angle < -CANON_MAX) return;
    reference_tip.set(std::sin(angle), std::cos(angle));
    reference_tip *= CANON_LARGO;
}
