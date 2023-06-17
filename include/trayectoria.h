#ifndef __TRAYECTORIA_H__
#define __TRAYECTORIA_H__
#include <SDL.h>

#include <vector>

#include "poligono.h"
#include "vec2.h"

class Trajectory {
   public:
    Trajectory();
    Trajectory(Trajectory &&) = default;
    Trajectory(const Trajectory &) = default;
    Trajectory &operator=(Trajectory &&) = default;
    Trajectory &operator=(const Trajectory &) = default;
    ~Trajectory() = default;

    bool agregar_coordenada(aVec2 p);
    bool dibujar(SDL_Renderer *renderer);

   private:
    std::vector<aVec2> linea;
};

Trajectory calcular(aVec2 pi, aVec2 vi, aVec2 a, float dt);

double computar_velocidad(double vi, double a, double dt);

double computar_posicion(double pi, double vi, double dt);
#endif /* __TRAYECTORIA_H__ */
