# Mandelbrot Fractal Explorer

Mandelbrot explorer using opengl

<img src="https://github.com/ejlly/Mandelbrot_Fractal_Explorer/blob/main/captures/Capture_2.png" width="334" height="223"/> <img src="https://github.com/ejlly/Mandelbrot_Fractal_Explorer/blob/main/captures/Capture_3.png" width="334" height="223"/> <img src="https://github.com/ejlly/Mandelbrot_Fractal_Explorer/blob/main/captures/Capture_6.png" width="334" height="223"/>

Allows for interactive exploration of the Mandelbrot fractal

# Controls

- Zoom in with click and drag

- Use k or ESC to kill the application

- Use left (or a) or right (or e) to cycle through previous picture

- Use p to define a new number of iterations, then type the new number and press Enter

- Use s to save the current picture

## Julia fractals

<img src="https://github.com/ejlly/Mandelbrot_Fractal_Explorer/blob/main/captures/Capture_5.png" width="334" height="223"/> <img src="https://github.com/ejlly/Mandelbrot_Fractal_Explorer/blob/main/captures/Capture_7.png" width="334" height="223"/> <img src="https://github.com/ejlly/Mandelbrot_Fractal_Explorer/blob/main/captures/Capture_8.png" width="334" height="223"/>

### To generate the Julia fractal corresponding to a specific point you are seeing on the screen, you must :

- Press j on your keyboard

- Click on the corresponding point with your mouse

You can then navigate normally through this specific Julia set

### To revert to Mandelbrot set

Simply go back through pictures with left or a

# Installation

### Only tested on Linux

Use cmake .
Run make command

### Params

By default, ./mandel will open a 1500 900 window
you can use -w width_integer or -h height_inbteger to change those parameters

example : ./mandel -w 1920 -h 1080
