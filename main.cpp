#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
#include "bmp/bmp.h"
#include <string>
#include <list>
#include <gmpxx.h>

#define HEIGHT 1000
#define WIDTH 1000

#define DT 1

using namespace std;

typedef long double ld;

long nb_its = 100;
mp_bitcnt_t preci = 32;
mpf_class pow2 = 1;
mp_exp_t expo = 1600;
BMP_Picture img;

list<string> pics_titles;

typedef struct{
	char key[512];
} Input;

long pool_number(){
	long k = 0;
	printf("New number of iterations (press Enter when done) :\n");
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
	mpf_class x,y;

	Complex(mpf_class x_coord, mpf_class y_coord){
		x = x_coord;
		y = y_coord;
	}
};

void update_prec(Complex& a, Complex &b){
	mpf_class tmpx(b.x - a.x, preci), tmpy(b.y - a.y, preci);
	tmpx *= pow2;
	tmpy *= pow2;

	if(tmpx < 1 || tmpy < 1){
		printf("resizing!\n");
		preci += 10;
		pow2 *= 1024;
	}
}

void calculate_frame(Complex& a, Complex& b){
	printf("calculating...\n");
	update_prec(a, b);
	mpf_class x(0, preci), y(0, preci), x2(0, preci), y2(0, preci), x0(0, preci), y0(0, preci), delta_x(0, preci), delta_y(0, preci);
	mpf_class const dx((b.x - a.x)/HEIGHT, preci), dy((b.y - a.y)/WIDTH, preci);

	long n = 0;
	for(int i(0); i<HEIGHT; i++){
		delta_x += dx;
		delta_y = 0;
		for(int j(0); j<WIDTH; j++){

			n = 0;

			delta_y += dy;

			x = a.x + delta_x; x0 = x;
			y = a.y + delta_y; y0 = y;

			x2 = x*x;
			y2 = y*y;

			while(x2 + y2 <= 4 && n++ < nb_its){
				y =	(x+x)*y + y0;
				x =	x2 - y2	+ x0;
				x2 = x*x;
				y2 = y*y;
			}

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

				if(h <= 60){
					img(WIDTH-j-1,i).set_c((int) ((c+m)*255), (int) ((x+m)*255), (int) (m*255));
				}
				else if (h <= 120){
					img(WIDTH-j-1,i).set_c((int) ((x+m)*255), (int) ((c+m)*255), (int) (m*255));
				}
				else if (h <= 180){
					img(WIDTH-j-1,i).set_c((int) (m*255), (int) ((c+m)*255), (int) ((x+m)*255));
				}
				else if (h <= 240){
					img(WIDTH-j-1,i).set_c((int) (m*255), (int) ((x+m)*255), (int) ((c+m)*255));
				}
				else if (h <= 300){
					img(WIDTH-j-1,i).set_c((int) ((x+m)*255), (int) (m*255), (int) ((c+m)*255));
				}
				else{
					img(WIDTH-j-1,i).set_c((int) ((c+m)*255), (int) (m*255), (int) ((x+m)*255));
				}
			}
		}
	}
	pics_titles.push_front(string(a.x.get_str(expo) + "," + a.y.get_str(expo) + "," + b.x.get_str(expo) + "," + b.y.get_str(expo) + "," + to_string(nb_its) + ".bmp"));
	string tmp = "pics/" + pics_titles.front();
	img.save_BMP(tmp);
	printf("done\n");
}

void recalculate_frame(Complex& a, Complex& b){
	printf("calculating...\n");
	mpf_class x(0, preci), y(0, preci), x2(0, preci), y2(0, preci), x0(0, preci), y0(0, preci), delta_x(0, preci), delta_y(0, preci);
	mpf_class const dx((b.x - a.x)/HEIGHT, preci), dy((b.y - a.y)/WIDTH, preci);

	long n = 0;
	for(int i(0); i<HEIGHT; i++){
		delta_x += dx;
		delta_y = 0;
		for(int j(0); j<WIDTH; j++){
			delta_y += dy;
			if(img(WIDTH-j-1, i).get_r() == 0 && img(WIDTH-j-1, i).get_g() == 0 && img(WIDTH-j-1, i).get_b() == 0){
				n = 0;

				x = a.x + delta_x; x0 = x;
				y = a.y + delta_y; y0 = y;

				x2 = x*x;
				y2 = y*y;

				while(x2 + y2 <= 4 && n++ < nb_its){
					y =	(x+x)*y + y0;
					x =	x2 - y2	+ x0;
					x2 = x*x;
					y2 = y*y;
				}

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

					if(h <= 60){
						img(WIDTH-j-1,i).set_c((int) ((c+m)*255), (int) ((x+m)*255), (int) (m*255));
					}
					else if (h <= 120){
						img(WIDTH-j-1,i).set_c((int) ((x+m)*255), (int) ((c+m)*255), (int) (m*255));
					}
					else if (h <= 180){
						img(WIDTH-j-1,i).set_c((int) (m*255), (int) ((c+m)*255), (int) ((x+m)*255));
					}
					else if (h <= 240){
						img(WIDTH-j-1,i).set_c((int) (m*255), (int) ((x+m)*255), (int) ((c+m)*255));
					}
					else if (h <= 300){
						img(WIDTH-j-1,i).set_c((int) ((x+m)*255), (int) (m*255), (int) ((c+m)*255));
					}
					else{
						img(WIDTH-j-1,i).set_c((int) ((c+m)*255), (int) (m*255), (int) ((x+m)*255));
					}
				}
			}
		}
	}
	pics_titles.push_front(string(a.x.get_str(expo) + "," + a.y.get_str(expo) + "," + b.x.get_str(expo) + "," + b.y.get_str(expo) + "," + to_string(nb_its) + ".bmp"));
	string tmp = "pics/" + pics_titles.front();
	img.save_BMP(tmp);
	printf("done\n");
}

