CC := clang++

CFLAGS := `sdl2-config --libs --cflags` -O2 -Wall -lSDL2_image -fsycl
HDRS := src/sdlinput.h src/bmp.h src/params.h
SRCS := src/main.cpp src/bmp.cpp src/sdlinput.cpp

# generate names of object files
OBJDIR := objs/
OBJS := $(SRCS:.cpp=.o)

# name of executable
EXEC := gpu_accel

# default recipe
all: $(EXEC)

showfont: showfont.c Makefile
	$(CC) -o $@ $@.c $(CFLAGS) $(LIBS)

glfont: glfont.c Makefile
	$(CC) -o $@ $@.c $(CFLAGS) $(LIBS)

# recipe for building the final executable
$(EXEC): $(OBJS)  Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

$(OBJS): $(@:.o=.cpp) $(HDRS) Makefile
	$(CC) -o $@ $(@:.o=.cpp) -c $(CFLAGS)

# recipe to clean the workspace
clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean
