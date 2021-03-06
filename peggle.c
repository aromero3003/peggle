#include <SDL2/SDL.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "obstaculo.h"
#include "poligono.h"
#include "config.h"
#include "lista.h"
#include "juego.h"

#define DT (1.0 / JUEGO_FPS)




bool wrapper_actualizar_obstaculo(void *obstaculo, void *recibo_null ) {
    if(obstaculo_get_tocado((obstaculo_t *)obstaculo))
        obstaculo_set_dibujar((obstaculo_t *) obstaculo, false);
    return true;
}

bool wrapper_dibujar_obstaculo(void *obstaculo, void *renderer) {
    return obstaculo_dibujar((SDL_Renderer *)renderer, (obstaculo_t *)obstaculo);
}

void wrapper_destruccion_obstaculo(void *obstaculo) {
    obstaculo_destruir((obstaculo_t *)obstaculo);
}

bool wrapper_resetear_obstaculos(void *obstaculo, void *recibo_null) {
    obstaculo_set_tocado((obstaculo_t *)obstaculo, false);
    obstaculo_set_dibujar((obstaculo_t *)obstaculo, true);
    return true;
}




#define DT (1.0 / JUEGO_FPS)

#ifdef TTF
#include <SDL2/SDL_ttf.h>

// ------------------------------------------------------------- FUNCIONES QUE ESCRIBEN TEXTO EN LA PANTALLA -------------------------------------------------------------

