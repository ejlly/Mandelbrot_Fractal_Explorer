#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
#include <ctime>
#include <list>
#include <string>

#include "bmp/bmp.h"
#include "sdlinput.h"

#define HEIGHT 1000
#define WIDTH 1500

#define SAVE_FILE "/home/charles/Documents/misc_alg/cpp_train/mandel/pics/"


typedef long double ld;

int nb_its = 100;
BMP_Picture img;


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

int pics_count(0);
std::list<std::string> pics_titles;
std::list<Complex> values_record;
char* date;

void calculate_frame(Complex const &a, Complex const &b, bool recalculate){
	printf("calculating...\n");
	ld x = 0, y = 0, x2 = 0, y2 = 0, x0 = 0, y0 = 0;
	int n = 0;
	for(int i(0); i<WIDTH; i++){
		for(int j(0); j<HEIGHT; j++){
			int const c_x = HEIGHT - j - 1, c_y = i;
			if(!recalculate || (img(c_x, c_y).get_r() == 0 && img(c_x, c_y).get_g() == 0 && img(c_x, c_y).get_b() == 0)){
				n = 0;

				x = a.x + i*(b.x - a.x)/WIDTH; x0 = x;
				y = a.y + j*(b.y - a.y)/HEIGHT; y0 = y;

				x2 = x*x;
				y2 = y*y;

				while(x2 + y2 <= 4 && n++ < nb_its){
					y =	(x+x)*y + y0;
					x =	x2 - y2	+ x0;
					x2 = x*x;
					y2 = y*y;
				}

				if(n >= nb_its-1)
					img(c_x, c_y).set_c(0,0,0);
				else{

					ld const h = fmod(sqrt(n)*20, 360L);
					ld const s = .55;
					ld const l = .7;

					ld const c = (1 - abs(2*l - 1)) * s;
					ld const x = c*(1 - abs(fmod(h/60, 2L) - 1));
					ld const m = l-c/2;

					if(h <= 60)        img(c_x, c_y).set_c((int) ((c+m)*255), (int) ((x+m)*255), (int) (m*255));
					else if (h <= 120) img(c_x, c_y).set_c((int) ((x+m)*255), (int) ((c+m)*255), (int) (m*255));
					else if (h <= 180) img(c_x, c_y).set_c((int) (m*255), (int) ((c+m)*255), (int) ((x+m)*255));
					else if (h <= 240) img(c_x, c_y).set_c((int) (m*255), (int) ((x+m)*255), (int) ((c+m)*255));
					else if (h <= 300) img(c_x, c_y).set_c((int) ((x+m)*255), (int) (m*255), (int) ((c+m)*255));
					else               img(c_x, c_y).set_c((int) ((c+m)*255), (int) (m*255), (int) ((x+m)*255));
				}
			}
		}
	}


	pics_titles.push_front(date  + std::to_string(pics_count++) + ".bmp");
	std::string new_save_filepath= SAVE_FILE + pics_titles.front();

	img.save_BMP(new_save_filepath);
	printf("Saved picture as %s\n", pics_titles.front().c_str());

	//printf("done\n");
}

void update_screen(SDL_Window *win, SDL_Surface *ren_surface){
	std::string pix_title = SAVE_FILE + pics_titles.front();

	SDL_Surface *to_plot_img = SDL_LoadBMP(pix_title.c_str());

	if(!to_plot_img)
		printf("Failed load bmp\n");

	int result = SDL_BlitSurface(to_plot_img, NULL, ren_surface, NULL);
	SDL_UpdateWindowSurface(win);

	if(result < 0)
		printf("Failed blit image\n");

	SDL_FreeSurface(to_plot_img);
}

bool revert_frame(Complex& a, Complex& b){
	if(pics_titles.size() <= 1)
		return false;

	pics_titles.pop_front();
	std::string tmp = pics_titles.front();

	b = values_record.front();
	values_record.pop_front();
	a = values_record.front();
	values_record.pop_front();


	return true;
}

bool zoom(Complex &a, Complex &b, int sto_x, int sto_y, int mouse_x, int mouse_y){

	values_record.push_front(a);
	values_record.push_front(b);

	if(mouse_x != sto_x && mouse_y != sto_y){
		int a_x = std::min(mouse_x, sto_x);
		int a_y = std::min(mouse_y, sto_y);
		int b_x = std::max(mouse_x, sto_x);
		int b_y = std::max(mouse_y, sto_y);

		if((b_x - a_x)*HEIGHT > (b_y - a_y)*WIDTH){
			int const bary = (a_y + b_y)/2;
			a_y = bary - (b_x - a_x)/2 * HEIGHT/WIDTH;
			b_y = bary + (b_x - a_x)/2 * HEIGHT/WIDTH;
		}
		else{
			int const bary = (a_x + b_x)/2;
			a_x = bary - (b_y - a_y)/2 * WIDTH/HEIGHT;
			b_x = bary + (b_y - a_y)/2 * WIDTH/HEIGHT;
		}


		ld const tmp_ax(a.x), tmp_ay(a.y), tmp_bx(b.x), tmp_by(b.y);

		a.x += a_x * (tmp_bx - tmp_ax)/WIDTH;
		b.x -= (WIDTH - b_x) * (tmp_bx - tmp_ax)/WIDTH;

		a.y += (HEIGHT - b_y) * (tmp_by - tmp_ay)/HEIGHT;
		b.y -= a_y * (tmp_by - tmp_ay)/HEIGHT;

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

	SDL_Window *win = SDL_CreateWindow("Mandelbrot", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);

	SDL_Surface *ren_surface = SDL_GetWindowSurface(win);

	int mouse_x(0), mouse_y(0); //coordonnées de la souris

	SDL_Event events;

	time_t const cur_time = time(NULL);
	date = asctime(localtime(&cur_time)); 
	date[19] = ',';
	date[20] = '\0';
	date = date + 4;

	//a is bottom left, b if top right
	#if (WIDTH >= HEIGHT)
	Complex a = {-2.*WIDTH/HEIGHT, -2}, b = {2.*WIDTH/HEIGHT, 2};
	#else
	Complex a = {-2, -2.*HEIGHT/WIDTH}, b = {2, 2.*HEIGHT/WIDTH};
	#endif


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
					printf("Loading previous picture\n");
					if(revert_frame(a, b))
						update_screen(win, ren_surface);
					else
						printf("Can't go back !!\n");
				}
				in[events.key.keysym.sym] = 0;
				break;

			case SDL_MOUSEBUTTONDOWN:
			//	printf("+clic\n");
				sto_x = events.motion.x;
				sto_y = events.motion.y;
				break;

			case SDL_MOUSEMOTION:
				mouse_x = events.motion.x;
				mouse_y = events.motion.y;
				//cout << mouse_x << " " << mouse_y << endl;
				break;

			case SDL_MOUSEBUTTONUP:
			//	printf("-clic\n");
				if(zoom(a, b, sto_x, sto_y, mouse_x, mouse_y)){
					calculate_frame(a, b, false);
					update_screen(win, ren_surface);
				}
				break;
		}
	}

	return 0;
}