void update_screen(SDL_Window *win, SDL_Surface *ren_surface){
	string pix_title = "pics/" + pics_titles.front();

	SDL_Surface *to_plot_img = SDL_LoadBMP(pix_title.c_str());

	if(!to_plot_img)
		printf("failed load bmp\n");

	int result = SDL_BlitSurface(to_plot_img, NULL, ren_surface, NULL);
	SDL_UpdateWindowSurface(win);

	if(result < 0)
		printf("failed blit image\n");

	SDL_FreeSurface(to_plot_img);
}

void revert_frame(Complex& a, Complex& b){
	pics_titles.pop_front();
	string tmp = pics_titles.front();

	a.x = mpf_class(tmp.substr(0, tmp.find(",")));
	tmp.erase(0,tmp.find(",")+1);

	a.y = mpf_class(tmp.substr(0, tmp.find(",")));
	tmp.erase(0,tmp.find(",")+1);

	b.x = mpf_class(tmp.substr(0, tmp.find(",")));
	tmp.erase(0,tmp.find(",")+1);

	b.y = mpf_class(tmp.substr(0, tmp.find(",")));
}

int main(int argc, char* argv[]){
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error initializing SDL: %s\n", SDL_GetError());
	}

	for(int i(0); i<preci-10; i++){
		pow2 *= 2;
	}

	Input in;
	memset(&in,0,sizeof(in));

	SDL_Window *win = SDL_CreateWindow("Mandelbrot Explorer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, HEIGHT, WIDTH, SDL_WINDOW_RESIZABLE);
	SDL_Surface *ren_surface = SDL_GetWindowSurface(win);

	int mouse_x(0), mouse_y(0); //coordonnées de la souris

	//SDL_Delay(DT*1000);
	SDL_Event events;

	//a is bottom left, b if top right
	Complex a = {-2, -2}, b = {2, 2};
	a.x = mpf_class(-2, preci);
	a.y = mpf_class(-2, preci);

	b.x = mpf_class(2, preci);
	b.y = mpf_class(2, preci);

	int sto_x(0), sto_y(0);

	int a_x(0), a_y(0), b_x(0), b_y(0);

	img = BMP_Picture(WIDTH, HEIGHT);
	calculate_frame(a, b);
	update_screen(win, ren_surface);

	while(SDL_WaitEvent(&events)){
		switch(events.type){

			case SDL_QUIT:
				SDL_FreeSurface(ren_surface);
				SDL_DestroyWindow(win);
				SDL_Quit();
				printf("Closing !\n");
				return 0;
				break;

			case SDL_KEYDOWN:
				if(events.key.keysym.sym < 512)
					in.key[events.key.keysym.sym] = 1;
				break;

			case SDL_KEYUP:
				if(in.key[SDLK_p] == 1){
					nb_its = pool_number();
					recalculate_frame(a, b);
					update_screen(win, ren_surface);
				}
				if(in.key[SDLK_k] == 1){
					SDL_FreeSurface(ren_surface);
					SDL_DestroyWindow(win);
					SDL_Quit();
					printf("Closing !\n");
					return 0;
				}
				if(in.key[SDLK_b] == 1){
					printf("going back !\n");
					revert_frame(a, b);
					update_screen(win, ren_surface);
				}
				if(events.key.keysym.sym < 512)
					in.key[events.key.keysym.sym] = 0;
				break;

			case SDL_MOUSEMOTION:
				mouse_x = events.motion.x;
				mouse_y = events.motion.y;
				break;

			case SDL_MOUSEBUTTONDOWN:
				//printf("+clic\n");
				sto_x = events.motion.x;
				sto_y = events.motion.y;
				break;

			case SDL_MOUSEBUTTONUP:
				//printf("-clic\n");
				if(mouse_x != sto_x && mouse_y != sto_y){
					a_x = min(mouse_x, sto_x);
					a_y = min(mouse_y, sto_y);
					b_x = max(mouse_x, sto_x);
					b_y = max(mouse_y, sto_y);

					if((b_x - a_x)*WIDTH > (b_y - a_y)*HEIGHT){
						int bary = (a_y + b_y)/2;
						a_y = bary - (b_x - a_x)/2;
						b_y = bary + (b_x - a_x)/2;
					}
					else{
						int bary = (a_x + b_x)/2;
						a_x = bary - (b_y - a_y)/2;
						b_x = bary + (b_y - a_y)/2;
					}

					mpf_class const tmp_ax(a.x), tmp_ay(a.y), tmp_bx(b.x), tmp_by(b.y);

					a.x += a_x * (tmp_bx - tmp_ax)/HEIGHT;
					b.x -= (HEIGHT - b_x) * (tmp_bx - tmp_ax)/HEIGHT;

					a.y += (WIDTH - b_y) * (tmp_by - tmp_ay)/WIDTH;
					b.y -= a_y * (tmp_by - tmp_ay)/WIDTH;

					calculate_frame(a, b);
					update_screen(win, ren_surface);
				}
				break;
		}
	}
	return 0;
}
