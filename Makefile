CC=gcc
CFLAGS=-Wall -pedantic -std=c99
DEBUG=-g
SDLFLAGS=-lm -lSDL2 -lSDL2_ttf
DFLAG=-DTTF

all: peggle

peggle: obstaculo.o poligono.o lectura.o peggle.o lista.o juego.o  
	$(CC) $(CFLAGS) $(DEBUG) obstaculo.o poligono.o lectura.o peggle.o lista.o juego.o -o peggle $(SDLFLAGS)

peggle.o: peggle.c obstaculo.h poligono.h config.h lista.h juego.h
	$(CC) -c peggle.c $(DFLAG)

juego.o: juego.c juego.h obstaculo.h poligono.h config.h tipos.h
	$(CC) -c juego.c 


obstaculo.o: obstaculo.c obstaculo.h lectura.h config.h tipos.h
	$(CC) -c obstaculo.c

lectura.o: lectura.c lectura.h tipos.h config.h
	$(CC) -c lectura.c

poligono.o: poligono.c poligono.h config.h
	$(CC) -c poligono.c

lista.o: lista.c lista.h
	$(CC) -c lista.c

clean:
	rm *.o