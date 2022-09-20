# Mandelbrot Fractal Explorer

Mandelbrot explorer using SDL2

Allow for interactive exploration of the Mandelbrot fractal

# Controls

\begin{itemize}

\item Zoom in with click and drag

\item Use k or ESC to kill the application

\item Use b to load the previous picture

\item Use p to define a new number of iterations, then type the new number and press Enter

\end{itemize}

## Julia fractals

### To generate the Julia fractal corresponding to a specific point you are seeing on the screen, you must first :

- Press n on your keyboard

- Click on the corresponding point with your mouse

You can then navigate normally through this specific Julia set

### To revert to Mandelbrot set

Simply press n on your keyboard to revert to last Mandelbrot picture

# Installation

Change the SAVE_FILE constant inside the bmp/sdl_input.h file to an existing path on your machine

Compile with make
