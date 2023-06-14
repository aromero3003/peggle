#include "poligono.h"

#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include <cmath>
#include <vector>

#include "config.h"
#include "vec2.h"

// Crea un polígono de n vertices en R2
poligono_t::poligono_t(float vertices[][2], size_t n) : vertices(n) {
    for (size_t i = 0; i < n; i++) {
        this->vertices[i].x = vertices[i][0];
        this->vertices[i].y = vertices[i][1];
    }
}

poligono_t::poligono_t(const std::vector<aVec2> &vertices)
    : vertices(vertices) {}

poligono_t::poligono_t(float radio, int resolucion) {
    int angulo = 360 / resolucion;
    for (size_t i = 0; i < resolucion; i++) {
        vertices.emplace_back(0, radio);
        this->rotar_centro(-angulo * M_PI / 180, 0, 0);
    }
}

poligono_t::poligono_t(const poligono_t &other) : vertices(other.vertices) {}

poligono_t &poligono_t::operator=(const poligono_t &other) {
    if (this == &other) return *this;
    this->vertices = other.vertices;
    return *this;
}

// Crea un polígono de n vertices en R2
poligono_t *poligono_t::crear(float vertices[][2], size_t n) {
    return new poligono_t(vertices, n);
}

// Crea un polígono de n vertices en R2
poligono_t *poligono_t::crear(const std::vector<aVec2> &vertices) {
    return new poligono_t(vertices);
}

// Crea un poligono circular de 8 vertices de resolucion con centro en el
// origen;
poligono_t *poligono_t::crear_circular(float radio, int resolucion) {
    return new poligono_t(radio, resolucion);
}

// Devuelve la cantidad de vértices de un polígono
size_t poligono_t::cantidad_vertices() const { return vertices.size(); }

// Devuelve true y por interfaz las coordenadas del vértice pos o false si no se
// puede
bool poligono_t::obtener_vertice(size_t pos, float *x, float *y) const {
    if (cantidad_vertices() <= pos) return false;  // raise exception
    *x = vertices[pos].x;
    *y = vertices[pos].y;
    return true;
}

// Agrega un vértice a un polígono
bool poligono_t::agregar_vertice(float x, float y) {
    vertices.emplace_back(x, y);
    return true;
}

// Traslada un polígono de n vértices dx en el eje de las abscisas, y dy en le
// eje de las ordenadas.
void poligono_t::trasladar(float dx, float dy) {
    for (size_t i = 0; i < vertices.size(); i++) {
        vertices[i].x += dx;
        vertices[i].y += dy;
    }
}

// Rota un polígono de n vértices una rad cantidad de radianes.
void poligono_t::rotar(double rad) {
    for (size_t i = 0; i < vertices.size(); i++) {
        float x = vertices[i].x;
        float y = vertices[i].y;
        vertices[i].x = x * cos(rad) - y * sin(rad);
        vertices[i].y = x * sin(rad) + y * cos(rad);
        /*
        float x = poligono->vertices[i][0];
        poligono->vertices[i][0] = poligono->vertices[i][0] * cos(rad) -
        poligono->vertices[i][1] * sin(rad); poligono->vertices[i][1] = x
        * sin(rad) + poligono->vertices[i][1] * cos(rad) ;
        */
    }
}

void poligono_t::rotar2(double rad) {
    double angulo = rad * PI / 180;
    for (size_t i = 0; i < vertices.size(); i++) {
        float x = vertices[i].x;
        float y = vertices[i].y;
        vertices[i].x = x * cos(angulo) - y * sin(angulo);
        vertices[i].y = x * sin(angulo) + y * cos(angulo);
        /*
        float x = poligono->vertices[i][0];
        poligono->vertices[i][0] = poligono->vertices[i][0] * cos(rad) -
        poligono->vertices[i][1] * sin(rad); poligono->vertices[i][1] = x
        * sin(rad) + poligono->vertices[i][1] * cos(rad) ;
        */
    }
}

// Rota un poligono respecto a un centro
void poligono_t::rotar_centro(double rad, float centro_x, float centro_y) {
    poligono_t::trasladar(-centro_x, -centro_y);
    poligono_t::rotar(rad);
    poligono_t::trasladar(centro_x, centro_y);
}

void poligono_t::rotar_centro2(double rad, float centro_x, float centro_y) {
    poligono_t::trasladar(-centro_x, -centro_y);
    poligono_t::rotar2(rad);
    poligono_t::trasladar(centro_x, centro_y);
}

