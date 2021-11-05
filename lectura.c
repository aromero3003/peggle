#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "poligono.h"
#include "lectura.h"
#include "config.h"
#include "tipos.h"

#define MASK_COLOR 0x3  // 0011
#define MASK_MOVE  0x3  // 0011
#define MASK_FORM  0xf  // 1111

#define SHIFT_COLOR 6
#define SHIFT_MOVE 4




// Funcion para leer la cantidad de obstaculos en un nivel

bool leer_cantidad_obstaculos(FILE *f, int16_t *cantidad) {
    if(f == NULL || cantidad == NULL) return false;
    if(fread(cantidad, SIZE16B, 1, f) != 1) return false;
    return true;
}



// Funcion para leer encabezado--------------------------------------------------------------------------------------

bool leer_encabezado(FILE *f, color_t *color, movimiento_t *movimiento, geometria_t *geometria) {
    
    if(f == NULL) return false;

    uint8_t encabezado;
    if(fread(&encabezado, 1, 1, f) != 1) return false;

    *color = (encabezado >> SHIFT_COLOR) & MASK_COLOR;
    *movimiento = (encabezado >> SHIFT_MOVE) & MASK_MOVE;
    *geometria = encabezado & MASK_FORM;

    return true;
}


 
// Funciones para leer movimiento ---------------------------------------------------------------------------

bool leer_movimiento_inmovil(FILE *f, int16_t parametros[]) {
    return true;
}

bool leer_movimiento_circular(FILE *f, int16_t parametros[]) {
    if(fread(parametros, SIZE16B, 3, f) != 3) return false;
    //printf("mov %d, %d, %d\n",parametros[0], parametros[1], parametros[2] );
    return true;
}

bool leer_movimiento_horizontal(FILE *f, int16_t parametros[]) {
    if(fread(parametros, SIZE16B, 3, f) != 3) return false;
    //printf("mov %d, %d, %d\n",parametros[0], parametros[1], parametros[2] );
    return true;   
}

// Tabla de funciones de leer movimiento
bool (*funciones_leer_movimiento[])(FILE *f, int16_t parametros[]) = {
    [INMOVIL] = leer_movimiento_inmovil,
    [CIRCULAR] = leer_movimiento_circular,
    [HORIZONTAL] = leer_movimiento_horizontal
};

// Funcion para usar la tabla de funciones_leer_m
bool leer_movimiento(FILE *f, movimiento_t movimiento, int16_t parametros[]) {
    if(f == NULL) return false;
    return funciones_leer_movimiento[movimiento](f, parametros);
}






// Funciones para leer geometría -------------------------------------------------------------------

poligono_t *leer_geometria_circulo(FILE *f) {
    int16_t x, y, r;
    if(fread(&x, SIZE16B, 1, f) != 1) return NULL;
    if(fread(&y, SIZE16B, 1, f) != 1) return NULL;
    if(fread(&r, SIZE16B, 1, f) != 1) return NULL;

    float radio = r;
    poligono_t *poligono = poligono_crear_circular(radio, RESOL_BOLA_OBS);
    if(poligono == NULL) return NULL;
    
    poligono_trasladar(poligono, x, y);
    //printf("CIRCULO: x = %d, y = %d, radio = %d\n",x, y, r);
    return poligono;
}

poligono_t *leer_geometria_rectangulo(FILE *f) {
    int16_t x, y, ancho, alto, angulo;
    if(fread(&x, SIZE16B, 1, f) != 1) return NULL;
    if(fread(&y, SIZE16B, 1, f) != 1) return NULL;
    if(fread(&ancho, SIZE16B, 1, f) != 1) return NULL;
    if(fread(&alto, SIZE16B, 1, f) != 1) return NULL;
    if(fread(&angulo, SIZE16B, 1, f) != 1) return NULL;

    //int16_t param[5]

    float vertices[][2] = {{-ancho / 2, -alto / 2 }, {ancho / 2, -alto / 2}, {ancho / 2, alto / 2}, {-ancho / 2, alto / 2}};
    /*for (size_t i = 0; i < 4; i++) {
        printf("%g ", vertices[i][0]);
        printf("%g  ", vertices[i][1]);
    }
    putchar('\n');
    */
    poligono_t *poligono = poligono_crear(vertices, 4);
    if(poligono == NULL) return NULL;

    //printf("angul = %g x: %d y: %d\n",(double)angulo, x, y);
    poligono_rotar2(poligono, angulo); 
    poligono_trasladar(poligono, x, y);
    //printf("RECTANGULO: x = %d, y = %d, ancho = %d, alto = %d, angulo = %d\n", x, y, ancho, alto, angulo);
    return poligono;
}

poligono_t *leer_geometria_poligono(FILE *f) {
    int16_t puntos;
    if(fread(&puntos, SIZE16B, 1, f) != 1) return NULL;
    //printf("POLIGONO: puntos = %d\n", puntos);
    
    int16_t param[puntos][2];
    float parametros[puntos][2];
    
    for (size_t i = 0; i < puntos; i++) {
        if(fread(param + i, SIZE16B, 2, f) !=  2) return NULL;        
        parametros[i][0] = param[i][0];
        parametros[i][1] = param[i][1];
    }
    
    poligono_t *poligono = poligono_crear(parametros, puntos);
    if(poligono == NULL) return NULL;
    
    /*for (size_t i = 0; i < puntos; i++) {
        printf("\tx%zd = %g,", i, parametros[i][0]);
        printf(" y%zd = %g\n", i, parametros[i][1]); 
    }*/
    return poligono;
}

//Tabla de busqueda de funciones para leer geometria
poligono_t *(*funciones_leer_geometria[])(FILE *f) = {
    [CIRCULO] = leer_geometria_circulo,
    [RECTANGULO] = leer_geometria_rectangulo,
    [POLIGONO] = leer_geometria_poligono
};

// Funcion para utilizar la tabla de busqueda
poligono_t *leer_geometria(FILE*f, geometria_t geometria) {
    if(f == NULL) return NULL;
    return funciones_leer_geometria[geometria](f);
}
