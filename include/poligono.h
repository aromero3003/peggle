#ifndef POLIGONO_H
#define POLIGONO_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stddef.h>

#include <vector>

#include "vec2.h"

struct poligono_t {
    poligono_t() = default;
    poligono_t(float vertices[][2], size_t n);
    poligono_t(const std::vector<aVec2> &vertices);
    poligono_t(float radio, int resolucion);

    poligono_t *crear(float vertices[][2], size_t n);
    poligono_t *crear(const std::vector<aVec2> &vertices);
    poligono_t *crear_circular(float radio, int resolucion);

    void destruir(poligono_t *poligono);

    void imprimir(poligono_t *poligono)
        const;  // Imprime por consola los puntos del poligono

    size_t cantidad_vertices(const poligono_t *poligono) const;
    bool obtener_vertice(const poligono_t *poligono, size_t pos, float *x,
                         float *y) const;

    poligono_t *clonar(const poligono_t *poligono);

    bool agregar_vertice(poligono_t *poligono, float x, float y);

    void trasladar(poligono_t *poligono, float dx, float dy);

    void rotar(poligono_t *poligono, double rad);
    void rotar2(poligono_t *poligono, double rad);

    void rotar_centro(poligono_t *poligono, double rad, float centro_x,
                      float centro_y);
    void rotar_centro2(poligono_t *poligono, double rad, float centro_x,
                       float centro_y);

    bool punto_dentro(poligono_t *poligono, float px, float py) const;

    bool dibujar(SDL_Renderer *renderer, poligono_t *poligono);
    bool abierto_dibujar(SDL_Renderer *renderer, poligono_t *poligono);

    double distancia(const poligono_t *p, float xp, float yp, float *nor_x,
                     float *nor_y) const;

   private:
    std::vector<aVec2> vertices;
};

#endif
