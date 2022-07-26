# Mandelbrot Fractal Explorer

Mandelbrot explorer using SDL2

<img src="https://github.com/ejlly/Mandelbrot_Fractal_Explorer/blob/main/captures/Capture_2.png" width="334" height="223"/> <img src="https://github.com/ejlly/Mandelbrot_Fractal_Explorer/blob/main/captures/Capture_3.png" width="334" height="223"/> <img src="https://github.com/ejlly/Mandelbrot_Fractal_Explorer/blob/main/captures/Capture_6.png" width="334" height="223"/>

Allows for interactive exploration of the Mandelbrot fractal

# Controls

- Zoom in with click and drag

- Use k or ESC to kill the application

- Use b to load the previous picture

- Use p to define a new number of iterations, then type the new number and press Enter

## Julia fractals

<img src="https://github.com/ejlly/Mandelbrot_Fractal_Explorer/blob/main/captures/Capture_5.png" width="334" height="223"/> <img src="https://github.com/ejlly/Mandelbrot_Fractal_Explorer/blob/main/captures/Capture_7.png" width="334" height="223"/> <img src="https://github.com/ejlly/Mandelbrot_Fractal_Explorer/blob/main/captures/Capture_8.png" width="334" height="223"/>

### To generate the Julia fractal corresponding to a specific point you are seeing on the screen, you must :

- Press n on your keyboard

- Click on the corresponding point with your mouse

You can then navigate normally through this specific Julia set

### To revert to Mandelbrot set

Simply press n on your keyboard to revert to last Mandelbrot picture

# Installation

### Only tested on Linux

Change the SAVE_FILE constant inside the bmp/params.h file to an existing path on your machine.
You may change WIDTH and HEIGHT to fit your screen

Compile with make

Run with ./exec
