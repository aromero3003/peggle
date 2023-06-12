#ifndef LECTURA_H
#define LECTURA_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "poligono.h"
#include "tipos.h"

/*
#ifndef COLORES
#define COLORES

const char *colores[] = {
    [COLOR_AZUL] = "Azul",
        [COLOR_NARANJA] = "Naranja",
        [COLOR_VERDE] = "Verde",
        [COLOR_GRIS] = "Gris"
};

#endif
*/

bool leer_cantidad_obstaculos(FILE *f, int16_t *cantidad);

bool leer_encabezado(FILE *f, color_t *color, movimiento_t *movimiento,
                     geometria_t *geometria);

bool leer_movimiento_inmovil(FILE *f, int16_t parametros[]);
bool leer_movimiento_circular(FILE *f, int16_t parametros[]);
bool leer_movimiento_horizontal(FILE *f, int16_t parametros[]);

bool leer_movimiento(FILE *f, movimiento_t movimiento, int16_t parametros[]);

poligono_t *leer_geometria_circulo(FILE *f);
poligono_t *leer_geometria_rectangulo(FILE *f);
poligono_t *leer_geometria_poligono(FILE *f);

poligono_t *leer_geometria(FILE *f, geometria_t geometria);

#endif
