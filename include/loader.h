#ifndef __LOADER_H__
#define __LOADER_H__

#include <cstddef>
#include <fstream>
#include <list>

#include "level.h"
#include "obstacle.h"
#include "types.h"
class Loader {
   public:
    Loader(const char *path);
    Loader(Loader &&) = default;
    Loader(const Loader &) = delete;
    Loader &operator=(Loader &&) = default;
    Loader &operator=(const Loader &) = delete;
    ~Loader();

    bool can_continue();
    Level read_level();

    size_t leer_cantidad_de_obstaculos();
    Obstacle leer_obstaculo();
    std::list<Obstacle> leer_obstaculos(size_t n);

    void leer_encabezado(color_t &col, movimiento_t &mov, geometria_t &geo);

    bool leer_movimiento(movimiento_t movimiento, int16_t parametros[]);

    Polygon leer_geometria(geometria_t geometria);

   private:
    std::ifstream file;

    // Tabla de funciones de leer movimiento
    // bool (*funciones_leer_movimiento[3])(int16_t parametros[]) = {
    //     [INMOVIL] = leer_movimiento_inmovil,
    //     [CIRCULAR] = leer_movimiento_circular,
    //     [HORIZONTAL] = leer_movimiento_horizontal};

    bool leer_movimiento_inmovil(int16_t parametros[]);
    bool leer_movimiento_circular(int16_t parametros[]);
    bool leer_movimiento_horizontal(int16_t parametros[]);

    Polygon leer_geometria_circulo();
    Polygon leer_geometria_rectangulo();
    Polygon leer_geometria_poligono();
};

#endif /* __LOADER_H__ */
