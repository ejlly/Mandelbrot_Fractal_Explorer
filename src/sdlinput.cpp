#include "sdlinput.hpp"


int pool_int(){
	int k = 0;
  /*
	SDL_Event event; //TODO: use same SDL_Event object as in main loop
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
  */
	return k;
}


void init_mem_pics(Mem_pics &memory){
  /*
	memory.pics_count = 0;

	//Generate date
	time_t const cur_time = time(NULL);
	char *date = asctime(localtime(&cur_time)); 
	date[19] = ',';
	date[20] = '\0';
	date = date + 4;

	memory.date = date;


	//a is bottom left, b if top right
	#if (WIDTH >= HEIGHT)
	memory.a = {-2.*WIDTH/HEIGHT, -2};
	memory.b = {2.*WIDTH/HEIGHT, 2};
	#else
	memory.a = {-2, -2.*HEIGHT/WIDTH};
	memory.b = {2, 2.*HEIGHT/WIDTH};
	#endif

	memory.pics_titles.clear();
	memory.values_record.clear();


	memory.img = BMP_Picture(WIDTH, HEIGHT);
  */
}

void calculate_frame(Mem_pics &memory, bool recalculate, bool drawsJulia){
  /*
	printf("calculating...\n");
	ld x = 0, y = 0, x2 = 0, y2 = 0, x0 = 0, y0 = 0;
	int n = 0;
	for(int i(0); i<WIDTH; i++){
		for(int j(0); j<HEIGHT; j++){
			int const c_x = HEIGHT - j - 1, c_y = i;
			if(!recalculate || (memory.img(c_x, c_y).get_r() == 0 && memory.img(c_x, c_y).get_g() == 0 && memory.img(c_x, c_y).get_b() == 0)){
				n = 0;

				x = memory.a.x + i*(memory.b.x - memory.a.x)/WIDTH;
				y = memory.a.y + j*(memory.b.y - memory.a.y)/HEIGHT;
				if(!drawsJulia){
					x0 = x;
					y0 = y;
				}
				else{
					x0 = memory.origin.x;
					y0 = memory.origin.y;
				}

				x2 = x*x;
				y2 = y*y;

				while(x2 + y2 <= 4 && n++ < memory.nb_its){
					y =	(x+x)*y + y0;
					x =	x2 - y2	+ x0;
					x2 = x*x;
					y2 = y*y;
				}

				if(n >= memory.nb_its-1)
					memory.img(c_x, c_y).set_c(0,0,0);
				else{

					ld const h = fmod(sqrt(n)*20, 360L);
					ld const s = .55;
					ld const l = .7;

					ld const c = (1 - abs(2*l - 1)) * s;
					ld const x = c*(1 - abs(fmod(h/60, 2L) - 1));
					ld const m = l-c/2;

					if(h <= 60)        memory.img(c_x, c_y).set_c((int) ((c+m)*255), (int) ((x+m)*255), (int) (m*255));
					else if (h <= 120) memory.img(c_x, c_y).set_c((int) ((x+m)*255), (int) ((c+m)*255), (int) (m*255));
					else if (h <= 180) memory.img(c_x, c_y).set_c((int) (m*255), (int) ((c+m)*255), (int) ((x+m)*255));
					else if (h <= 240) memory.img(c_x, c_y).set_c((int) (m*255), (int) ((x+m)*255), (int) ((c+m)*255));
					else if (h <= 300) memory.img(c_x, c_y).set_c((int) ((x+m)*255), (int) (m*255), (int) ((c+m)*255));
					else               memory.img(c_x, c_y).set_c((int) ((c+m)*255), (int) (m*255), (int) ((x+m)*255));
				}
			}
		}
	}


	if(!drawsJulia) memory.pics_titles.push_front(memory.date  + std::to_string(memory.pics_count++) + ".bmp");
	else  memory.pics_titles.push_front(memory.date  + std::to_string(memory.pics_count++) + "(j).bmp");

	std::string new_save_filepath= SAVE_FILE + memory.pics_titles.front();

	memory.img.save_BMP(new_save_filepath);
	printf("Saved picture as %s\n", memory.pics_titles.front().c_str());

	//printf("done\n");
  */
}


void update_screen(SDL_objs &sdl_objs, Mem_pics const& memory){
  return;
  /*
	std::string pix_title = SAVE_FILE + memory.pics_titles.front();

	SDL_Surface *to_plot_img = SDL_LoadBMP(pix_title.c_str());

	if(!to_plot_img)
		printf("Failed load bmp\n");

	int result = SDL_BlitSurface(to_plot_img, NULL, sdl_objs.ren_surface, NULL);
	SDL_UpdateWindowSurface(sdl_objs.win);

	if(result < 0)
		printf("Failed blit image\n");

	SDL_FreeSurface(to_plot_img);
  */
}

bool revert_frame(Mem_pics &memory){
  /*
	if(memory.pics_titles.size() <= 1)
		return false;

	memory.pics_titles.pop_front();
	std::string tmp = memory.pics_titles.front();

	memory.b = memory.values_record.front();
	memory.values_record.pop_front();
	memory.a = memory.values_record.front();
	memory.values_record.pop_front();


  */
	return true;
}

