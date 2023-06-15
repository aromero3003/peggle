#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#include <iostream>
#include <list>

#include "bola.h"
#include "cannon.h"
#include "config.h"
#include "juego.h"
#include "loader.h"
#include "obstaculo.h"
#include "poligono.h"
#include "recuperador.h"
#include "trayectoria.h"
#include "utility.h"
#include "vec2.h"
#include "vidas.h"

#define DT (1.0 / JUEGO_FPS)

const aVec2 G_VEC(0.0f, G);

void resetear_obstaculos(obstaculo_t &obs) {
    obs.set_tocado(false);
    obs.set_dibujar(true);
}

#define DT (1.0 / JUEGO_FPS)

#ifdef TTF
#include <SDL2/SDL_ttf.h>

// ------------------------------------------------------------- FUNCIONES QUE
// ESCRIBEN TEXTO EN LA PANTALLA
// -------------------------------------------------------------

void escribir_texto(SDL_Renderer *renderer, TTF_Font *font, const char *s,
                    int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    SDL_Color color = {r, g, b};  // Estaría bueno si la función recibe un
                                  // enumerativo con el color, ¿no?
    SDL_Surface *surface = TTF_RenderText_Solid(font, s, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = surface->w;
    rect.h = surface->h;

    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void puntaje_escribir(SDL_Renderer *renderer, TTF_Font *font, puntaje_t puntaje,
                      int x, int y) {
    char p[20];
    sprintf(p, "Puntaje: %zd", puntaje);

    SDL_Color color = {255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, p, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = surface->w * 0.8;
    rect.h = surface->h * 0.8;

    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void puntaje_multiplicador_escribir(SDL_Renderer *renderer, TTF_Font *font,
                                    int multiplicador, int x, int y) {
    char m[4];
    sprintf(m, "x%d", multiplicador);

    SDL_Color color = {255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, m, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = surface->w * 0.8;
    rect.h = surface->h * 0.8;

    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void nivel_escribir(SDL_Renderer *renderer, TTF_Font *font, int nivel, int x,
                    int y) {
    char n[10];
    sprintf(n, "Nivel %d", nivel);

    SDL_Color color = {255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, n, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = surface->w * 0.8;
    rect.h = surface->h * 0.8;

    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void vidas_escribir(SDL_Renderer *renderer, TTF_Font *font, vidas_t *vidas,
                    int x, int y) {
    char v[3];
    size_t aux = vidas->restantes();
    sprintf(v, "%zd", aux);
    escribir_texto(renderer, font, v, x, y, 0xFF, 0xFF, 0xFF);
}

void escribir_numero(SDL_Renderer *renderer, TTF_Font *font, int contador,
                     int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    char c[4];
    sprintf(c, "%ds", contador);
    escribir_texto(renderer, font, c, x, y, r, g, b);
}

#endif

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << "<levels_path.bin" << std::endl;
        return 1;
    }

    SDL_Init(SDL_INIT_VIDEO);

#ifdef TTF
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("../resources/FreeSansBold.ttf", 24);
#endif

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    SDL_CreateWindowAndRenderer(VENTANA_ANCHO, VENTANA_ALTO, 0, &window,
                                &renderer);
    SDL_SetWindowTitle(window, "Peggle");

    int dormir = 0;

    Cannon canon(aVec2(CANON_X, CANON_Y));  // Ángulo del cañón
    bool cayendo = false;                   // ¿Hay bola disparada?

    aVec2 c;  // Centro de la bola
    aVec2 v;  // Velocidad de la bola

    aVec2 v_estancada;  // Estos 2 parametros para calcular el modulo de
    aVec2 p_estancada;  // la posición y empezar un contador para
    //                           determinar si la bola quedó estancada;

    bool bola_recuperada =
        false;  // Para saber si la bola entró al recuperador de bolas

    size_t contador_trayectoria =
        20;  // contador para actualizar la trayectoria en intervalos de 5

    puntaje_t puntaje_total = 0;

    trayectoria_t tray;

    Loader loader(argv[1]);

    int16_t cant_obstaculos;
    int nivel = 0;

    vidas_t vidas(VIDAS_INICIALES, 60, MIN_Y + BOLA_RADIO);
    // if (vidas == NULL) {
    //     fclose(f);
    //     SDL_DestroyRenderer(renderer);
    //     SDL_DestroyWindow(window);
    //     return 3;
    // }
    // recuperador_t *recuperador = recuperador_crear(60, 10, 0.6);
    recuperador_t recuperador(60, 10, 0.6);
    // if (recuperador == NULL) {
    //     fclose(f);
    //     SDL_DestroyRenderer(renderer);
    //     SDL_DestroyWindow(window);
    //     return 3;
    // }

    game_state_t estado = GAME_RUNNING;

    while (estado) {
        cant_obstaculos = loader.leer_cantidad_de_obstaculos();
        if (cant_obstaculos == 0) {
            // Si no pude leer más obstaculos, GAME OVER
            estado = GAME_OVER;
        }

        size_t cantidad_naranjas = 0, naranjas_golpeados = 0;
        puntaje_t puntaje_en_nivel = 0;
        int multiplicador = 1;
        nivel++;

        vidas.resetear();  // Se setean las vidas a la cantidad inicial

        std::list<obstaculo_t> obstaculos;

        for (size_t i = 0; i < cant_obstaculos;
             i++) {  // Se levantan todos los obstáculos del nivel
            obstaculo_t nuevo = loader.leer_obstaculo();
            if (nuevo.es_naranja()) cantidad_naranjas++;
            obstaculos.push_back(nuevo);
        }

        unsigned int ticks = SDL_GetTicks();

        int bola_trabada = 0;  // Contador para saber si la bola quedó estancada
        bool pasar_nivel = false;
        ;
#ifdef TTF
        uint8_t color_ = 0xFF;
        int color_cambiar = -1;
        int contador_game_over = CONTADOR_GAME_OVER;
#endif
        while (!pasar_nivel && estado) {
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
            switch (estado) {
                ///////////////////////////////////////////////// JUEGO
                /// CORRIENDO /////////////////////////////////////////////////
                case GAME_RUNNING: {
                    if (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                            estado = GAME_CLOSE;
                            break;
                        }

                        if (event.type == SDL_KEYUP) {
                            if (event.key.keysym.sym == SDLK_q)
                                estado = GAME_LEVEL_UP;
                        }

                        if (event.type == SDL_MOUSEBUTTONDOWN) {
                            if (!cayendo) {
                                cayendo = true;
                                tray = trayectoria_t();
                            }
                        } else if (event.type == SDL_MOUSEMOTION) {
                            canon.update(atan2(event.motion.x - CANON_X,
                                               event.motion.y - CANON_Y));
                        }
                        continue;
                    }
#ifdef TTF
                    // escribir_texto(renderer, font, "Peggle", 100, 20);
                    SDL_SetRenderDrawColor(renderer, 0xFF, 0x60, 0x00, 0x00);
                    {
                        bola_t dibujito(MAX_X + 40, MAX_Y - 50, 10,
                                        RESOL_BOLA_OBS);
                        dibujito.dibujar(renderer);
                    }
                    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);

                    char contador_golpeados[6];
                    sprintf(contador_golpeados, "%zd/%zd", naranjas_golpeados,
                            cantidad_naranjas);
                    escribir_texto(renderer, font, contador_golpeados,
                                   MAX_X + 15, MAX_Y - 30, 0xFF, 0xFF, 0xFF);
                    escribir_texto(renderer, font, "Peggle", MAX_X / 2, 20,
                                   0xFF, 0xFF, 0xFF);
                    puntaje_escribir(renderer, font, puntaje_en_nivel, 600,
                                     MIN_Y / 2);
                    puntaje_multiplicador_escribir(renderer, font,
                                                   multiplicador, 730, 50);
                    nivel_escribir(renderer, font, nivel, MIN_X, MIN_Y / 2);
                    vidas_escribir(renderer, font, &vidas, MIN_X / 4, MIN_Y);
#endif

                    if (cayendo) {
                        // Si la bola está cayendo actualizamos su posición
                        v = computar_velocidad(v, G_VEC, DT) * ROZAMIENTO;
                        c = computar_posicion(c, v, DT);

                        // Se computa la velocidad en el siguiente instante para
                        // comparar con la actual luego y determinar  si quedó
                        // estancada
                        v_estancada = computar_velocidad(v, G_VEC, DT);
                        p_estancada = computar_posicion(c, v_estancada, DT);
                        //     computar_posicion(cx, vx * ROZAMIENTO, DT);
                        // bola_estancada_y = computar_posicion(
                        //     cy, bola_estancada_vy * ROZAMIENTO, DT);

                        // Se agrega una coordenada cada que el contador supera
                        // 5
                        if (contador_trayectoria > 5) {
                            tray.agregar_coordenada(c);
                            contador_trayectoria = 0;
                        }
                        tray.dibujar(renderer);
                        contador_trayectoria++;
                    } else {
                        // Si la bola no se disparó establecemos condiciones
                        // iniciales

                        c = canon.tip();
                        v = aVec2(std::sin(canon.angle()),
                                  std::cos(canon.angle()));
                        v *= BOLA_VI;
                        // trayectoria_destruir(tray);
                        // tray = NULL;
                        bola_recuperada = false;
                        puntaje_actualizar_multiplicador(&multiplicador,
                                                         naranjas_golpeados);
                        {
                            trayectoria_t calculada =
                                calcular(c, v, G_VEC, 0.01);
                            calculada.dibujar(renderer);
                        }
                    }

                    // Rebote contra las paredes:
                    if (c.x < MIN_X + BOLA_RADIO || c.x > MAX_X - BOLA_RADIO)
                        v.x = -v.x;
                    if (c.y < MIN_Y + BOLA_RADIO) v.y = -v.y;

                    // Se salió de la pantalla:
                    if (c.y > MAX_Y - BOLA_RADIO) {
                        cayendo = false;
                        for (obstaculo_t &obs : obstaculos) {
                            if (obs.get_tocado()) obs.set_dibujar(false);
                        }
                        if (!bola_recuperada) {
                            if (!vidas.estan_agotadas())
                                vidas.quitar();
                            else {
                                estado = GAME_LEVEL_FAILED;
                                break;
                            }
                        }
                    }

                    if (fabs(v.y) < 15 || aDistance(c, p_estancada) < 0.5)
                        bola_trabada++;
                    else
                        bola_trabada = 0;

                    if (bola_trabada > 120)
                        for (auto &obs : obstaculos) {
                            if (obs.get_tocado()) obs.set_dibujar(false);
                        }

                    // Dibujamos el cañón:
                    canon.draw(renderer);

                    // Dibujamos la bola:
                    {
                        bola_t bola(c, BOLA_RADIO, 10);
                        bola.dibujar(renderer);
                    }
                    // bola_destruir(bola);

                    // Dibujamos las vidas
                    if (!vidas.estan_agotadas()) vidas.dibujar(renderer);

                    // Dibujamos las paredes:
                    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
                    SDL_RenderDrawLine(renderer, MIN_X, MIN_Y, MAX_X, MIN_Y);
                    SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MAX_X, MAX_Y);
                    SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MIN_X, MIN_Y);
                    SDL_RenderDrawLine(renderer, MAX_X, MAX_Y, MAX_X, MIN_Y);

                    // Dibujamos el vector de velocidad:
                    //        SDL_RenderDrawLine(renderer, cx, cy, cx + vx, cy +
                    //        vy);

                    // Dibujamos el recuperador de bolas
                    recuperador.dibujar(renderer);
                    recuperador.mover(1);
                    if (recuperador.bola_recuperada(c.x, c.y))
                        bola_recuperada = true;

                    // Dibujasmos los obstaculos y realizamos la interacción con
                    // la bola
                    aVec2 norma;

                    for (auto &obs : obstaculos) {
                        if (obs.get_dibujar()) {
                            obs.dibujar(renderer);
                            if (obs.distancia(c, norma) < BOLA_RADIO) {
                                reflejar(norma, c, v);
                                v *= PLASTICIDAD;
                                if (!obs.es_gris()) {
                                    if (obs.es_naranja() &&
                                        obs.get_dibujar() == true &&
                                        obs.get_tocado() == false)
                                        naranjas_golpeados++;
                                    puntaje_actualizar(&puntaje_en_nivel, obs,
                                                       multiplicador);
                                    obs.set_tocado(true);
                                }
                            }
                        }
                        obs.mover(DT);
                    }

                    if (naranjas_golpeados == cantidad_naranjas) {
                        estado = GAME_LEVEL_UP;
                        naranjas_golpeados = 0;
                        puntaje_total += puntaje_en_nivel;
                        SDL_Delay(300);
                        cayendo = false;
                        break;
                    }
                    break;
                }

                    // ///////////////////////////////////////////////// LEVEL
                    // PASSED /////////////////////////////////////////////////

                case GAME_LEVEL_UP:  // Caso se pasó de nivel
                {
                    if (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                            estado = GAME_CLOSE;
                            break;
                        }

                        if (event.type == SDL_MOUSEBUTTONDOWN) {
                            estado = GAME_RUNNING;
                            pasar_nivel = true;
                            break;
                        } else if (event.type == SDL_MOUSEMOTION) {
                            canon.update(atan2(event.motion.x - CANON_X,
                                               event.motion.y - CANON_Y));
                        }

                        continue;
                    }

#ifdef TTF
                    char p_actual[14];
                    sprintf(p_actual, "SCORE: %zd", puntaje_en_nivel);

                    color_--;
                    escribir_texto(renderer, font, "MUY BIEN!!", 340, 200,
                                   color_, color_, color_);
                    escribir_texto(renderer, font, p_actual, 330, 260, color_,
                                   color_, color_);
                    escribir_texto(renderer, font, "Haga click para continuar",
                                   260, 230, color_, color_, color_);
#endif

                    // Dibujamos el cañón:
                    canon.draw(renderer);

                    // Dibujamos las paredes:
                    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
                    SDL_RenderDrawLine(renderer, MIN_X, MIN_Y, MAX_X, MIN_Y);
                    SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MAX_X, MAX_Y);
                    SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MIN_X, MIN_Y);
                    SDL_RenderDrawLine(renderer, MAX_X, MAX_Y, MAX_X, MIN_Y);

                    break;
                }

                    // ///////////////////////////////////////////////// NIVEL
                    // FALLIDO /////////////////////////////////////////////////
                case GAME_LEVEL_FAILED:  // Caso no se pasó de nivel
                {
                    if (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                            estado = GAME_CLOSE;
                            break;
                        }

                        if (event.type == SDL_MOUSEBUTTONDOWN) {
                            naranjas_golpeados = 0;
                            puntaje_en_nivel = 0;
                            estado = GAME_RUNNING;
                            for (auto &obs : obstaculos) {
                                resetear_obstaculos(obs);
                                // obs.set_tocado(&obs, false);
                                // obs.set_dibujar(&obs, true);
                            }
                            vidas.resetear();
#ifdef TTF
                            contador_game_over = CONTADOR_GAME_OVER;
#endif
                            break;
                        } else if (event.type == SDL_MOUSEMOTION) {
                            canon.update(atan2(event.motion.x - CANON_X,
                                               event.motion.y - CANON_Y));
                        }

                        continue;
                    }
                    for (auto &obs : obstaculos) {
                        return obs.dibujar((SDL_Renderer *)renderer);
                    }

#ifdef TTF
                    SDL_SetRenderDrawColor(renderer, 0xFF, 0x60, 0x00, 0x00);
                    {
                        bola_t dibujito(MAX_X + 40, MAX_Y - 50, 10,
                                        RESOL_BOLA_OBS);
                        dibujito.dibujar(renderer);
                    }
                    //     bola_crear(MAX_X + 40, MAX_Y - 50, 10,
                    //     RESOL_BOLA_OBS);
                    // bola_dibujar(renderer, dibujito);
                    // bola_destruir(dibujito);

                    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);

                    escribir_texto(renderer, font, "OOPs Nivel Fallido!", 290,
                                   MIN_Y + 18, 0xFF, 0xFF, 0xFF);
                    escribir_texto(renderer, font, "Haz click para reintentar",
                                   265, 290, color_, color_, color_);
                    escribir_numero(renderer, font, contador_game_over, 390,
                                    320, color_, color_, color_);
                    puntaje_escribir(renderer, font, puntaje_en_nivel, 345,
                                     MIN_Y + 48);
                    color_ += color_cambiar;
                    if (color_ == 0 || color_ == 255) {
                        color_cambiar = -color_cambiar;
                    }
                    if (color_ == 127) contador_game_over--;
                    if (contador_game_over < 0) {
                        estado = GAME_OVER;
                        break;
                    }

                    char contador_golpeados[6];
                    sprintf(contador_golpeados, "%zd/%zd", naranjas_golpeados,
                            cantidad_naranjas);
                    escribir_texto(renderer, font, contador_golpeados,
                                   MAX_X + 15, MAX_Y - 30, 0xFF, 0xFF, 0xFF);

                    sprintf(contador_golpeados, "%d", 0);
                    escribir_texto(renderer, font, contador_golpeados,
                                   MIN_X / 4, MIN_Y, 0xFF, 0xFF, 0xFF);

                    escribir_texto(renderer, font, "Peggle", MAX_X / 2, 20,
                                   0xFF, 0xFF, 0xFF);
                    puntaje_multiplicador_escribir(renderer, font,
                                                   multiplicador, 730, 50);
                    nivel_escribir(renderer, font, nivel, MIN_X, MIN_Y / 2);

#endif

                    // Dibujamos el cañón:
                    SDL_SetRenderDrawColor(renderer, 0xFE, 0XFE, 0XFE, 0X00);
                    canon.draw(renderer);

                    // Dibujamos las paredes:
                    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
                    SDL_RenderDrawLine(renderer, MIN_X, MIN_Y, MAX_X, MIN_Y);
                    SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MAX_X, MAX_Y);
                    SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MIN_X, MIN_Y);
                    SDL_RenderDrawLine(renderer, MAX_X, MAX_Y, MAX_X, MIN_Y);
                }

                break;

                    // ///////////////////////////////////////////////// GAME
                    // OVER /////////////////////////////////////////////////
                case GAME_OVER:  // Caso se termina el juego

                    if (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                            estado = GAME_CLOSE;
                            break;
                        }

                        if (event.type == SDL_MOUSEBUTTONDOWN) {
                            estado = GAME_CLOSE;
                            break;
                        }

                        continue;
                    }
