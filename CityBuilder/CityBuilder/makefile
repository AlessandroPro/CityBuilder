# Linux (default)
EXE = main
LDFLAGS = -lGL -lGLU -lglut

# Windows (cygwin)
ifeq ($(OS), "Windows_NT")
	EXE = hello.exe
	LDFLAGS = -lopengl32 -lglu32 -lglut32
endif
# OS X
ifeq "$(shell uname)" "Darwin"
	LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT
endif

main : main.o Vector3D.o Polygon.o PrismMesh.o Building.o Spline.o Camera.o
	g++ -o assignment2 main.o Vector3D.o Polygon.o PrismMesh.o Building.o Spline.o Camera.o $(CFLAGS) $(LDFLAGS)

main.o : main.cpp Vector3D.hpp Polygon.hpp PrismMesh.hpp Building.hpp Camera.hpp
	g++ -c -w main.cpp 

Vector3D.o : Vector3D.cpp Vector3D.hpp
	g++ -c -w Vector3D.cpp 

Polygon.o : Polygon.cpp Polygon.hpp Vector3D.hpp
	g++ -c -w Polygon.cpp

Spline.o : Spline.cpp
	g++ -c -w Spline.cpp

PrismMesh.o : PrismMesh.cpp PrismMesh.hpp Polygon.hpp Spline.cpp
	g++ -c -w PrismMesh.cpp

Building.o : Building.cpp Building.hpp Polygon.hpp PrismMesh.hpp Spline.cpp
	g++ -c -w Building.cpp

Camera.o : Camera.cpp Camera.hpp Vector3D.hpp
	g++ -c -w Camera.cpp


clean:
	rm *.o*
