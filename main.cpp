#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
#include "bmp/bmp.h"
#include <list>
#include <string>

#define HEIGHT 1000
#define WIDTH 1000

#define DT 1

#define COMPLEX_OPTI 1

typedef long double ld;

int nb_its = 100;
BMP_Picture img;

std::list<std::string> pics_titles;

typedef struct{
	char key[512];

	char& operator[](int index){
		if(index > 127)
			return key[index - 1073741824 + 128];
		else return key[index];
	}

} Input;

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

struct Complex {
	ld x,y;

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


void calculate_frame(Complex a, Complex b, bool recalculate){
	printf("calculating...\n");
	#if COMPLEX_OPTI == 0
	Complex z0 = {0, 0}, z = {0, 0};
	#else
	ld x = 0, y = 0, x2 = 0, y2 = 0, x0 = 0, y0 = 0;
	#endif
	int n = 0;
	for(int i(0); i<HEIGHT; i++){
		for(int j(0); j<WIDTH; j++){
			if(!recalculate || (img(WIDTH-j-1, i).get_r() == 0 && img(WIDTH-j-1, i).get_g() == 0 && img(WIDTH-j-1, i).get_b() == 0)){
				n = 0;

				#if COMPLEX_OPTI == 0
				z0.x = a.x + i*(b.x - a.x)/HEIGHT; z.x = z0.x;
				z0.y = a.y + j*(b.y - a.y)/WIDTH; z.y = z0.y;

				while(n++ < nb_its && z.abs2() <= 4){
					z.evo(z0);
				}

				n--;
				#else
				x = a.x + i*(b.x - a.x)/HEIGHT; x0 = x;
				y = a.y + j*(b.y - a.y)/WIDTH; y0 = y;

				x2 = x*x;
				y2 = y*y;

				while(x2 + y2 <= 4 && n++ < nb_its){
					y =	(x+x)*y + y0;
					x =	x2 - y2	+ x0;
					x2 = x*x;
					y2 = y*y;
				}

				#endif
				//cout << n << endl;

				if(n >= nb_its-1){
					img(WIDTH-j-1, i).set_c(0,0,0);
				}
				else{

					ld h = fmod(sqrt(n)*20, 360L);
					ld s = .55;
					ld l = .7;

					ld c = (1 - abs(2*l - 1)) * s;
					ld x = c*(1 - abs(fmod(h/60, 2L) - 1));
					ld m = l-c/2;

					if(h <= 60) img(WIDTH-j-1,i).set_c((int) ((c+m)*255), (int) ((x+m)*255), (int) (m*255));
					else if (h <= 120) img(WIDTH-j-1,i).set_c((int) ((x+m)*255), (int) ((c+m)*255), (int) (m*255));
					else if (h <= 180) img(WIDTH-j-1,i).set_c((int) (m*255), (int) ((c+m)*255), (int) ((x+m)*255));
					else if (h <= 240) img(WIDTH-j-1,i).set_c((int) (m*255), (int) ((x+m)*255), (int) ((c+m)*255));
					else if (h <= 300) img(WIDTH-j-1,i).set_c((int) ((x+m)*255), (int) (m*255), (int) ((c+m)*255));
					else img(WIDTH-j-1,i).set_c((int) ((c+m)*255), (int) (m*255), (int) ((x+m)*255));
				}
			}
		}
	}
	pics_titles.push_front(std::string(std::to_string(a.x) + "," + std::to_string(a.y) +"," + std::to_string(b.x) + "," + std::to_string(b.y) + ",.bmp"));

	std::string tmp = "/home/charles/Documents/misc_alg/cpp_train/mandel/pics/" + pics_titles.front();

	img.save_BMP(tmp);
	std::cout << "Saved picture as " << pics_titles.front() << "\n";

	printf("done\n");
}

void update_screen(SDL_Window *win, SDL_Surface *ren_surface){
	std::string pix_title = "/home/charles/Documents/misc_alg/cpp_train/mandel/pics/" + pics_titles.front();

	SDL_Surface *to_plot_img = SDL_LoadBMP(pix_title.c_str());

	if(!to_plot_img)
		printf("failed load bmp\n");

	int result = SDL_BlitSurface(to_plot_img, NULL, ren_surface, NULL);
	SDL_UpdateWindowSurface(win);

	if(result < 0)
		printf("failed blit image\n");

	SDL_FreeSurface(to_plot_img);
}

bool revert_frame(Complex& a, Complex& b){
	if(pics_titles.size() <= 1)
		return false;

	pics_titles.pop_front();
	std::string tmp = pics_titles.front();


	int cur_pos(0), nxt_pos(tmp.find(","));

	a.x = stold(tmp.substr(cur_pos, nxt_pos));

	cur_pos = nxt_pos+1;
	nxt_pos = tmp.find(",", cur_pos);

	a.y = stold(tmp.substr(cur_pos, nxt_pos));

	cur_pos = nxt_pos+1;
	nxt_pos = tmp.find(",", cur_pos);

	b.x = stold(tmp.substr(cur_pos, nxt_pos));

	cur_pos = nxt_pos+1;
	nxt_pos = tmp.find(",", cur_pos);

	b.y = stold(tmp.substr(cur_pos, nxt_pos));

	return true;
}

bool zoom(Complex &a, Complex &b, int sto_x, int sto_y, int mouse_x, int mouse_y){
	if(mouse_x != sto_x && mouse_y != sto_y){
		int a_x = std::min(mouse_x, sto_x);
		int a_y = std::min(mouse_y, sto_y);
		int b_x = std::max(mouse_x, sto_x);
		int b_y = std::max(mouse_y, sto_y);

		if((b_x - a_x)*WIDTH > (b_y - a_y)*HEIGHT){
			int const bary = (a_y + b_y)/2;
			a_y = bary - (b_x - a_x)/2; //TODO : mult par un HEIGHT/WIDTH
			b_y = bary + (b_x - a_x)/2;
		}
		else{
			int const bary = (a_x + b_x)/2;
			a_x = bary - (b_y - a_y)/2; //TODO : idem qu'au dessus
			b_x = bary + (b_y - a_y)/2;
		}


		ld const tmp_ax(a.x), tmp_ay(a.y), tmp_bx(b.x), tmp_by(b.y);

		a.x += a_x * (tmp_bx - tmp_ax)/HEIGHT;
		b.x -= (HEIGHT - b_x) * (tmp_bx - tmp_ax)/HEIGHT;

		a.y += (WIDTH - b_y) * (tmp_by - tmp_ay)/WIDTH;
		b.y -= a_y * (tmp_by - tmp_ay)/WIDTH;

		return true;
	}

	return false;
}


int main(int argc, char* argv[]){
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error initializing SDL: %s\n", SDL_GetError());
	}

