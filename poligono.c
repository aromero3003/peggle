#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include "poligono.h"
#include "config.h"

struct poligono{
    float (*vertices)[2];
    size_t n;
};

// Crea un polígono de n vertices en R2
poligono_t *poligono_crear(float vertices[][2], size_t n) {
    
    poligono_t *poligono = (poligono_t *)malloc( sizeof(poligono_t) );
    if (poligono == NULL) return NULL;
    
    poligono->vertices = (float(*)[2]) malloc( n * sizeof(float[2]) );
    if (poligono->vertices == NULL) {
        free(poligono);
        return NULL;
    }
    
    poligono->n = n;

    for (size_t i = 0; i < n; i++){
        poligono->vertices[i][0] = vertices[i][0];
        poligono->vertices[i][1] = vertices[i][1];
    }
    
    return poligono;
}


// Crea un poligono circular de 8 vertices de resolucion con centro en el origen;
poligono_t *poligono_crear_circular(float radio, int resolucion) {
    poligono_t *circulo = poligono_crear(NULL, 0); 
    if(circulo == NULL) return NULL;

    int angulo = 360 / resolucion;

    for (size_t i = 0; i < resolucion ; i++) {
        if(! poligono_agregar_vertice(circulo, 0, radio)) {
            poligono_destruir(circulo);
            return NULL;
        }
        poligono_rotar_centro(circulo, -angulo * PI / 180, 0, 0);
    }

    return circulo;    
}


// Destruye un poligono
void poligono_destruir(poligono_t *poligono) {
    free(poligono->vertices);
    free(poligono);
}

// Devuelve la cantidad de vértices de un polígono
size_t poligono_cantidad_vertices(const poligono_t *poligono) {
    return poligono->n;
}

// Devuelve true y por interfaz las coordenadas del vértice pos o false si no se puede
bool poligono_obtener_vertice(const poligono_t *poligono, size_t pos, float *x, float *y) {

    if (poligono == NULL  ||  poligono->n <= pos  ||  poligono->vertices == NULL) return false;
    *x = poligono->vertices[pos][0];
    *y = poligono->vertices[pos][1];
    return true;
}

// Clona un polígono
poligono_t *poligono_clonar(const poligono_t *poligono) {
    
   poligono_t *poligono2 = poligono_crear(poligono->vertices, poligono->n);
    return poligono2;
}

// Agrega un vértice a un polígono
bool poligono_agregar_vertice(poligono_t *poligono, float x, float y) {
    if (poligono == NULL) return false;
    if (poligono->vertices == NULL) return false;

    float (*aux)[2] = (float(*)[2])realloc(poligono->vertices, (poligono->n + 1) * sizeof(float[2]));
    if (aux == NULL) return false;

    poligono->vertices = aux;
    poligono->vertices[poligono->n][0] = x;
    poligono->vertices[poligono->n][1] = y;
    poligono->n++;

    return true;
}

// Traslada un polígono de n vértices dx en el eje de las abscisas, y dy en le eje de las ordenadas.
void poligono_trasladar(poligono_t *poligono, float dx, float dy) {
    for (size_t i = 0; i < poligono->n; i++) {
        poligono->vertices[i][0] += dx; 
        poligono->vertices[i][1] += dy; 
    }
}


// Rota un polígono de n vértices una rad cantidad de radianes.
void poligono_rotar(poligono_t *poligono, double rad) {
    for (size_t i = 0; i < poligono->n; i++) {
        float x = poligono->vertices[i][0];
        float y = poligono->vertices[i][1];
        poligono->vertices[i][0] = x * cos(rad) - y * sin(rad);
        poligono->vertices[i][1] = x * sin(rad) + y * cos(rad);
        /*
        float x = poligono->vertices[i][0]; 
        poligono->vertices[i][0] = poligono->vertices[i][0] * cos(rad) - poligono->vertices[i][1] * sin(rad);
        poligono->vertices[i][1] =             x            * sin(rad) + poligono->vertices[i][1] * cos(rad) ; 
        */
    }
}

void poligono_rotar2(poligono_t *poligono, double rad) {
    double angulo = rad * PI / 180;
    for (size_t i = 0; i < poligono->n; i++) {
        float x = poligono->vertices[i][0];
        float y = poligono->vertices[i][1];
        poligono->vertices[i][0] = x * cos(angulo) - y * sin(angulo);
        poligono->vertices[i][1] = x * sin(angulo) + y * cos(angulo);
        /*
        float x = poligono->vertices[i][0]; 
        poligono->vertices[i][0] = poligono->vertices[i][0] * cos(rad) - poligono->vertices[i][1] * sin(rad);
        poligono->vertices[i][1] =             x            * sin(rad) + poligono->vertices[i][1] * cos(rad) ; 
        */
    }
}

// Rota un poligono respecto a un centro
void poligono_rotar_centro(poligono_t *poligono, double rad, float centro_x, float centro_y) {
    poligono_trasladar(poligono, -centro_x, -centro_y);
    poligono_rotar(poligono, rad);
    poligono_trasladar(poligono, centro_x, centro_y);
}

void poligono_rotar_centro2(poligono_t *poligono, double rad, float centro_x, float centro_y) {
    poligono_trasladar(poligono, -centro_x, -centro_y);
    poligono_rotar2(poligono, rad);
    poligono_trasladar(poligono, centro_x, centro_y);
}


