################################

Author: Meghan Grayson
Date: December 1, 2024

################################

Instructions to compile and run on MacOS:

==== Install Dependencies ====

Use the following commands in Terminal:

		brew install glfw
		brew install glm
		brew install glew

If any issues compiling, try adding them to PATH manually:

		sudo nano /etc/paths

==== Compiling ====

In the provided Makefile, change the include path for GLEW to your system path to GLEW.
If you have an M1/M2/... Mac, the path for brew installs will be different from mine.

Use the following command in Terminal to compile:

		make

==== Running ====

You may optionally set a new radius, radius and window size, or radius, window size, and gravity setting. By default, the radius is 40 with window size 300 x 300 and gravity set to true.

Run with 	./WindowedBouncyBall
		./WindowedBouncyBall <radius>
		./WindowedBouncyBall <radius> <width> <height>
		./WindowedBouncyBall <radius> <width> <height> <gravity (0 or 1)>

The zero gravity ball will only move if it collides with the window.