	Input in;
	memset(&in, 0, sizeof(in));

	SDL_Window *win = SDL_CreateWindow("Mandelbrot", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, HEIGHT, WIDTH, SDL_WINDOW_RESIZABLE);

	SDL_Surface *ren_surface = SDL_GetWindowSurface(win);

	int mouse_x(0), mouse_y(0); //coordonnées de la souris

	SDL_Event events;

	//a is bottom left, b if top right
	Complex a = { -2, -2}, b = { 2, 2};

	int sto_x(0), sto_y(0);

	img = BMP_Picture(WIDTH, HEIGHT);
	calculate_frame(a, b, false);
	update_screen(win, ren_surface);

	while(SDL_WaitEvent(&events)){
		switch(events.type){

			case SDL_QUIT:
				SDL_FreeSurface(ren_surface);
				SDL_DestroyWindow(win);
				SDL_Quit();
				printf("Closing !\n");
				break;

			case SDL_KEYDOWN:
				in[events.key.keysym.sym] = 1;
				break;

			case SDL_KEYUP:
				if(in[SDLK_ESCAPE] == 1){
					SDL_FreeSurface(ren_surface);
					SDL_DestroyWindow(win);
					SDL_Quit();
					printf("Closing !\n");
					return 0;
				}
				if(in[SDLK_p] == 1){
					printf("New number of iterations (press Enter when done) :\n");
					nb_its = pool_int();
					calculate_frame(a, b, true);
					update_screen(win, ren_surface);
				}
				if(in[SDLK_k] == 1){
					SDL_FreeSurface(ren_surface);
					SDL_DestroyWindow(win);
					SDL_Quit();
					printf("Closing !\n");
					return 0;
				}
				if(in[SDLK_b] == 1){
					printf("going back !\n");
					if(revert_frame(a, b))
						update_screen(win, ren_surface);
					else
						std::cout << "Can't go back !!\n";
				}
				in[events.key.keysym.sym] = 0;
				break;

			case SDL_MOUSEMOTION:
				mouse_x = events.motion.x;
				mouse_y = events.motion.y;
				//cout << mouse_x << " " << mouse_y << endl;
				break;

			case SDL_MOUSEBUTTONDOWN:
				//printf("+clic\n");
				sto_x = events.motion.x;
				sto_y = events.motion.y;
				break;

			case SDL_MOUSEBUTTONUP:
				//printf("-clic\n");
				if(zoom(a, b, sto_x, sto_y, mouse_x, mouse_y)){
					calculate_frame(a, b, false);
					update_screen(win, ren_surface);
				}
				break;
		}
	}

	return 0;
}
