An example of how to draw a cube in 3D space using Glut.

Textures have been mapped to the sides, which gives the cube the visual identity of a die.

The mouse spins the cube in any direction while tied to a central axis.

Documentation on how this actually works: 

http://www.opengl.org/wiki/Object_Mouse_Trackball

To compile:

In Mac OS, if you have an nVidia GPU, compile with the following: 

g++ Trackball.cpp -o trackball -framework GLUT -I/System/Library/Frameworks/OpenGL.framework/Headers -framework OpenGLe

In Linux, compile with the provided makefile or the following: 

g++ Trackball.cpp -o trackball -lGL -lglut -lGLU

NOTE: If you have never coded in OpenGL before, you will need to run: 

sudo apt-get install freeglut3 freeglut3-dev 

sudo apt-get install binutils-gold

Thanks and enjoy!