// Calcula el producto vectorial de dos vectores (FUNCION AUXILIAR)
static float producto_vectorial(float vector1[2], float vector2[2]) {
    return vector1[0] * vector2[1] - vector1[1] * vector2[0];
}

// Determina si un punto de coordenadas px, py está dentro de un triángulo de
// coordenadas ax, ay, bx ,by, cx, cy.
static bool punto_en_triangulo(float px, float py, float ax, float ay, float bx,
                               float by, float cx, float cy) {
    //                                                              B
    float pa[2] = {ax - px, ay - py}; /*           Vector PA /\ */
    float pb[2] = {bx - px, by - py}; /*           Vector PB /P.\ */
    float pc[2] = {cx - px, cy - py}; /*           Vector PC /____\ */
    //                                                          A      C
    return (producto_vectorial(pa, pb) <= 0 &&
            producto_vectorial(pb, pc) <= 0 &&
            producto_vectorial(pc, pa) <= 0) ||
           (producto_vectorial(pa, pb) >= 0 &&
            producto_vectorial(pb, pc) >= 0 && producto_vectorial(pc, pa) >= 0);
}

// Determina si un punto de coordenadas px, py está contenido en un polígono de
// n vértices.
bool poligono_t::punto_dentro(float px, float py) const {
    for (size_t i = 1; i < vertices.size() - 1; i++) {
        if (punto_en_triangulo(px, py, vertices[0].x, vertices[0].y,
                               vertices[i].x, vertices[i].y, vertices[i + 1].x,
                               vertices[i + 1].y) == true) {
            return true;
        }
    }
    return false;
}

// Imprimir las coordenadas de los vertices de un polígono.
void poligono_t::imprimir() const {
    for (size_t i = 0; i < vertices.size(); i++) {
        printf("  (%g ; %g) ", vertices[i].x, vertices[i].y);
    }
    putchar('\n');
}

// Dibuja un poligono cerrado sobre un SDL_Renderer
bool poligono_t::dibujar(SDL_Renderer *renderer) {
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
bool poligono_t::abierto_dibujar(SDL_Renderer *renderer) {
    if (renderer == NULL) return false;
    for (size_t i = 0; i + 1 < vertices.size(); i++) {
        SDL_RenderDrawLine(renderer, vertices[i].x, vertices[i].y,
                           vertices[i + 1].x, vertices[i + 1].y);
    }
    return true;
}

static float producto_interno(float ax, float ay, float bx, float by) {
    return ax * bx + ay * by;
}

static double distancia(float ax, float ay, float bx, float by) {
    return sqrt(pow(bx - ax, 2) + pow(by - ay, 2));
}

static void punto_mas_cercano(float x0, float y0, float x1, float y1, float xp,
                              float yp, float *x, float *y) {
    float ax = xp - x0;
    float ay = yp - y0;
    float bx = x1 - x0;
    float by = y1 - y0;

    float alfa =
        producto_interno(ax, ay, bx, by) / producto_interno(bx, by, bx, by);

    if (alfa <= 0) {
        *x = x0;
        *y = y0;
    } else if (alfa >= 1) {
        *x = x1;
        *y = y1;
    } else {
        *x = alfa * bx + x0;
        *y = alfa * by + y0;
    }
}

// Devuelve por interfaz el punto del polígono más cercano al punto (xp;yp)
double poligono_t::distancia(float xp, float yp, float *nor_x,
                             float *nor_y) const {
    double d = 1 / 0.0;
    size_t idx = 0;

    for (size_t i = 0; i < vertices.size(); i++) {
        float xi, yi;
        punto_mas_cercano(
            vertices[i].x, vertices[i].y, vertices[(i + 1) % vertices.size()].x,
            vertices[(i + 1) % vertices.size()].y, xp, yp, &xi, &yi);
        double di = ::distancia(xp, yp, xi, yi);

        if (di < d) {
            d = di;
            idx = i;
        }
    }

    float nx = vertices[(idx + 1) % vertices.size()].y - vertices[idx].y;
    float ny = vertices[idx].x - vertices[(idx + 1) % vertices.size()].x;
    float dn = ::distancia(nx, ny, 0, 0);

    nx /= dn;
    ny /= dn;

    *nor_x = nx;
    *nor_y = ny;

    return d;
}
