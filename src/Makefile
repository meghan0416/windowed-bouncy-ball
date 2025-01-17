# Makefile for Windowed Bouncy Ball
# Author: Meghan Grayson
# Date: December 1, 2024
# **********************************
# Variables
CC = g++
CCFLAGS = -Wall -g
INCLUDES = -I /opt/homebrew/Cellar/glew/2.2.0_1/include -I /opt/homebrew/Cellar/glfw/3.4/include -I /opt/homebrew/Cellar/glm/1.0.1/include # Change include paths as needed
LIBRARY_PATHS = -L /opt/homebrew/Cellar/glew/2.2.0_1/lib -L /opt/homebrew/Cellar/glfw/3.4/lib -L /opt/homebrew/Cellar/glm/1.0.1/lib # Change library paths as needed
LIBRARIES = -lGLEW -lglfw -framework OpenGL
EXECUTABLES = WindowedBouncyBall

WindowedBouncyBall: main.o Circle.o
	$(CC) $(CCFLAGS) $(INCLUDES) -o WindowedBouncyBall main.o Circle.o $(LIBRARY_PATHS) $(LIBRARIES)

Circle.o: Circle.hpp

# Rule for generating .o file from .cpp file
%.o: %.cpp
	$(CC) $(CCFLAGS) $(INCLUDES) -c $^ 

# All files to be generated
all: WindowedBouncyBall

# Clean the directory
clean: 
	rm -rf $(EXECUTABLES) *.o *.gch
