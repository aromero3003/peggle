#include "loader.h"

#include <cstdint>
#include <ios>
#include <iostream>
#include <list>

#include "level.h"
#include "obstacle.h"
#include "types.h"
#include "vec2.h"

#define MASK_COLOR 0x3  // 0011
#define MASK_MOVE 0x3   // 0011
#define MASK_FORM 0xf   // 1111

#define SHIFT_COLOR 6
#define SHIFT_MOVE 4

#define RESOL_BOLA_OBS 30  // los obstaculos circulares

Loader::Loader(const char *path) : file(path, std::ios::binary) {
    if (not file.is_open()) {
        std::cerr << "Error opening " << path << std::endl;
        throw -1;  // TODO añadir excepcion
    }
}

bool Loader::can_continue() { return not file.eof(); }

Level Loader::read_level() {
    std::list<Obstacle> obs = leer_obstaculos(leer_cantidad_de_obstaculos());
    // return Level(leer_obstaculos(leer_cantidad_de_obstaculos()));
    return Level(obs);
}

size_t Loader::leer_cantidad_de_obstaculos() {
    uint16_t buffer = 0;
    file.read(reinterpret_cast<char *>(&buffer), sizeof(buffer));
    if (file.fail()) return 0;
    return buffer;
}

Obstacle Loader::leer_obstaculo() {
    color_t col;
    movimiento_t mov;
    geometria_t geo;

    // if (leer_encabezado(col, mov, geo)) {
    leer_encabezado(col, mov, geo);
    // fprintf(stderr, "No se pudo leer\n");
    // }

    // printf("%d, %d, %d\n",color, mov, geo);

    float parametros[3] = {0};
    int16_t param[3] = {0};

    leer_movimiento(mov, param);

    parametros[0] = param[0];
    parametros[1] = param[1];
    parametros[2] = param[2];

    // std::cout << parametros[0] << " " << parametros[1] << " " <<
    // parametros[2]
    //           << std::endl;
    return Obstacle(leer_geometria(geo), col, mov, geo, parametros);
}

std::list<Obstacle> Loader::leer_obstaculos(size_t n) {
    std::list<Obstacle> obstaculos;
    for (size_t i = 0; i < n; i++) obstaculos.emplace_back(leer_obstaculo());
    return obstaculos;
}

void Loader::leer_encabezado(color_t &col, movimiento_t &mov,
                             geometria_t &geo) {
    uint8_t encabezado;
    file.readsome((char *)&encabezado, 1);

    col = (color_t)((encabezado >> SHIFT_COLOR) & MASK_COLOR);
    mov = (movimiento_t)((encabezado >> SHIFT_MOVE) & MASK_MOVE);
    geo = (geometria_t)(encabezado & MASK_FORM);
}

bool Loader::leer_movimiento_inmovil(int16_t parametros[]) {
    parametros[0] = parametros[1] = parametros[2] = 0;
    return true;
}

bool Loader::leer_movimiento_circular(int16_t parametros[]) {
    return (bool)file.read(reinterpret_cast<char *>(parametros), SIZE16B * 3);
}

bool Loader::leer_movimiento_horizontal(int16_t parametros[]) {
    return (bool)file.read(reinterpret_cast<char *>(parametros), SIZE16B * 3);
}

// Funcion para usar la tabla de funciones_leer_m
bool Loader::leer_movimiento(movimiento_t movimiento, int16_t parametros[]) {
    // return funciones_leer_movimiento[movimiento](f, parametros);
    if (movimiento == INMOVIL)
        return leer_movimiento_inmovil(parametros);
    else if (movimiento == CIRCULAR)
        return leer_movimiento_circular(parametros);
    else
        return leer_movimiento_horizontal(parametros);
}

// Funciones para leer geometría
// -------------------------------------------------------------------

Polygon Loader::leer_geometria_circulo() {
    int16_t x, y, r;
    if (not((bool)file.read(reinterpret_cast<char *>(&x), SIZE16B) and
            (bool) file.read(reinterpret_cast<char *>(&y), SIZE16B) and
            (bool) file.read(reinterpret_cast<char *>(&r), SIZE16B)))
        throw -1;  // TODO Crear Excepcion

    Polygon poligono(r, RESOL_BOLA_OBS);
    poligono.trasladar(aVec2(x, y));
    // printf("CIRCULO: x = %d, y = %d, radio = %d\n",x, y, r);
    return poligono;
}

Polygon Loader::leer_geometria_rectangulo() {
    int16_t x, y, ancho, alto, angulo;
    if (not((bool)file.read(reinterpret_cast<char *>(&x), SIZE16B) and
            (bool) file.read(reinterpret_cast<char *>(&y), SIZE16B) and
            (bool) file.read(reinterpret_cast<char *>(&ancho), SIZE16B) and
            (bool) file.read(reinterpret_cast<char *>(&alto), SIZE16B) and
            (bool) file.read(reinterpret_cast<char *>(&angulo), SIZE16B))) {
        throw -1;
    }

    float vertices[][2] = {{(float)-ancho / 2, (float)-alto / 2},
                           {(float)ancho / 2, (float)-alto / 2},
                           {(float)ancho / 2, (float)alto / 2},
                           {(float)-ancho / 2, (float)alto / 2}};
    Polygon poligono(vertices, 4);

    // printf("angul = %g x: %d y: %d\n",(double)angulo, x, y);
    poligono.rotar2(angulo);
    poligono.trasladar(aVec2(x, y));
    // printf("RECTANGULO: x = %d, y = %d, ancho = %d, alto = %d, angulo =
    // %d\n", x, y, ancho, alto, angulo);
    return poligono;
}

Polygon Loader::leer_geometria_poligono() {
    uint16_t puntos;
    if (not(bool) file.read(reinterpret_cast<char *>(&puntos), SIZE16B))
        throw -1;
    // printf("POLIGONO: puntos = %d\n", puntos);

    float parametros[puntos][2];

    for (size_t i = 0; i < puntos; i++) {
        int16_t x, y;
        if (not((bool)file.read(reinterpret_cast<char *>(&x), SIZE16B) and
                file.read(reinterpret_cast<char *>(&y), SIZE16B)))
            throw -1;
        parametros[i][0] = x;
        parametros[i][1] = y;
    }

    /*for (size_t i = 0; i < puntos; i++) {
        printf("\tx%zd = %g,", i, parametros[i][0]);
        printf(" y%zd = %g\n", i, parametros[i][1]);
    }*/
    return Polygon(parametros, puntos);
}

// Tabla de busqueda de funciones para leer geometria
// Polygon *(*funciones_leer_geometria[])(FILE *f) = {
//     [CIRCULO] = leer_geometria_circulo,
//     [RECTANGULO] = leer_geometria_rectangulo,
//     [POLIGONO] = leer_geometria_poligono};

// Funcion para utilizar la tabla de busqueda
Polygon Loader::leer_geometria(geometria_t geometria) {
    // return funciones_leer_geometria[geometria](f);
    if (geometria == CIRCULO)
        return leer_geometria_circulo();
    else if (geometria == RECTANGULO)
        return leer_geometria_rectangulo();
    else
        return leer_geometria_poligono();
}

Loader::~Loader() { file.close(); }
