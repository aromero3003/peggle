#ifndef __TRAYECTORIA_H__
#define __TRAYECTORIA_H__
#include <SDL.h>

#include "poligono.h"

class trayectoria_t {
   public:
    trayectoria_t() = default;
    trayectoria_t(trayectoria_t &&) = default;
    trayectoria_t(const trayectoria_t &) = default;
    trayectoria_t &operator=(trayectoria_t &&) = default;
    trayectoria_t &operator=(const trayectoria_t &) = default;
    ~trayectoria_t() = default;

    bool agregar_coordenada(float x, float y);
    bool dibujar(SDL_Renderer *renderer);

   private:
    poligono_t linea;
};

trayectoria_t calcular(float xi, float yi, float vxi, float vyi, float a,
                       float dt);

double computar_velocidad(double vi, double a, double dt);

double computar_posicion(double pi, double vi, double dt);
#endif /* __TRAYECTORIA_H__ */
