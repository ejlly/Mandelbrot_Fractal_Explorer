#include <SDL2/SDL.h>
#include "sdlinput.h"


int pool_int(){
	int k = 0;
	SDL_Event event;
	while(SDL_WaitEvent(&event)){
		switch(event.type){
			case SDL_KEYUP:
				switch(event.key.keysym.sym){
					case SDLK_RETURN:
						printf("\n");
						return k;
					case SDLK_0:
						printf("0");
						fflush(stdout);
						k = 10*k;
						break;
					case SDLK_1:
						printf("1");
						fflush(stdout);
						k = 10*k + 1;
						break;
					case SDLK_2:
						printf("2");
						fflush(stdout);
						k = 10*k + 2;
						break;
					case SDLK_3:
						printf("3");
						fflush(stdout);
						k = 10*k + 3;
						break;
					case SDLK_4:
						printf("4");
						fflush(stdout);
						k = 10*k + 4;
						break;
					case SDLK_5:
						printf("5");
						fflush(stdout);
						k = 10*k + 5;
						break;
					case SDLK_6:
						printf("6");
						fflush(stdout);
						k = 10*k + 6;
						break;
					case SDLK_7:
						printf("7");
						fflush(stdout);
						k = 10*k + 7;
						break;
					case SDLK_8:
						printf("8");
						fflush(stdout);
						k = 10*k + 8;
						break;
					case SDLK_9:
						printf("9");
						fflush(stdout);
						k = 10*k + 9;
						break;
					case SDLK_BACKSPACE:
						printf("\033[1D \033[1D");
						fflush(stdout);
						k /= 10;
						break;
				}
				break;
		}
	}
	return k;
}
