#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "vec2.h"

// Common physics utility for game

inline aVec2 computar_velocidad(aVec2 vi, aVec2 a, double dt) {
    return a * dt + vi;
}

inline aVec2 computar_posicion(aVec2 pi, aVec2 vi, double dt) {
    return vi * dt + pi;
}

inline void reflejar(aVec2 norm, aVec2 &c, aVec2 &v) {
    float proy = aDot(norm, v);

    if (proy >= 0) return;

    v -= 2 * norm * proy;
    c += norm * 0.1;
}

#endif /* __UTILITY_H__ */
