FLAGS = -I/usr/include/GL -g
LIBS = -L/usr/include -lglut -lglui -lGLU -lGL -lm
CPP=g++
APPS = scene

all: $(APPS)

TextureLoader.o: TextureLoader.c TextureLoader.h
	$(CPP) -c TextureLoader.c -o TextureLoader.o $(FLAGS) $(LIBS)

Matrix.o: Matrix.cpp Matrix.h
	$(CPP) -c Matrix.cpp -o Matrix.o $(FLAGS) $(LIBS)

Fish.o: Fish.h Fish.cpp
	$(CPP) -c Fish.cpp -o Fish.o $(FLAGS) $(LIBS)

scene: Scene.h Scene.cpp Fish.o Matrix.o TextureLoader.o
	$(CPP) Scene.cpp Fish.o Matrix.o TextureLoader.o -o $(APPS) $(FLAGS) $(LIBS)

clean:
	@rm -rf $(APPS)
	@rm -rf *.o