void escribir_texto(SDL_Renderer *renderer, TTF_Font *font, const char *s, int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    SDL_Color color = {r, g, b};  // Estaría bueno si la función recibe un enumerativo con el color, ¿no?
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


void puntaje_escribir(SDL_Renderer *renderer, TTF_Font *font , puntaje_t puntaje, int x, int y) {
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

void puntaje_multiplicador_escribir(SDL_Renderer *renderer, TTF_Font *font , int multiplicador, int x, int y) {
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

void nivel_escribir(SDL_Renderer *renderer, TTF_Font *font , int nivel, int x, int y) {
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

void vidas_escribir(SDL_Renderer *renderer, TTF_Font *font , vidas_t *vidas, int x, int y) {
    char v[3];
    size_t aux = vidas_restantes(vidas);
    sprintf(v, "%zd", aux);
    escribir_texto(renderer, font, v, x, y, 0xFF, 0xFF, 0xFF);
}

void escribir_numero(SDL_Renderer *renderer, TTF_Font *font , int contador, int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    char c[4];
    sprintf(c, "%ds", contador);
    escribir_texto(renderer, font, c, x, y, r, g, b);
}

#endif



int main(int argc, char *argv[]) {

    if(argc != 2) {
        fprintf(stderr, "Uso: %s <archivo_nivel.bin>\n", argv[0]);
        return 1;
    }
    
    SDL_Init(SDL_INIT_VIDEO);

#ifdef TTF
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("FreeSansBold.ttf", 24);
#endif

    
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    

    SDL_CreateWindowAndRenderer(VENTANA_ANCHO, VENTANA_ALTO, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Peggle");


    int dormir = 0;

    float canon_angulo = 0; // Ángulo del cañón
    bool cayendo = false;   // ¿Hay bola disparada?

    float cx, cy;   // Centro de la bola
    float vx, vy;   // Velocidad de la bola

    int bola_estancada_vy; // Estos 3 son parametros para calcular el modulo de
    int bola_estancada_x;  // la posición y empezar un contador para determinar
    int bola_estancada_y;  // si la bola quedó estancada;

    bool bola_recuperada = false; // Para saber si la bola entró al recuperador de bolas


    size_t contador_trayectoria = 20; // contador para actualizar la trayectoria en intervalos de 5

    puntaje_t puntaje_total = 0;

    trayectoria_t *tray = NULL;

    FILE *f = fopen(argv[1], "rb");
    if(f == NULL) {
        fprintf(stderr, "No se pudo abrir\n");
        return 1;
    }

    int16_t cant_obstaculos;
    int nivel = 0;

    vidas_t *vidas = vidas_inicializar(VIDAS_INICIALES, 60, MIN_Y  + BOLA_RADIO);
    if(vidas == NULL) {
        fclose(f);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return 3;
    }
    recuperador_t *recuperador = recuperador_crear(60, 10, 0.6);
    if(recuperador == NULL) {
        vidas_destruir(vidas);
        fclose(f);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return 3;
    }

    game_state_t estado = GAME_RUNNING;

    while( estado) {
        
        if(! obstaculo_leer_cantidad_de_obstaculos(f, &cant_obstaculos)) { // Si no pude leer más obstaculos, GAME OVER
            estado = GAME_OVER;
            cant_obstaculos = 0;
        }

        size_t cantidad_naranjas = 0, naranjas_golpeados = 0;
        puntaje_t puntaje_en_nivel = 0;
        int multiplicador = 1;
        nivel++;

        vidas_resetear(vidas); // Se setean las vidas a la cantidad inicial

        lista_t *obstaculos = lista_crear();

        if(obstaculos == NULL) {
            //fprintf(stderr, "No se pudo crear lista\n");
            //return 1;
            break;
        }

        
        for (size_t i = 0; i < cant_obstaculos; i++) {                    // Se levantan todos los obstáculos del nivel
            obstaculo_t *nuevo = obstaculo_levantar_obstaculo(f);
            if(obstaculo_es_naranja(nuevo))
                cantidad_naranjas++;
            lista_insertar_al_principio(obstaculos, nuevo);
        }

        unsigned int ticks = SDL_GetTicks();

        int bola_trabada = 0; // Contador para saber si la bola quedó estancada
        bool pasar_nivel = false;;
#ifdef TTF
        uint8_t color_ = 0xFF;
        int color_cambiar = -1;
        int contador_game_over = CONTADOR_GAME_OVER;
#endif
        while(! pasar_nivel && estado ) {

            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
            switch (estado) {

                ///////////////////////////////////////////////// JUEGO CORRIENDO /////////////////////////////////////////////////
                case GAME_RUNNING:
                    if(SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                            estado = GAME_CLOSE;
                            break;
                        }

                        if(event.type == SDL_MOUSEBUTTONDOWN) {
                            if(! cayendo) {
                                cayendo = true;
                                tray = trayectoria_crear();
                            }
                        }
                        else if (event.type == SDL_MOUSEMOTION) {
                            canon_angulo = atan2(event.motion.x - CANON_X, event.motion.y - CANON_Y);
                            if(canon_angulo > CANON_MAX)
                                canon_angulo = CANON_MAX;
                            if(canon_angulo < -CANON_MAX)
                                canon_angulo = -CANON_MAX;
                        }
                        continue;
                    }
#ifdef TTF
                        //escribir_texto(renderer, font, "Peggle", 100, 20);
                        SDL_SetRenderDrawColor(renderer, 0xFF, 0x60, 0x00, 0x00);
                        bola_t *dibujito = bola_crear( MAX_X + 40, MAX_Y - 50, 10, RESOL_BOLA_OBS);
                        bola_dibujar(renderer, dibujito);
                        bola_destruir(dibujito);
                        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);

                        char contador_golpeados[6];
                        sprintf(contador_golpeados, "%zd/%zd", naranjas_golpeados, cantidad_naranjas);
                        escribir_texto(renderer, font, contador_golpeados, MAX_X + 15, MAX_Y - 30, 0xFF, 0xFF, 0xFF);
                        escribir_texto(renderer, font, "Peggle", MAX_X / 2, 20, 0xFF, 0xFF, 0xFF);
                        puntaje_escribir(renderer, font, puntaje_en_nivel, 600, MIN_Y / 2);
                        puntaje_multiplicador_escribir(renderer, font , multiplicador, 730, 50);
                        nivel_escribir(renderer ,font, nivel, MIN_X, MIN_Y / 2);
                        vidas_escribir(renderer, font, vidas, MIN_X / 4, MIN_Y); 
#endif

                    if(cayendo) {
                        // Si la bola está cayendo actualizamos su posición
                        vy = computar_velocidad(vy, G, DT);
                        vx *= ROZAMIENTO;
                        vy *= ROZAMIENTO;
                        cx = computar_posicion(cx, vx, DT);
                        cy = computar_posicion(cy, vy, DT);

                        // Se computa la velocidad en el siguiente instante para comparar con la actual luego
                        // y determinar  si quedó estancada
                        bola_estancada_vy = computar_velocidad(vy, G, DT);
                        bola_estancada_x = computar_posicion(cx, vx * ROZAMIENTO, DT);
                        bola_estancada_y = computar_posicion(cy, bola_estancada_vy * ROZAMIENTO, DT);

                        // Se agrega una coordenada cada que el contador supera 5
                        if(contador_trayectoria > 5) {
                            trayectoria_agregar_coordenada(tray, cx,cy);
                            contador_trayectoria = 0;
                        }
                        trayectoria_dibujar(renderer, tray);
                        contador_trayectoria++;
                    }
                    else {
                        // Si la bola no se disparó establecemos condiciones iniciales
                        cx = CANON_X + CANON_LARGO * sin(canon_angulo);
                        cy = CANON_Y + CANON_LARGO * cos(canon_angulo);
                        vx = BOLA_VI * sin(canon_angulo);
                        vy = BOLA_VI * cos(canon_angulo);
                        trayectoria_destruir(tray);
                        tray = NULL;
                        bola_recuperada = false;
                        puntaje_actualizar_multiplicador(&multiplicador, naranjas_golpeados);
                        trayectoria_t *calculada = trayectoria_calcular(cx, cy, vx, vy, G, 0.01);
                        trayectoria_dibujar(renderer, calculada);
                        trayectoria_destruir(calculada);


                    }
                    
                    // Rebote contra las paredes:
                    if(cx < MIN_X + BOLA_RADIO || cx > MAX_X - BOLA_RADIO) vx = -vx;
                    if(cy < MIN_Y + BOLA_RADIO) vy = -vy;

                    // Se salió de la pantalla:
                    if(cy > MAX_Y - BOLA_RADIO) {
                        cayendo = false;
                        lista_recorrer(obstaculos, wrapper_actualizar_obstaculo, NULL);
                        if(! bola_recuperada) {
                            if(! vidas_estan_agotadas(vidas))
                                vidas_quitar(vidas);
                            else {
                                estado = GAME_LEVEL_FAILED;
                                break;
                            }
                        }
                    }

                    if(modulo(0, vy) < 15 || modulo(bola_estancada_x - cx, bola_estancada_y - cy) < 0.5) 
                        bola_trabada++;
                    else 
                        bola_trabada = 0;

                    if(bola_trabada > 120)
                        lista_recorrer(obstaculos, wrapper_actualizar_obstaculo, renderer);
                    
                    // Dibujamos el cañón:
                    SDL_RenderDrawLine(renderer, CANON_X, CANON_Y, CANON_X + sin(canon_angulo) * CANON_LARGO, CANON_Y + cos(canon_angulo) * CANON_LARGO);

                    // Dibujamos la bola:
                    bola_t *bola = bola_crear(cx, cy, BOLA_RADIO, 10);
                    bola_dibujar(renderer, bola);
                    bola_destruir(bola);
                    
                    // Dibujamos las vidas
                    if(! vidas_estan_agotadas(vidas))
                        vidas_dibujar(renderer, vidas);

                    // Dibujamos las paredes:
                    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
                    SDL_RenderDrawLine(renderer, MIN_X, MIN_Y, MAX_X, MIN_Y);
                    SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MAX_X, MAX_Y);
                    SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MIN_X, MIN_Y);
                    SDL_RenderDrawLine(renderer, MAX_X, MAX_Y, MAX_X, MIN_Y);



                    // Dibujamos el vector de velocidad:
            //        SDL_RenderDrawLine(renderer, cx, cy, cx + vx, cy + vy);

                    // Dibujamos el recuperador de bolas
                    recuperador_dibujar(renderer, recuperador);
                    recuperador_mover(recuperador, 1);
                    if(recuperador_bola_recuperada(recuperador, cx, cy))
                        bola_recuperada = true;

                    // Dibujasmos los obstaculos y realizamos la interacción con la bola
                    lista_iter_t* iterador = lista_iter_crear(obstaculos);
                    float nor_x, nor_y;

                    while(! lista_iter_al_final(iterador)) {
                        obstaculo_t *actual = lista_iter_ver_actual(iterador);
                        if(obstaculo_get_dibujar(actual)) {

                            obstaculo_dibujar(renderer, actual);
                            if(obstaculo_distancia(actual, cx, cy, &nor_x, &nor_y) < BOLA_RADIO) {
                                
                                reflejar(nor_x, nor_y, &cx, &cy, &vx, &vy);
                                vy *= PLASTICIDAD;
                                vx *= PLASTICIDAD;
                                if(! obstaculo_es_gris(actual)){
                                    
                                    if(obstaculo_es_naranja(actual) && obstaculo_get_dibujar(actual) == true && obstaculo_get_tocado(actual) == false)
                                        naranjas_golpeados++;
                                    puntaje_actualizar(&puntaje_en_nivel, actual, multiplicador);
                                    obstaculo_set_tocado(actual, true);
                                }
                            }
                        }
                        obstaculo_mover(actual, DT);
                        lista_iter_avanzar(iterador);
                    }
                    lista_iter_destruir(iterador);
                    
                    if(naranjas_golpeados == cantidad_naranjas) {
                        estado = GAME_LEVEL_UP;
                        naranjas_golpeados = 0;
                        puntaje_total += puntaje_en_nivel;
                        SDL_Delay(300);
                        cayendo = false;
                        break;
                    }
                    break;

// ///////////////////////////////////////////////// LEVEL PASSED /////////////////////////////////////////////////   


            case GAME_LEVEL_UP: //Caso se pasó de nivel
                    
                if(SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        estado = GAME_CLOSE;
                        break;
                    }

                    if(event.type == SDL_MOUSEBUTTONDOWN) {
                        estado = GAME_RUNNING;
                        pasar_nivel = true;
                        break;
                    }
                    else if (event.type == SDL_MOUSEMOTION) {
                        canon_angulo = atan2(event.motion.x - CANON_X, event.motion.y - CANON_Y);
                        if(canon_angulo > CANON_MAX)
                            canon_angulo = CANON_MAX;
                        if(canon_angulo < -CANON_MAX)
                            canon_angulo = -CANON_MAX;
                    }

                    continue;
                }

#ifdef TTF
                char p_actual[14];
                sprintf(p_actual, "SCORE: %zd", puntaje_en_nivel);

                color_--;
                escribir_texto(renderer, font, "MUY BIEN!!", 340, 200, color_, color_, color_);
                escribir_texto(renderer, font, p_actual, 330, 260, color_, color_, color_);
                escribir_texto(renderer, font, "Haga click para continuar", 260, 230, color_, color_, color_);
#endif

                // Dibujamos el cañón:
                SDL_RenderDrawLine(renderer, CANON_X, CANON_Y, CANON_X + sin(canon_angulo) * CANON_LARGO, CANON_Y + cos(canon_angulo) * CANON_LARGO);

                // Dibujamos las paredes:
                SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
                SDL_RenderDrawLine(renderer, MIN_X, MIN_Y, MAX_X, MIN_Y);
                SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MAX_X, MAX_Y);
                SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MIN_X, MIN_Y);
                SDL_RenderDrawLine(renderer, MAX_X, MAX_Y, MAX_X, MIN_Y);

                break;

// ///////////////////////////////////////////////// NIVEL FALLIDO ///////////////////////////////////////////////// 
            case GAME_LEVEL_FAILED: //Caso no se pasó de nivel
                    
                if(SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        estado = GAME_CLOSE;
                        break;
                    }

                    if(event.type == SDL_MOUSEBUTTONDOWN) {
                        naranjas_golpeados = 0;
                        puntaje_en_nivel = 0;
                        estado = GAME_RUNNING;
                        lista_recorrer(obstaculos, wrapper_resetear_obstaculos, NULL);
                        vidas_resetear(vidas);
#ifdef TTF
                        contador_game_over = CONTADOR_GAME_OVER;
#endif
                        break;
                    }
                    else if (event.type == SDL_MOUSEMOTION) {
                        canon_angulo = atan2(event.motion.x - CANON_X, event.motion.y - CANON_Y);
                        if(canon_angulo > CANON_MAX)
                            canon_angulo = CANON_MAX;
                        if(canon_angulo < -CANON_MAX)
                            canon_angulo = -CANON_MAX;
                    }

                    continue;
                }
                lista_recorrer(obstaculos, wrapper_dibujar_obstaculo, renderer);

#ifdef TTF
                SDL_SetRenderDrawColor(renderer, 0xFF, 0x60, 0x00, 0x00);
                dibujito = bola_crear( MAX_X + 40, MAX_Y - 50, 10, RESOL_BOLA_OBS);
                bola_dibujar(renderer, dibujito);
                bola_destruir(dibujito);
                
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);


                escribir_texto(renderer, font, "OOPs Nivel Fallido!", 290, MIN_Y + 18, 0xFF, 0xFF, 0xFF);
                escribir_texto(renderer, font, "Haz click para reintentar", 265, 290, color_, color_, color_);
                escribir_numero(renderer, font, contador_game_over, 390, 320, color_ , color_ , color_);
                puntaje_escribir(renderer, font, puntaje_en_nivel, 345 , MIN_Y + 48);
                color_ += color_cambiar;
                if(color_ == 0 || color_ == 255) {
                    color_cambiar = -color_cambiar;
                }
                if (color_ == 127)
                    contador_game_over--;
                if(contador_game_over < 0) {
                    estado = GAME_OVER;
                    break;
                }

                sprintf(contador_golpeados, "%zd/%zd", naranjas_golpeados, cantidad_naranjas);
                escribir_texto(renderer, font, contador_golpeados, MAX_X + 15, MAX_Y - 30, 0xFF, 0xFF, 0xFF);

                sprintf(contador_golpeados, "%d", 0);
                escribir_texto(renderer, font, contador_golpeados, MIN_X / 4, MIN_Y, 0xFF, 0xFF, 0xFF);

                escribir_texto(renderer, font, "Peggle", MAX_X / 2, 20, 0xFF, 0xFF, 0xFF);
                puntaje_multiplicador_escribir(renderer, font , multiplicador, 730, 50);
                nivel_escribir(renderer ,font, nivel, MIN_X, MIN_Y / 2);


