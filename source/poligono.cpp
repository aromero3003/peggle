#include "poligono.h"

#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include <vector>

#include "config.h"
#include "vec2.h"

// Crea un polígono de n vertices en R2
poligono_t *poligono_t::crear(float vertices[][2], size_t n) {
    poligono_t *poligono = new poligono_t;

    poligono->vertices.resize(n);

    for (size_t i = 0; i < n; i++) {
        poligono->vertices[i].x = vertices[i][0];
        poligono->vertices[i].y = vertices[i][1];
    }

    return poligono;
}

// Crea un polígono de n vertices en R2
poligono_t *poligono_t::crear(const std::vector<aVec2> &vertices) {
    poligono_t *poligono = new poligono_t;
    if (poligono == NULL) return NULL;

    poligono->vertices = vertices;

    return poligono;
}

// Crea un poligono circular de 8 vertices de resolucion con centro en el
// origen;
poligono_t *poligono_t::crear_circular(float radio, int resolucion) {
    poligono_t *circulo = poligono_t::crear(NULL, 0);
    if (circulo == NULL) return NULL;

    int angulo = 360 / resolucion;

    for (size_t i = 0; i < resolucion; i++) {
        if (!poligono_t::agregar_vertice(circulo, 0, radio)) {
            poligono_t::destruir(circulo);
            return NULL;
        }
        poligono_t::rotar_centro(circulo, -angulo * PI / 180, 0, 0);
    }

    return circulo;
}

// Destruye un poligono
void poligono_t::destruir(poligono_t *poligono) { delete poligono; }

// Devuelve la cantidad de vértices de un polígono
size_t poligono_t::cantidad_vertices(const poligono_t *poligono) const {
    return poligono->vertices.size();
}

// Devuelve true y por interfaz las coordenadas del vértice pos o false si no se
// puede
bool poligono_t::obtener_vertice(const poligono_t *poligono, size_t pos,
                                 float *x, float *y) const {
    if (poligono == NULL || poligono->cantidad_vertices(poligono) <= pos)
        return false;
    *x = poligono->vertices[pos].x;
    *y = poligono->vertices[pos].y;
    return true;
}

// Clona un polígono
poligono_t *poligono_t::clonar(const poligono_t *poligono) {
    poligono_t *poligono2 = poligono_t::crear(poligono->vertices);
    return poligono2;
}

// Agrega un vértice a un polígono
bool poligono_t::agregar_vertice(poligono_t *poligono, float x, float y) {
    if (poligono == NULL) return false;

    poligono->vertices.emplace_back(x, y);
    return true;
}

// Traslada un polígono de n vértices dx en el eje de las abscisas, y dy en le
// eje de las ordenadas.
void poligono_t::trasladar(poligono_t *poligono, float dx, float dy) {
    for (size_t i = 0; i < poligono->vertices.size(); i++) {
        poligono->vertices[i].x += dx;
        poligono->vertices[i].y += dy;
    }
}

// Rota un polígono de n vértices una rad cantidad de radianes.
void poligono_t::rotar(poligono_t *poligono, double rad) {
    for (size_t i = 0; i < poligono->vertices.size(); i++) {
        float x = poligono->vertices[i].x;
        float y = poligono->vertices[i].y;
        poligono->vertices[i].x = x * cos(rad) - y * sin(rad);
        poligono->vertices[i].y = x * sin(rad) + y * cos(rad);
        /*
        float x = poligono->vertices[i][0];
        poligono->vertices[i][0] = poligono->vertices[i][0] * cos(rad) -
        poligono->vertices[i][1] * sin(rad); poligono->vertices[i][1] = x
        * sin(rad) + poligono->vertices[i][1] * cos(rad) ;
        */
    }
}

void poligono_t::rotar2(poligono_t *poligono, double rad) {
    double angulo = rad * PI / 180;
    for (size_t i = 0; i < poligono->vertices.size(); i++) {
        float x = poligono->vertices[i].x;
        float y = poligono->vertices[i].y;
        poligono->vertices[i].x = x * cos(angulo) - y * sin(angulo);
        poligono->vertices[i].y = x * sin(angulo) + y * cos(angulo);
        /*
        float x = poligono->vertices[i][0];
        poligono->vertices[i][0] = poligono->vertices[i][0] * cos(rad) -
        poligono->vertices[i][1] * sin(rad); poligono->vertices[i][1] = x
        * sin(rad) + poligono->vertices[i][1] * cos(rad) ;
        */
    }
}

