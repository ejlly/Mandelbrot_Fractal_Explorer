#ifndef BMP_HEADER_PERSO
#define BMP_HEADER_PERSO

#include <memory>
#include <string>

typedef unsigned char uc;

typedef unsigned char uc;

class Pixel{
	//RGBA
	private:
		uc r,g,b;

	public:

		Pixel();
		Pixel(uc p_r, uc p_g, uc p_b);

		uc get_r();
		uc get_g();
		uc get_b();

		void set_r(uc p_r);
		void set_g(uc p_g);
		void set_b(uc p_b);
		void set_c(uc p_r, uc p_g, uc p_b);

};

class BMP_Picture{
	private:
		int width, height;
		std::unique_ptr<Pixel[]> pixels;

	public:

		BMP_Picture();
		BMP_Picture(int w, int h);
		BMP_Picture(int w, int h, Pixel *pixels_tab);

		int get_width();
		int get_height();

		Pixel& operator()(int i, int j);
		
		void save_BMP(const char* path);
		void save_BMP(std::string &path);
};

#endif