// Calcula el producto vectorial de dos vectores (FUNCION AUXILIAR)
static float producto_vectorial(float vector1[2], float vector2[2]) {
    return vector1[0] * vector2[1] - vector1[1] * vector2[0];
}

// Determina si un punto de coordenadas px, py está dentro de un triángulo de coordenadas ax, ay, bx ,by, cx, cy. 
static bool punto_en_triangulo(float px, float py, float ax, float ay, float bx, float by, float cx, float cy) {
    //                                                              B
    float pa[2] = {ax - px, ay - py};   /*           Vector PA     /\                                   */  
    float pb[2] = {bx - px, by - py};   /*           Vector PB    /P.\                                  */
    float pc[2] = {cx - px, cy - py};   /*           Vector PC   /____\                                 */
    //                                                          A      C
    return (producto_vectorial(pa , pb) <= 0 && producto_vectorial(pb, pc) <= 0 && producto_vectorial(pc, pa) <= 0) || (producto_vectorial(pa , pb) >= 0 && producto_vectorial(pb , pc) >= 0 && producto_vectorial(pc , pa) >= 0);
}

// Determina si un punto de coordenadas px, py está contenido en un polígono de n vértices.
bool poligono_punto_dentro(poligono_t *poligono, float px, float py){
    for (size_t i = 1; i < poligono->n - 1; i++) {
        if (punto_en_triangulo(
            px, py,
            poligono->vertices[0][0], poligono->vertices[0][1], 
            poligono->vertices[i][0], poligono->vertices[i][1], 
            poligono->vertices[i+1][0], poligono->vertices[i+1][1]
        ) == true) {
            return true;
        }
    }
    return false;
}


// Imprimir las coordenadas de los vertices de un polígono.
void imprimir(poligono_t *poligono) {
    for (size_t i = 0; i < poligono->n; i++) {
        printf ("  (%g ; %g) ", poligono->vertices[i][0], poligono->vertices[i][1]);
    }
    putchar('\n');   
}


// Dibuja un poligono cerrado sobre un SDL_Renderer
bool poligono_dibujar(SDL_Renderer *renderer, poligono_t *poligono) {
    if(poligono == NULL || renderer == NULL) return false;
    for(size_t i = 0 ; i + 1 < poligono->n ; i++) {
        SDL_RenderDrawLine(renderer,
        poligono->vertices[i][0], poligono->vertices[i][1],
        poligono->vertices[i + 1][0], poligono->vertices[i + 1][1]
        );
    }
    SDL_RenderDrawLine(renderer,
        poligono->vertices[(poligono->n) - 1][0], poligono->vertices[(poligono->n - 1)][1],
        poligono->vertices[0][0], poligono->vertices[0][1]

    );
    return true;
}

// Dibuja un poligono abierto sobre un SDL_Renderer
bool poligono_abierto_dibujar(SDL_Renderer *renderer, poligono_t *poligono) {
    if(poligono == NULL || renderer == NULL) return false;
    for(size_t i = 0 ; i + 1< poligono->n ; i++) {
        SDL_RenderDrawLine(renderer,
        poligono->vertices[i][0], poligono->vertices[i][1],
        poligono->vertices[i + 1][0], poligono->vertices[i + 1][1]
        );
    }
    return true;
}


static float producto_interno(float ax, float ay, float bx, float by) {
    return ax * bx + ay * by;
}

static double distancia(float ax, float ay, float bx, float by) {
    return sqrt(pow(bx - ax, 2) + pow(by - ay, 2));
}

static void punto_mas_cercano(float x0, float y0, float x1, float y1, float xp, float yp, float *x, float *y) {
    float ax = xp - x0;
    float ay = yp - y0;
    float bx = x1 - x0;
    float by = y1 - y0;

    float alfa = producto_interno(ax, ay, bx, by) / producto_interno(bx, by, bx, by);

    if(alfa <= 0) {
        *x = x0;
        *y = y0;
    }
    else if(alfa >= 1) {
        *x = x1;
        *y = y1;
    }
    else {
        *x = alfa * bx + x0;
        *y = alfa * by + y0;
    }
}


// Devuelve por interfaz el punto del polígono más cercano al punto (xp;yp)
double poligono_distancia(const poligono_t *p, float xp, float yp, float *nor_x, float *nor_y) {
    double d = 1 / 0.0;
    size_t idx = 0;

    for(size_t i = 0; i < p->n; i++) {
        float xi, yi;
        punto_mas_cercano(p->vertices[i][0], p->vertices[i][1], p->vertices[(i + 1) % p->n][0], p->vertices[(i + 1) % p->n][1], xp, yp, &xi, &yi);
        double di = distancia(xp, yp, xi, yi);

        if(di < d) {
            d = di;
            idx = i;
        }
    }

    float nx = p->vertices[(idx + 1) % p->n][1] - p->vertices[idx][1];
    float ny = p->vertices[idx][0] - p->vertices[(idx + 1) % p->n][0];
    float dn = distancia(nx, ny, 0, 0);

    nx /= dn;
    ny /= dn;

    *nor_x = nx;
    *nor_y = ny;

    return d;
}

