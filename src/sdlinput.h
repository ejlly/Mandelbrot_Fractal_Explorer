#ifndef SDLINPUT_H
#define SDLINPUT_H

#include <SDL2/SDL.h>
#include <list>
#include <string>
#include <ctime>

#include "params.h"
#include "bmp.h"


typedef double ld;

struct Complex {
	ld x,y;

	Complex(){
		x = 0;
		y = 0;
	}

	Complex(ld x_coord, ld y_coord){
		x = x_coord;
		y = y_coord;
	}

	Complex operator+(Complex &z){
		return Complex(x+z.x, y+z.y);
	}

	//Opé optimisée pour Mandelbrot
	void evo(Complex a){
		ld const tmpx = x;
		x = x*x - y*y + a.x;
		y = 2*tmpx*y + a.y;
	}

	ld abs2(){
		//module carré
		return x*x+y*y;
	}
};

struct Input{
	char key[512];

	char& operator[](int index){
		if(index > 127)
			return key[index - 1073741824 + 128];
		else return key[index];
	}

};

struct SDL_objs{
	SDL_Window *win;
	SDL_Surface *ren_surface;
	SDL_Event events;
	Input in;
};

struct Mem_pics{
	int pics_count;
	int nb_its = 100;
	BMP_Picture img;
	Complex a, b;
	std::list<std::string> pics_titles;
	std::list<Complex> values_record;
	char* date;
	Complex origin;
};

int pool_int();

void init_mem_pics(Mem_pics &memory);

void calculate_frame(Mem_pics &memory, bool recalculate, bool drawsJulia);

void update_screen(SDL_objs &sdl_objs, Mem_pics const& memory);

bool revert_frame(Mem_pics &memory);

bool zoom(Mem_pics &memory, int sto_x, int sto_y, int mouse_x, int mouse_y);

void init_SDL_objs(SDL_objs &sdl_objs);

void kill_sdl(SDL_objs &sdl_objs);

bool catch_julia_press(SDL_objs &sdl_objs, Complex &origin);

bool main_loop(SDL_objs &sdl_objs, Complex &origin, bool drawsJulia);


#endif /* SDLINPUT_H */
