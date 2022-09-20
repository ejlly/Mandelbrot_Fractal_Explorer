#include "bmp.h"
#include <fstream>
#include <stdio.h>
#include <iostream>

Pixel::Pixel() : r(0), g(0), b(0) {};

Pixel::Pixel(uc p_r, uc p_g, uc p_b) :
	r(p_r), g(p_g), b(p_b) {};

uc Pixel::get_r(){return r;}
uc Pixel::get_g(){return g;}
uc Pixel::get_b(){return b;}

void Pixel::set_r(uc p_r){r = p_r;}
void Pixel::set_g(uc p_g){g = p_g;}
void Pixel::set_b(uc p_b){b = p_b;}
void Pixel::set_c(uc p_r, uc p_g, uc p_b){r = p_r; g = p_g; b = p_b;}

BMP_Picture::BMP_Picture() : width(0), height(0) {};
BMP_Picture::BMP_Picture(int w, int h) :
	width(w), height(h), pixels(sycl::malloc_shared<Pixel>(w*h, q)) {};
BMP_Picture::BMP_Picture(int w, int h, Pixel *pixels_tab) :
	width(w), height(h), pixels(pixels_tab) {};

BMP_Picture::~BMP_Picture(){
	free(pixels, q);
}

int BMP_Picture::get_width(){return width;}
int BMP_Picture::get_height(){return height;}

Pixel& BMP_Picture::operator()(int i, int j){
	if(i < 0 || i > height || j < 0 || j > width)
		throw std::invalid_argument("Out of BMP bounds");
	return pixels[i*width + j];
}

void BMP_Picture::save_BMP(const char* path){
	FILE *img;
	img = fopen(path, "wb");
	long int filesize = 4*(long int) width*(long int) height + 54;

	uc bmp_header[] = {'B', 'M', (uc) filesize, (uc) (filesize >> 8), (uc) (filesize >> 16), (uc) (filesize >> 24), 0,0,0,0,54,0,0,0,0};
	fwrite(bmp_header, 1, 14, img);

	uc dib_header[40] = {40, 0, 0, 0, (uc) width, (uc) (width >> 8), (uc) (width >> 16), (uc) (width >> 24), (uc) height, (uc) (height>>8), (uc) (height>>16), (uc) (height>>24), 1, 0, 24, 0 };

	dib_header[24] = 19;
	dib_header[25] = 11;

	dib_header[28] = 19;
	dib_header[29] = 11;

	fwrite(dib_header, 1, 40, img);

	uc bmp_pad[3] = {0, 0, 0}, cur_pixel[3] = {0, 0, 0};


	for(int i(height-1); i>=0; i--){
		for(int j(0); j<width; j++){
			Pixel cur_wrote = this->operator()(i,j);
			cur_pixel[0] = cur_wrote.get_b();
			cur_pixel[1] = cur_wrote.get_g();
			cur_pixel[2] = cur_wrote.get_r();

			fwrite(cur_pixel, 1, 3, img);
		}
		fwrite(bmp_pad, 1, (4-(width)%4)%4, img);
	}
	
	fclose(img);
}

void BMP_Picture::save_BMP(std::string &path){
	this->save_BMP(path.c_str());
}
