#include "poligono.h"

#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include <cmath>
#include <iostream>
#include <vector>

#include "config.h"
#include "vec2.h"

// Crea un polígono de n vertices en R2
poligono_t::poligono_t(float vertices[][2], size_t n) : vertices(n) {
    for (size_t i = 0; i < n; i++) {
        this->vertices[i].set(vertices[i][0], vertices[i][1]);
    }
}

poligono_t::poligono_t(const std::vector<aVec2> &vertices)
    : vertices(vertices) {}

poligono_t::poligono_t(float radio, size_t resolucion) : vertices(resolucion) {
    int angulo = 360 / resolucion;
    for (aVec2 &v : vertices) {
        v.set(0., radio);
        rotar_centro2(-angulo, aVec2Zero);
    }
}

poligono_t::poligono_t(const poligono_t &other) : vertices(other.vertices) {}

poligono_t &poligono_t::operator=(const poligono_t &other) {
    if (this == &other) return *this;
    this->vertices = other.vertices;
    return *this;
}

// Devuelve la cantidad de vértices de un polígono
size_t poligono_t::cantidad_vertices() const { return vertices.size(); }

// Devuelve true y por interfaz las coordenadas del vértice pos o false si no se
// puede
bool poligono_t::obtener_vertice(size_t pos, float &x, float &y) const {
    if (cantidad_vertices() <= pos) return false;  // raise exception
    x = vertices[pos].x;
    y = vertices[pos].y;
    return true;
}

bool poligono_t::obtener_vertice(size_t pos, aVec2 &v) const {
    if (cantidad_vertices() <= pos) return false;  // raise exception
    v = vertices[pos];
    return true;
}

// Agrega un vértice a un polígono
bool poligono_t::agregar_vertice(aVec2 xy) {
    vertices.emplace_back(xy);
    return true;
}

// Traslada un polígono de n vértices dx en el eje de las abscisas, y dy en le
// eje de las ordenadas.
void poligono_t::trasladar(aVec2 d) {
    for (aVec2 &vertex : vertices) vertex += d;
}

// Rota un polígono de n vértices una rad cantidad de radianes.
void poligono_t::rotar(double rad) {
    for (aVec2 &v : vertices) {
        v.set(v.x * cos(rad) - v.y * sin(rad), v.x * sin(rad) + v.y * cos(rad));
    }
}

void poligono_t::rotar2(double rad) {
    double angulo = rad * PI / 180;
    rotar(angulo);
}

// Rota un poligono respecto a un centro
void poligono_t::rotar_centro(double rad, aVec2 centro) {
    poligono_t::trasladar(-centro);
    poligono_t::rotar(rad);
    poligono_t::trasladar(centro);
}

void poligono_t::rotar_centro2(double rad, aVec2 centro) {
    poligono_t::trasladar(-centro);
    poligono_t::rotar2(rad);
    poligono_t::trasladar(centro);
}

// Determina si un punto de coordenadas px, py está dentro de un triángulo de
// coordenadas A(x,y), B(x,y), C(x,y).
static bool punto_en_triangulo(aVec2 p, aVec2 a, aVec2 b, aVec2 c) {
    //                                         B
    aVec2 pa = a - p; /*           Vector PA   /\ */
    aVec2 pb = b - p; /*           Vector PB  /P.\ */
    aVec2 pc = c - p; /*           Vector PC /____\ */
    //                                      A      C
    return (aCross(pa, pb) <= 0 and aCross(pb, pc) <= 0 and
            aCross(pc, pa) <= 0) or
           (aCross(pc, pa) >= 0 and aCross(pa, pb) >= 0 and
            aCross(pb, pc) >= 0);
}

// Determina si un punto de coordenadas px, py está contenido en un polígono de
// n vértices.
bool poligono_t::punto_dentro(aVec2 p) const {
    for (size_t i = 1; i < vertices.size() - 1; i++)
        if (punto_en_triangulo(p, vertices[0], vertices[i], vertices[i + 1]) ==
            true)
            return true;
    return false;
}

// Imprimir las coordenadas de los vertices de un polígono.
void poligono_t::imprimir() const {
    for (const aVec2 &vertex : vertices)
        std::cout << "  (" << vertex.x << " ; " << vertex.y << ") ";
    std::cout << std::endl;
    // for (size_t i = 0; i < vertices.size(); i++) {
    //     printf("  (%g ; %g) ", vertices[i].x, vertices[i].y);
    // }
    // putchar('\n');
}

// Dibuja un poligono cerrado sobre un SDL_Renderer
bool poligono_t::dibujar(SDL_Renderer *renderer) const {
    if (renderer == NULL) return false;
    for (size_t i = 0; i + 1 < vertices.size(); i++) {
        SDL_RenderDrawLine(renderer, vertices[i].x, vertices[i].y,
                           vertices[i + 1].x, vertices[i + 1].y);
    }
    SDL_RenderDrawLine(renderer, vertices[(vertices.size()) - 1].x,
                       vertices[(vertices.size() - 1)].y, vertices[0].x,
                       vertices[0].y

    );
    return true;
}

// Dibuja un poligono abierto sobre un SDL_Renderer
bool poligono_t::abierto_dibujar(SDL_Renderer *renderer) const {
    if (renderer == NULL) return false;
    for (size_t i = 0; i + 1 < vertices.size(); i++) {
        SDL_RenderDrawLine(renderer, vertices[i].x, vertices[i].y,
                           vertices[i + 1].x, vertices[i + 1].y);
    }
    return true;
}

static aVec2 punto_mas_cercano(aVec2 xy0, aVec2 xy1, aVec2 p) {
    aVec2 a = p - xy0;
    aVec2 b = xy1 - xy0;

    float alfa = aDot(a, b) / aDot(b, b);

    if (alfa <= 0)
        return xy0;
    else if (alfa >= 1)
        return xy1;
    else
        return alfa * b + xy0;
}

// Devuelve por interfaz el punto del polígono más cercano al punto (xp;yp)
double poligono_t::distancia(aVec2 punto, aVec2 &norma) const {
    double d = 1 / 0.0;
    size_t idx = 0;
    size_t cant = cantidad_vertices();

    for (size_t i = 0; i < vertices.size(); i++) {
        aVec2 xyi =
            punto_mas_cercano(vertices[i], vertices[(i + 1) % cant], punto);
        double di = aDistance(xyi, punto);
        if (di < d) {
            d = di;
            idx = i;
        }
    }

    norma.set(vertices[(idx + 1) % cant].y - vertices[idx].y,
              vertices[idx].x - vertices[(idx + 1) % cant].x);
    // n /= aDistance(n, aVec2Zero);
    norma.Normalize();
    return d;
}