#ifdef TTF
                    color_ += color_cambiar;
                    if (color_ == 0 || color_ == 255) {
                        color_cambiar = -color_cambiar;
                    }
                    escribir_texto(renderer, font, "GAME OVER", 305, 180,
                                   color_, color_, color_);
                    char final_score[22];
                    sprintf(final_score, "PUNTAJE TOTAL %zd", puntaje_total);
                    escribir_texto(renderer, font, final_score, 270, 230,
                                   color_, color_, color_);
#endif

                    // Dibujamos las paredes:
                    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
                    SDL_RenderDrawLine(renderer, MIN_X, MIN_Y, MAX_X, MIN_Y);
                    SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MAX_X, MAX_Y);
                    SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MIN_X, MIN_Y);
                    SDL_RenderDrawLine(renderer, MAX_X, MAX_Y, MAX_X, MIN_Y);

                    break;

                default:
                    break;
            }

            SDL_RenderPresent(renderer);
            ticks = SDL_GetTicks() - ticks;
            if (dormir) {
                SDL_Delay(dormir);
                dormir = 0;
            } else if (ticks < 1000 / JUEGO_FPS)
                SDL_Delay(1000 / JUEGO_FPS - ticks);
            ticks = SDL_GetTicks();
        }
    }

    // trayectoria_destruir(
    //     tray);  // Por si se cerró el juego antes de que la bola toque el
    //     piso
    // recuperador_destruir(recuperador);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

#ifdef TTF
    TTF_CloseFont(font);
    TTF_Quit();
#endif
    SDL_Quit();
    return 0;
}
