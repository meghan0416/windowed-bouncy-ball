# Makefile for Windowed Bouncy Ball
# Author: Meghan Grayson
# Date: December 1, 2024
# **********************************
# Variables
CC = g++
CCFLAGS = -Wall -g
INCLUDES = -I /usr/local/Cellar/glew/2.2.0_1/include/GL # Change include path for glew as needed
LIBRARIES = -lGLEW -lglfw -framework OpenGL
EXECUTABLES = WindowedBouncyBall

WindowedBouncyBall: main.o Circle.o
	$(CC) $(CCFLAGS) $(INCLUDES) -o WindowedBouncyBall main.o Circle.o $(LIBRARIES)

Circle.o: Circle.hpp

# Rule for generating .o file from .cpp file
%.o: %.cpp
	$(CC) $(CCFLAGS) $(INCLUDES) -c $^ 

# All files to be generated
all: WindowedBouncyBall

# Clean the directory
clean: 
	rm -rf $(EXECUTABLES) *.o *.gch