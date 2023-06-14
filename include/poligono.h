#ifndef POLIGONO_H
#define POLIGONO_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stddef.h>

#include <vector>

#include "vec2.h"

struct poligono_t {
    poligono_t() = delete;
    poligono_t(float vertices[][2], size_t n);
    poligono_t(const std::vector<aVec2> &vertices);
    poligono_t(float radio, int resolucion);
    poligono_t(const poligono_t &p);

    poligono_t &operator=(const poligono_t &other);

    poligono_t *crear(float vertices[][2], size_t n);
    poligono_t *crear(const std::vector<aVec2> &vertices);
    poligono_t *crear_circular(float radio, int resolucion);

    void destruir();

    void imprimir() const;  // Imprime por consola los puntos del poligono

    size_t cantidad_vertices() const;
    bool obtener_vertice(const size_t pos, float *x, float *y) const;

    bool agregar_vertice(float x, float y);

    void trasladar(float dx, float dy);

    void rotar(double rad);
    void rotar2(double rad);

    void rotar_centro(double rad, float centro_x, float centro_y);
    void rotar_centro2(double rad, float centro_x, float centro_y);

    bool punto_dentro(float px, float py) const;

    bool dibujar(SDL_Renderer *renderer);
    bool abierto_dibujar(SDL_Renderer *renderer);

    double distancia(float xp, float yp, float *nor_x, float *nor_y) const;

   private:
    std::vector<aVec2> vertices;
};

#endif
