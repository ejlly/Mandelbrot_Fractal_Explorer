#include <cmath>
#include <ctime>

#include "mandel_calculation.hpp"
#include "keys.hpp"

//#define SAVE_FILE "pics/"

Plot::Plot() {
	//Generate date
	time_t const cur_time = std::time(NULL);
	date = std::asctime(std::localtime(&cur_time)); 
	date[19] = ',';
	date.erase(0, 4);
}

Plot::Plot(int width, int height) {
	Plot();

	if (width >= height) {
		bottom_left = Complex(-2.*width/height, -2);
		top_right = Complex(2.*width/height, 2);
	}
	else {
		bottom_left = Complex(-2, -2.*height/width);
		top_right = Complex(2, 2.*height/width);
	}

	//bottom_left = Complex(0, 0);
	//top_right = Complex(1, 1.5);

	img = BMP_Picture(width, height);
	img_title = date + bottom_left.to_string() + "_" + top_right.to_string() + ".bmp";
}

Plot::Plot(int width, int height, Complex const& _bottom_left, Complex const& _top_right) {
	Plot();

	bottom_left = _bottom_left;
	top_right = _top_right;

	img = BMP_Picture(width, height);
	img_title = date + bottom_left.to_string() + "_" + top_right.to_string() + ".bmp";
}

void calculate_frame(Window &glWindow, Plot &plot, bool recalculate, bool drawsJulia) {
	//printf("calculating...\n");

	int const width = glWindow.getwidth();
	int const height = glWindow.getheight();

	long double const s = .55;
	long double const l = .7;

	long double const c = (1 - abs(2*l - 1)) * s;
	long double const m = l-c/2;

	long double x = 0, y = 0, x2 = 0, y2 = 0, x0 = 0, y0 = 0;
	int n = 0;
	for(int i(0); i<width; i++){
		for(int j(0); j<height; j++){
			int const c_x = height - j - 1, c_y = i;
			if(!recalculate || (plot.img(c_x, c_y).get_r() == 0 && plot.img(c_x, c_y).get_g() == 0 && plot.img(c_x, c_y).get_b() == 0)){
				n = 0;

				x = plot.bottom_left.x + i*(plot.top_right.x - plot.bottom_left.x)/width;
				y = plot.bottom_left.y + j*(plot.top_right.y - plot.bottom_left.y)/height;
				if(!drawsJulia){
					x0 = x;
					y0 = y;
				}
				else{
					x0 = plot.origin.x;
					y0 = plot.origin.y;
				}

				x2 = x*x;
				y2 = y*y;

				while(x2 + y2 <= 4 && n++ < glWindow.get_nb_its()){
					y =	(x+x)*y + y0;
					x =	x2 - y2	+ x0;
					x2 = x*x;
					y2 = y*y;
				}

				if(n >= glWindow.get_nb_its()-1)
					plot.img(c_x, c_y).set_c(0,0,0);
				else{

					long double const h = std::fmod(std::sqrt(n)*20, 360L);

					long double const x = c*(1 - std::abs(std::fmod(h/60, 2L) - 1));

					if(h <= 60)        plot.img(c_x, c_y).set_c(static_cast<int>((c+m)*255), static_cast<int>((x+m)*255), static_cast<int>(m*255));
					else if (h <= 120) plot.img(c_x, c_y).set_c(static_cast<int>((x+m)*255), static_cast<int>((c+m)*255), static_cast<int>(m*255));
					else if (h <= 180) plot.img(c_x, c_y).set_c(static_cast<int>(m*255)    , static_cast<int>((c+m)*255), static_cast<int>((x+m)*255));
					else if (h <= 240) plot.img(c_x, c_y).set_c(static_cast<int>(m*255)    , static_cast<int>((x+m)*255), static_cast<int>((c+m)*255));
					else if (h <= 300) plot.img(c_x, c_y).set_c(static_cast<int>((x+m)*255), static_cast<int>(m*255)    , static_cast<int>((c+m)*255));
					else               plot.img(c_x, c_y).set_c(static_cast<int>((c+m)*255), static_cast<int>(m*255)    , static_cast<int>((x+m)*255));
				}
			}
		}
	}


	//if(!drawsJulia) memory.pics_titles.push_front(memory.date  + std::to_string(memory.pics_count++) + ".bmp");
	//else  memory.pics_titles.push_front(memory.date  + std::to_string(memory.pics_count++) + "(j).bmp");

	//std::string new_save_filepath = SAVE_FILE + memory.pics_titles.front();

	//memory.img.save_BMP(new_save_filepath);
	//printf("Saved picture as %s\n", memory.pics_titles.front().c_str());

	//printf("done\n");
}


bool revert_frame(std::vector<Plot> &memory){
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

/*
bool zoom(Plot &memory, int sto_x, int sto_y, int mouse_x, int mouse_y){

	Plot new_plot(memory.img.get_width(), memory.img.get_height());

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

	return false;
}
*/


bool catch_julia_press(Complex &origin) {
	// Implement using GLFW input callbacks or polling in main loop
	return false;
}