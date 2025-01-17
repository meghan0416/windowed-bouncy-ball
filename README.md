# Windowed Bouncy Ball
![](https://github.com/meghan0416/windowed-bouncy-ball/blob/main/ScreenRecording2025-01-17at4.56.51PM-ezgif.com-video-to-gif-converter.gif?raw=true)

Instructions to compile and run on MacOS:

## Install Dependencies

Use the following commands in Terminal:

		brew install glfw
		brew install glm
		brew install glew

If any issues compiling, try adding them to PATH manually:

		sudo nano /etc/paths

## Compiling

In the provided Makefile, change the include paths and library paths to your system paths if needed.
If you have an Intel Mac, the path for brew installs will be different from mine.

Use the following command in Terminal to compile:

		make

## Running

You may optionally set a new radius, radius and window size, or radius, window size, and gravity setting. By default, the radius is 40 with window size 300 x 300 and gravity set to true.

Run with 	

		./WindowedBouncyBall
		./WindowedBouncyBall <radius>
		./WindowedBouncyBall <radius> <width> <height>
		./WindowedBouncyBall <radius> <width> <height> <gravity (0 or 1)>

The zero gravity ball will only move if it collides with the window.







