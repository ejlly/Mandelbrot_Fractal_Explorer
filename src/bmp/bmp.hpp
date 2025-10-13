#ifndef BMP_HEADER_PERSO
#define BMP_HEADER_PERSO

#include <vector>
#include <string>

typedef unsigned char uc;

class Pixel{
	//RGBA
	private:
		uc r,g,b;

	public:

		Pixel();
		Pixel(uc p_r, uc p_g, uc p_b);

		uc get_r() const;
		uc get_g() const;
		uc get_b() const;

		void set_r(uc p_r);
		void set_g(uc p_g);
		void set_b(uc p_b);
		void set_c(uc p_r, uc p_g, uc p_b);

};

class BMP_Picture{
	private:
		int width, height;
		std::vector<Pixel> pixels;

	public:

		BMP_Picture();
		BMP_Picture(int w, int h);
		BMP_Picture(int w, int h, std::vector<Pixel> pixels_tab);

		int get_width() const;
		int get_height() const;

		Pixel& operator()(int i, int j);
		Pixel const& operator()(int i, int j) const;
		
		void save_BMP(const char* path);
		void save_BMP(std::string const& path);
};

#endif
