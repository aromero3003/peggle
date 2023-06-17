#ifndef __POLYGON_H__
#define __POLYGON_H__

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stddef.h>

#include <vector>

#include "vec2.h"

struct Polygon {
    Polygon() = delete;
    Polygon(float vertices[][2], size_t n);
    Polygon(const std::vector<aVec2> &vertices);
    Polygon(float radio, size_t resolucion);
    Polygon(const Polygon &p);

    Polygon &operator=(const Polygon &other);

    void imprimir() const;  // Imprime por consola los puntos del poligono

    size_t cantidad_vertices() const;
    bool obtener_vertice(const size_t pos, float &x, float &y) const;
    bool obtener_vertice(const size_t pos, aVec2 &v) const;

    bool agregar_vertice(aVec2 xy);

    void trasladar(aVec2 d);

    void rotar(double rad);
    void rotar2(double rad);

    void rotar_centro(double rad, aVec2 centro);
    void rotar_centro2(double rad, aVec2 centro);

    bool punto_dentro(aVec2 p) const;

    bool dibujar(SDL_Renderer *renderer) const;
    bool abierto_dibujar(SDL_Renderer *renderer) const;

    double distancia(aVec2 punto, aVec2 &norma) const;

   private:
    std::vector<aVec2> vertices;
};

#endif
