#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

#include "sdlinput.h"


int main(int argc, char* argv[]){
	SDL_objs sdl_objs;
	init_SDL_objs(sdl_objs);


	Complex origin;

	if(main_loop(sdl_objs, origin, false)){
		
	}

	return 0;
}
