CC=g++
CFLAGS=-Wall -pedantic -std=c++17
DEBUG=-g
SDLFLAGS=-lm -lSDL2 -lSDL2_ttf
DFLAG=-DTTF

all: peggle

peggle: obstaculo.o poligono.o lectura.o peggle.o lista.o juego.o  
	$(CC) $(CFLAGS) $(DEBUG) obstaculo.o poligono.o lectura.o peggle.o lista.o juego.o -o peggle $(SDLFLAGS)

peggle.o: peggle.cpp obstaculo.h poligono.h config.h lista.h juego.h
	$(CC) -c peggle.cpp $(DFLAG)

juego.o: juego.cpp juego.h obstaculo.h poligono.h config.h tipos.h
	$(CC) -c juego.cpp 


obstaculo.o: obstaculo.cpp obstaculo.h lectura.h config.h tipos.h
	$(CC) -c obstaculo.cpp

lectura.o: lectura.cpp lectura.h tipos.h config.h
	$(CC) -c lectura.cpp

poligono.o: poligono.cpp poligono.h config.h
	$(CC) -c poligono.cpp

lista.o: lista.cpp lista.h
	$(CC) -c lista.cpp

clean:
	rm *.o
