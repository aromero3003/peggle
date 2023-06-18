#ifndef __BALL_H__
#define __BALL_H__

#include "polygon.h"
#include "vec2.h"

struct Ball {
    Ball(aVec2 c, float radio, unsigned int resolucion);
    Ball(float radio, unsigned int resolucion);

    void reset();
    void actualizar(double dt);
    void rebotar_si_hay_pared();
    void reflejar(aVec2 norm);

    void set_position(aVec2 p);

    aVec2 position() const;
    aVec2 get_velocidad() const;
    bool esta_cayendo() const;
    bool esta_trabada() const;

    bool eyectar(float angle);

    void imprimir();

    // private:
    aVec2 centro;
    aVec2 velocidad;
    float radio;
    bool cayendo;

    bool stucked;
    uint8_t iterations_stucked;
};

#endif /* __BALL_H__ */
