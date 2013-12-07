all: Trackball.cpp
	g++ -lGL -lglut -lGLU Trackball.cpp -o trackball

clean:
	$(RM) trackball
