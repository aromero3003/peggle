#ifndef __TYPES_H__
#define __TYPES_H__

typedef enum { COLOR_AZUL, COLOR_NARANJA, COLOR_VERDE, COLOR_GRIS } color_t;
typedef enum { INMOVIL, CIRCULAR, HORIZONTAL } movimiento_t;
typedef enum { CIRCULO, RECTANGULO, POLIGONO } geometria_t;

#define SIZE16B (sizeof(int16_t))

#endif /* __TYPES_H__ */