// Rota un poligono respecto a un centro
void poligono_t::rotar_centro(poligono_t *poligono, double rad, float centro_x,
                              float centro_y) {
    poligono_t::trasladar(poligono, -centro_x, -centro_y);
    poligono_t::rotar(poligono, rad);
    poligono_t::trasladar(poligono, centro_x, centro_y);
}

void poligono_t::rotar_centro2(poligono_t *poligono, double rad, float centro_x,
                               float centro_y) {
    poligono_t::trasladar(poligono, -centro_x, -centro_y);
    poligono_t::rotar2(poligono, rad);
    poligono_t::trasladar(poligono, centro_x, centro_y);
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
bool poligono_t::punto_dentro(poligono_t *poligono, float px, float py) const {
    for (size_t i = 1; i < poligono->vertices.size() - 1; i++) {
        if (punto_en_triangulo(px, py, poligono->vertices[0].x,
                               poligono->vertices[0].y, poligono->vertices[i].x,
                               poligono->vertices[i].y,
                               poligono->vertices[i + 1].x,
                               poligono->vertices[i + 1].y) == true) {
            return true;
        }
    }
    return false;
}

// Imprimir las coordenadas de los vertices de un polígono.
void poligono_t::imprimir(poligono_t *poligono) const {
    for (size_t i = 0; i < poligono->vertices.size(); i++) {
        printf("  (%g ; %g) ", poligono->vertices[i].x,
               poligono->vertices[i].y);
    }
    putchar('\n');
}

// Dibuja un poligono cerrado sobre un SDL_Renderer
bool poligono_t::dibujar(SDL_Renderer *renderer, poligono_t *poligono) {
    if (poligono == NULL || renderer == NULL) return false;
    for (size_t i = 0; i + 1 < poligono->vertices.size(); i++) {
        SDL_RenderDrawLine(renderer, poligono->vertices[i].x,
                           poligono->vertices[i].y, poligono->vertices[i + 1].x,
                           poligono->vertices[i + 1].y);
    }
    SDL_RenderDrawLine(renderer,
                       poligono->vertices[(poligono->vertices.size()) - 1].x,
                       poligono->vertices[(poligono->vertices.size() - 1)].y,
                       poligono->vertices[0].x, poligono->vertices[0].y

    );
    return true;
}

// Dibuja un poligono abierto sobre un SDL_Renderer
bool poligono_t::abierto_dibujar(SDL_Renderer *renderer, poligono_t *poligono) {
    if (poligono == NULL || renderer == NULL) return false;
    for (size_t i = 0; i + 1 < poligono->vertices.size(); i++) {
        SDL_RenderDrawLine(renderer, poligono->vertices[i].x,
                           poligono->vertices[i].y, poligono->vertices[i + 1].x,
                           poligono->vertices[i + 1].y);
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
double poligono_t::distancia(const poligono_t *p, float xp, float yp,
                             float *nor_x, float *nor_y) const {
    double d = 1 / 0.0;
    size_t idx = 0;

    for (size_t i = 0; i < p->vertices.size(); i++) {
        float xi, yi;
        punto_mas_cercano(p->vertices[i].x, p->vertices[i].y,
                          p->vertices[(i + 1) % p->vertices.size()].x,
                          p->vertices[(i + 1) % p->vertices.size()].y, xp, yp,
                          &xi, &yi);
        double di = ::distancia(xp, yp, xi, yi);

        if (di < d) {
            d = di;
            idx = i;
        }
    }

    float nx =
        p->vertices[(idx + 1) % p->vertices.size()].y - p->vertices[idx].y;
    float ny =
        p->vertices[idx].x - p->vertices[(idx + 1) % p->vertices.size()].x;
    float dn = ::distancia(nx, ny, 0, 0);

    nx /= dn;
    ny /= dn;

    *nor_x = nx;
    *nor_y = ny;

    return d;
}
