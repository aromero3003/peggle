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
#include "renderer.h"
#include "trayectoria.h"
#include "utility.h"
#include "vec2.h"
#include "vidas.h"

#define DT (1.0 / JUEGO_FPS)

const aVec2 G_VEC(0.0f, G);

void resetear_obstaculos(Obstacle &obs) {
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

void vidas_escribir(SDL_Renderer *renderer, TTF_Font *font, Lifes *vidas, int x,
                    int y) {
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
    // if (argc != 2) {
    //     std::cerr << "Usage: " << argv[0] << "<levels_path.bin" << std::endl;
    //     return 1;
    // }

    // SDL_Init(SDL_INIT_VIDEO);
    Renderer r(VENTANA_ANCHO, VENTANA_ALTO, 0);

#ifdef TTF
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("resources/FreeSansBold.ttf", 24);
#endif

    // SDL_Window *window;
    // SDL_Renderer *renderer;
    SDL_Event event;

    // SDL_CreateWindowAndRenderer(VENTANA_ANCHO, VENTANA_ALTO, 0, &window,
    //                             &renderer);
    // SDL_SetWindowTitle(window, "Peggle");

    // Renderer r(renderer);
    int dormir = 0;

    Ball b(aVec2(CANON_X, CANON_Y + CANON_LARGO), BOLA_RADIO, BOLA_RESOL);
    Cannon canon(aVec2(CANON_X, CANON_Y), b);  // Ángulo del cañón

    // contador para actualizar la trayectoria en intervalos de 5
    size_t contador_trayectoria = 0;

    puntaje_t puntaje_total = 0;

    Trajectory tray;

    Loader loader("resources/levels.bin");

    int nivel = 0;
    Game game(loader);

    Lifes vidas(VIDAS_INICIALES, 60, MIN_Y + BOLA_RADIO);
    // if (vidas == NULL) {
    //     fclose(f);
    //     SDL_DestroyRenderer(renderer);
    //     SDL_DestroyWindow(window);
    //     return 3;
    // }
    // Retriever *recuperador = recuperador_crear(60, 10, 0.6);
    Retriever recuperador(60, 10, 0.6);
    // if (recuperador == NULL) {
    //     fclose(f);
    //     SDL_DestroyRenderer(renderer);
    //     SDL_DestroyWindow(window);
    //     return 3;
    // }

    // game_state_t estado = GAME_RUNNING;

    while (game.state) {
        if (game.current_level + 1 == game.getTotalLevels()) {
            game.state = GAME_OVER;
        }
        vidas.resetear();  // Se setean las vidas a la cantidad inicial

        Level *level = game.getCurrentLevel();
        unsigned int ticks = SDL_GetTicks();
#ifdef TTF
        uint8_t color_ = 0xFF;
        int color_cambiar = -1;
        int contador_game_over = CONTADOR_GAME_OVER;
#endif
        while (game.state) {
            // SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
            r.setColor(0x00, 0x00, 0x00, 0x00);
            // SDL_RenderClear(renderer);
            r.clear();
            // SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
            r.setColor(0xFF, 0xFF, 0xFF, 0x00);
            switch (game.state) {
                ///////////////////////////////////////////////// JUEGO
                /// CORRIENDO /////////////////////////////////////////////////
                case GAME_RUNNING: {
                    if (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                            game.state = GAME_CLOSE;
                            break;
                        }

                        switch (event.type) {
                            case SDL_KEYUP:
                                if (event.key.keysym.sym == SDLK_q)
                                    game.state = GAME_LEVEL_UP;
                                break;

                            case SDL_MOUSEBUTTONDOWN:
                                canon.fire();
                                // tray = Trajectory();
                            case SDL_MOUSEMOTION:
                                canon.update(atan2(event.motion.x - CANON_X,
                                                   event.motion.y - CANON_Y));
                                break;
                            default:
                                continue;
                                break;
                        }
                    }
#ifdef TTF
                    // escribir_texto(renderer, font, "Peggle", 100, 20);
                    SDL_SetRenderDrawColor(renderer, 0xFF, 0x60, 0x00, 0x00);
                    {
                        Ball dibujito(MAX_X + 40, MAX_Y - 50, 10,
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

                    if (b.esta_cayendo()) {
                        b.actualizar(DT);
                        // Se agrega una coordenada cada 5 iteraciones
                        if (contador_trayectoria > 5) {
                            tray.agregar_coordenada(b.centro);
                            contador_trayectoria = 0;
                        }
                        // tray.dibujar(renderer);
                        r.drawTrajectory(tray);
                        contador_trayectoria++;
                    } else {
                        tray = Trajectory();
                        // {
                        //     Trajectory calculada =
                        //         calcular(c, v, G_VEC, 0.01);
                        //     calculada.dibujar(renderer);
                        // }
                    }

                    // Se salió de la pantalla:
                    if (b.centro.y > MAX_Y - BOLA_RADIO) {
                        if (not recuperador.recuperar(b)) {
                            if (vidas.estan_agotadas()) {
                                game.state = GAME_LEVEL_FAILED;
                                break;
                            }
                            vidas.quitar();
                        }
                        canon.reload();
                        level->update_multiplier();
                        level->clean_touched_obstacles();
                    }

                    if (b.esta_trabada()) level->clean_touched_obstacles();

                    r.drawCannon(canon);  // Dibujamos el cañón:
                    r.drawBall(b);        // Dibujamos la bola:
                    r.drawScenario();     // Dibujamos las paredes:
                    r.drawLifes(vidas);   // Dibujamos las vidas
                    r.drawRetriever(recuperador);
                    // Dibujamos el vector de velocidad:
                    //        SDL_RenderDrawLine(renderer, cx, cy, cx + vx, cy +
                    //        vy);

                    // Dibujamos el recuperador de bolas
                    recuperador.mover(1);

                    // Dibujamos los obstaculos y realizamos la interacción con
                    // la bola
                    level->handle_collisions(b);
                    level->move_obstacles(DT);
                    r.drawLevel(level);
                    // level->draw(renderer);

                    if (level->is_completed()) {
                        canon.reload();
                        game.state = GAME_LEVEL_UP;
                        // puntaje_total += puntaje_en_nivel;
                        SDL_Delay(300);
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
                            game.state = GAME_CLOSE;
                            break;
                        }

                        if (event.type == SDL_MOUSEBUTTONDOWN) {
                            if (game.current_level + 1 == game.getTotalLevels())
                                game.state = GAME_OVER;
                            else {
                                // game.state = GAME_RUNNING;
                                game.state = GAME_RUNNING;
                                canon.reload();
                                game.current_level++;
                                level = game.getCurrentLevel();
                            }
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
                    r.drawCannon(canon);

                    // Dibujamos las paredes:
                    r.drawScenario();

                    break;
                }

                    // ///////////////////////////////////////////////// NIVEL
                    // FALLIDO /////////////////////////////////////////////////
                case GAME_LEVEL_FAILED:  // Caso no se pasó de nivel
                {
                    vidas.resetear();
                    level->reset();
                    if (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                            game.state = GAME_CLOSE;
                            break;
                        }

                        if (event.type == SDL_MOUSEBUTTONDOWN) {
                            // naranjas_golpeados = 0;
                            // puntaje_en_nivel = 0;
                            game.state = GAME_RUNNING;
                            /*
                            for (auto &obs : obstaculos) {
                                resetear_obstaculos(obs);
                                // obs.set_tocado(&obs, false);
                                // obs.set_dibujar(&obs, true);
                            }
                            */
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
#ifdef TTF
                    SDL_SetRenderDrawColor(renderer, 0xFF, 0x60, 0x00, 0x00);
                    {
                        Ball dibujito(MAX_X + 40, MAX_Y - 50, 10,
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
                        game.state = GAME_OVER;
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
                    // level->draw(renderer);
                    r.drawLevel(level);
                    r.drawCannon(canon);  // Dibujamos el cañón
                    r.drawScenario();     // Dibujamos las paredes
                }

                break;

                    // ///////////////////////////////////////////////// GAME
                    // OVER /////////////////////////////////////////////////
                case GAME_OVER:  // Caso se termina el juego

                    if (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                            game.state = GAME_CLOSE;
                            break;
                        }

                        if (event.type == SDL_MOUSEBUTTONDOWN) {
                            game.state = GAME_CLOSE;
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
                    r.drawScenario();

                    break;

                default:
                    break;
            }

            // SDL_RenderPresent(renderer);
            r.present();
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

    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);

#ifdef TTF
    TTF_CloseFont(font);
    TTF_Quit();
#endif
    // SDL_Quit();
    return 0;
}
