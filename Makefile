FLAGS = -I/usr/include/GL -g 
LIBS = -L/usr/include -lglut -lglui -lGLU -lGL -lm
CPP=g++
APPS = scene

all: $(APPS)

fish: Fish.h Fish.cpp
	$(CPP) -c Fish.cpp -o Fish.o $(FLAGS) $(LIBS)

scene: Scene.h Scene.cpp fish
	$(CPP) Scene.cpp Fish.o -o $(APPS) $(FLAGS) $(LIBS)

clean:
	@rm -rf $(APPS)
	@rm -rf *.o
