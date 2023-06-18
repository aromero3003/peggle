#include "peggle.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#include <iostream>
#include <list>

#include "ball.h"
#include "cannon.h"
#include "game.h"
#include "hybrid_renderer.h"
#include "level.h"
#include "lifes.h"
#include "loader.h"
#include "obstacle.h"
#include "polygon.h"
#include "renderer.h"
#include "retriever.h"
#include "settings.h"
#include "trajectory.h"
#include "utility.h"

#define DT (1.0 / JUEGO_FPS)

const aVec2 G_VEC(0.0f, G);

void resetear_obstaculos(Obstacle &obs) {
    obs.set_tocado(false);
    obs.set_dibujar(true);
}

#define DT (1.0 / JUEGO_FPS)

Peggle::Peggle()
    : r(VENTANA_ANCHO, VENTANA_ALTO, 0),
      b(aVec2(CANON_X, CANON_Y + CANON_LARGO), BOLA_RADIO, BOLA_RESOL),
      canon(aVec2(CANON_X, CANON_Y), b),
      vidas(VIDAS_INICIALES, 60, MIN_Y + BOLA_RADIO),
      recuperador(60, 10, 0.6) {}

void Peggle::run() {
    int dormir = 0;

    // contador para actualizar la trayectoria en intervalos de 5
    Loader loader("resources/levels.bin");

    int nivel = 0;
    Game game(loader);

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
            r.setColor(0x00, 0x00, 0x00, 0x00);
            r.clear();
            r.setColor(0xFF, 0xFF, 0xFF, 0x00);
            switch (game.state) {
                case GAME_RUNNING:
                    gameRunning(game, &level);
                    break;

                case GAME_LEVEL_UP:  // Caso se pasó de nivel
                    gameLevelUp(game, &level);
                    break;

                case GAME_LEVEL_FAILED:  // Caso no se pasó de nivel
                    gameLevelFailed(game, &level);
                    break;

                case GAME_OVER:  // Caso se termina el juego
                    gameOver(game);
                    break;

                case GAME_CLOSE:
                    return;
                    break;
            }

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
}

void Peggle::gameRunning(Game &game, Level **level) {
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            game.state = GAME_CLOSE;
        }

        switch (event.type) {
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_q)
                    game.state = GAME_LEVEL_UP;
                else if (event.key.keysym.sym == SDLK_e)
                    game.state = GAME_LEVEL_FAILED;
                break;

            case SDL_MOUSEBUTTONDOWN:
                canon.fire();
                // tray = Trajectory();
            case SDL_MOUSEMOTION:
                canon.update(
                    atan2(event.motion.x - CANON_X, event.motion.y - CANON_Y));
                break;
            default:
                break;
        }
    }

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
                return;
            }
            vidas.quitar();
        }
        canon.reload();
        (*level)->update_multiplier();
        (*level)->clean_touched_obstacles();
    }

    if (b.esta_trabada()) (*level)->clean_touched_obstacles();

    // Dibujamos el vector de velocidad:
    //        SDL_RenderDrawLine(renderer, cx, cy, cx + vx, cy +
    //        vy);

    recuperador.mover(1);

    /* Realizamos la interacción con la bola */
    (*level)->handle_collisions(b);
    (*level)->move_obstacles(DT);

    /* Dibujamos los elementos del juego */
    r.drawCannon(canon);
    r.drawBall(b);
    r.drawScenario();
    r.drawLifes(vidas);
    r.drawRetriever(recuperador);
    r.drawLevel(*level);
    r.drawTitle();

    if ((*level)->is_completed()) {
        canon.reload();
        game.state = GAME_LEVEL_UP;
        // puntaje_total += puntaje_en_nivel;
        SDL_Delay(300);
    }
}

void Peggle::gameOver(Game &game) {
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            game.state = GAME_CLOSE;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            game.state = GAME_CLOSE;
        }
    }
#ifdef TTF
    color_ += color_cambiar;
    if (color_ == 0 || color_ == 255) {
        color_cambiar = -color_cambiar;
    }
#endif
    r.drawScenario();  // Dibujamos las paredes:
    r.drawTitle();
    r.drawGameOver(puntaje_total);
}

void Peggle::gameLevelFailed(Game &game, Level **level) {
    vidas.resetear();
    (*level)->reset();
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            game.state = GAME_CLOSE;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            game.state = GAME_RUNNING;
            (*level)->reset(); /* OJO */
            vidas.resetear();
#ifdef TTF
            contador_game_over = CONTADOR_GAME_OVER;
#endif
        } else if (event.type == SDL_MOUSEMOTION) {
            canon.update(
                atan2(event.motion.x - CANON_X, event.motion.y - CANON_Y));
        }
    }
#ifdef TTF
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
    color_ += color_cambiar;
    if (color_ == 0 || color_ == 255) {
        color_cambiar = -color_cambiar;
    }
    if (color_ == 127) contador_game_over--;
    if (contador_game_over < 0) {
        game.state = GAME_OVER;
        break;
    }
#endif
    r.drawLifes(vidas);
    r.drawLevel(*level);
    r.drawCannon(canon);  // Dibujamos el cañón
    r.drawScenario();     // Dibujamos las paredes
    r.drawTitle();
    r.drawLevelFailed(*level);
}

void Peggle::gameLevelUp(Game &game, Level **level) {
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            game.state = GAME_CLOSE;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (game.current_level + 1 == game.getTotalLevels())
                game.state = GAME_OVER;
            else {
                vidas.resetear();
                game.state = GAME_RUNNING;
                canon.reload();
                game.current_level++;
                *level = game.getCurrentLevel();
            }
        } else if (event.type == SDL_MOUSEMOTION) {
            canon.update(
                atan2(event.motion.x - CANON_X, event.motion.y - CANON_Y));
        }
    }
    r.drawLevelUp(*level);
    r.drawCannon(canon);  // Dibujamos el cañón:
    r.drawScenario();     // Dibujamos las paredes:
    r.drawTitle();
}