bool zoom(Mem_pics &memory, int sto_x, int sto_y, int mouse_x, int mouse_y){
  /*
	memory.values_record.push_front(memory.a);
	memory.values_record.push_front(memory.b);

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


		ld const tmp_ax(memory.a.x), tmp_ay(memory.a.y), tmp_bx(memory.b.x), tmp_by(memory.b.y);

		memory.a.x += a_x * (tmp_bx - tmp_ax)/WIDTH;
		memory.b.x -= (WIDTH - b_x) * (tmp_bx - tmp_ax)/WIDTH;

		memory.a.y += (HEIGHT - b_y) * (tmp_by - tmp_ay)/HEIGHT;
		memory.b.y -= a_y * (tmp_by - tmp_ay)/HEIGHT;

		return true;
	}

  */
	return false;
}

void init_SDL_objs(SDL_objs &sdl_objs){
  /*
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error initializing SDL: %s\n", SDL_GetError());
	}

	sdl_objs.win = SDL_CreateWindow("Mandelbrot", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
	sdl_objs.ren_surface = SDL_GetWindowSurface(sdl_objs.win);

	memset(&sdl_objs.in, 0, sizeof(sdl_objs.in));
  */
}

void kill_sdl(SDL_objs &sdl_objs){
  /*
	SDL_FreeSurface(sdl_objs.ren_surface);
	SDL_DestroyWindow(sdl_objs.win);
	SDL_Quit();
  */
}

int catch_julia_press(SDL_objs &sdl_objs, Complex &origin, Mem_pics &memory, int mouse_x, int mouse_y){
  /*
	while(SDL_WaitEvent(&sdl_objs.events)){
		switch(sdl_objs.events.type){
			case SDL_QUIT: 
				kill_sdl(sdl_objs);
				printf("Closing !\n");
				return 2;
				break;

			case SDL_KEYDOWN:
				printf("Reverting to normal mode\n");
				return 1;
				break;

			case SDL_MOUSEMOTION:
				mouse_x = sdl_objs.events.motion.x;
				mouse_y = sdl_objs.events.motion.y;
				//cout << mouse_x << " " << mouse_y << endl;
				break;


			case SDL_MOUSEBUTTONDOWN:
				origin.x = memory.a.x + mouse_x*(memory.b.x - memory.a.x)/WIDTH;
				origin.y = memory.a.y + mouse_y*(memory.b.y - memory.a.y)/HEIGHT;
				return 0;
				break;

		}
	}
  */
	return 2;

}

bool main_loop(SDL_objs &sdl_objs, Complex &origin, bool drawsJulia){
  return true;
  /*
	//Mouse coordinates
	int mouse_x(0), mouse_y(0);
	int sto_x(0), sto_y(0);

	Mem_pics memory;
	init_mem_pics(memory);

	if(drawsJulia) memory.origin = origin;

	calculate_frame(memory, false, drawsJulia);
	update_screen(sdl_objs, memory);


	while(SDL_WaitEvent(&sdl_objs.events)){
		switch(sdl_objs.events.type){

			case SDL_QUIT:
				kill_sdl(sdl_objs);
				printf("Closing !\n");
				return false;
				break;

			case SDL_KEYDOWN:
				sdl_objs.in[sdl_objs.events.key.keysym.sym] = 1;
				break;

			case SDL_KEYUP:
				if(sdl_objs.in[SDLK_ESCAPE] || sdl_objs.in[SDLK_k]){
					kill_sdl(sdl_objs);
					printf("Closing !\n");
					return false;
				}
				if(sdl_objs.in[SDLK_p]){
					printf("New number of iterations (press Enter when done) :\n");
					memory.nb_its = pool_int();
					calculate_frame(memory, true, drawsJulia);
					update_screen(sdl_objs, memory);
				}
				if(sdl_objs.in[SDLK_b]){
					printf("Loading previous picture\n");

					if(revert_frame(memory))
						update_screen(sdl_objs, memory);
					else
						printf("Can't go back !!\n");
				}
				if(sdl_objs.in[SDLK_n]){
					sdl_objs.in[sdl_objs.events.key.keysym.sym] = 0;
					if(!drawsJulia){
						printf("Click to select new julia origin or press any other key to revert to normal mode\n");

						switch(catch_julia_press(sdl_objs, origin, memory, mouse_x, mouse_y)){
							case 0:
								if(!main_loop(sdl_objs, origin, true))
									return false;
								update_screen(sdl_objs, memory);
								break;
							case 2:
								return false;
						}

					}
					else{
						printf("Reverting to Mandelbrot\n");
						sdl_objs.in[sdl_objs.events.key.keysym.sym] = 0;
						return true;
					}

				}
				sdl_objs.in[sdl_objs.events.key.keysym.sym] = 0;
				break;

			case SDL_MOUSEBUTTONDOWN:
			//	printf("+clic\n");
				sto_x = sdl_objs.events.motion.x;
				sto_y = sdl_objs.events.motion.y;
				break;

			case SDL_MOUSEMOTION:
				mouse_x = sdl_objs.events.motion.x;
				mouse_y = sdl_objs.events.motion.y;
				//cout << mouse_x << " " << mouse_y << endl;
				break;

			case SDL_MOUSEBUTTONUP:
			//	printf("-clic\n");
				if(zoom(memory, sto_x, sto_y, mouse_x, mouse_y)){
					calculate_frame(memory, false, drawsJulia);
					update_screen(sdl_objs, memory);
				}
				break;
		}
	}
  */
	return false;
}