#endif

                // Dibujamos el cañón:
                SDL_SetRenderDrawColor(renderer, 0xFE, 0XFE, 0XFE, 0X00);
                SDL_RenderDrawLine(renderer, CANON_X, CANON_Y, CANON_X + sin(canon_angulo) * CANON_LARGO, CANON_Y + cos(canon_angulo) * CANON_LARGO);

                // Dibujamos las paredes:
                SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
                SDL_RenderDrawLine(renderer, MIN_X, MIN_Y, MAX_X, MIN_Y);
                SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MAX_X, MAX_Y);
                SDL_RenderDrawLine(renderer, MIN_X, MAX_Y, MIN_X, MIN_Y);
                SDL_RenderDrawLine(renderer, MAX_X, MAX_Y, MAX_X, MIN_Y);

                break;
            

// ///////////////////////////////////////////////// GAME OVER ///////////////////////////////////////////////// 
            case GAME_OVER: //Caso se termina el juego

                if(SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {
                            estado = GAME_CLOSE;
                            break;
                        }

                        if(event.type == SDL_MOUSEBUTTONDOWN) {
                            estado = GAME_CLOSE;
                            break;
                        }

                        continue;
                    }
#ifdef TTF
                    color_ += color_cambiar;
                    if(color_ == 0 || color_ == 255) {
                        color_cambiar = -color_cambiar;
                    }
                    escribir_texto(renderer, font, "GAME OVER", 305, 180, color_, color_, color_);
                    char final_score[22];
                    sprintf(final_score, "PUNTAJE TOTAL %zd", puntaje_total);
                    escribir_texto(renderer, font, final_score, 270, 230, color_, color_, color_);
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
            if(dormir) {
                SDL_Delay(dormir);
                dormir = 0;
            }
            else if(ticks < 1000 / JUEGO_FPS)
                SDL_Delay(1000 / JUEGO_FPS - ticks);
            ticks = SDL_GetTicks();

        }
        lista_destruir(obstaculos, wrapper_destruccion_obstaculo);

    }

    trayectoria_destruir(tray); // Por si se cerró el juego antes de que la bola toque el piso
    recuperador_destruir(recuperador);
    vidas_destruir(vidas);
    fclose(f);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

#ifdef TTF
    TTF_CloseFont(font);
    TTF_Quit();
#endif
    SDL_Quit();
    return 0;
}