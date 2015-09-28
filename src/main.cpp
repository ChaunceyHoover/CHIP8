#include "main.hpp"
#include <thread>
#include <chrono>

Chip8 chip8;

static void error_callback(int err, const char* description) {
	std::cerr << "[GLFW] " << description << "\n";
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	int state = (action == GLFW_PRESS ? 1 : 0);
	switch (key) {
	case '1':
		chip8.key[0x0] = state;
		break;
	case '2':
		chip8.key[0x1] = state;
		break;
	case '3':
		chip8.key[0x2] = state;
		break;
	case 'Q':
		chip8.key[0x3] = state;
		break;
	case 'W':
		chip8.key[0x4] = state;
		break;
	case 'E':
		chip8.key[0x5] = state;
		break;
	case 'A':
		chip8.key[0x6] = state;
		break;
	case 'S':
		chip8.key[0x7] = state;
		break;
	case 'D':
		chip8.key[0x8] = state;
		break;
	case 'Z':
		chip8.key[0x9] = state;
		break;
	case 'X':
		chip8.key[0xA] = state;
		break;
	case 'C':
		chip8.key[0xB] = state;
		break;
	case '4':
		chip8.key[0xC] = state;
		break;
	case 'R':
		chip8.key[0xD] = state;
		break;
	case 'F':
		chip8.key[0xE] = state;
		break;
	case 'V':
		chip8.key[0xF] = state;
		break;
	}
}

unsigned char texture_data[32][64][3] = { { { 0 } } };

void initTexture() {
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 64, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *)texture_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glEnable(GL_TEXTURE_2D);
}

int main() {
	if (!glfwInit()) {
		std::cerr << "[Main] Unable to initialize GLFW\n";
		return 1;
	}

	glfwSetErrorCallback(error_callback);
	GLFWwindow* window = glfwCreateWindow(1024, 512, "CHIP8 Emulator", NULL, NULL);

	if (!window) {
		std::cerr << "[Main] Unable to create GLFW window.\n";
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	// OpenGL Init
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 1024, 512, 0);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, 1024, 512);

	initTexture();

	if (chip8.load_rom("PONG")) {
		std::string title = "CHIP8 - ";
		title += chip8.loaded_rom_name;
		glfwSetWindowTitle(window, title.c_str());

		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
			chip8.cycle();
			
			if (chip8.draw_flag) {
				glClear(GL_COLOR_BUFFER_BIT);
				glLoadIdentity();
				
				for (int y = 0; y < 32; ++y) {
					for (int x = 0; x < 64; ++x) {
						if (chip8.gfx[(y * 64) + x] == 0) {
							texture_data[y][x][0] = texture_data[y][x][1] = texture_data[y][x][2] = 33;
						} else {
							texture_data[y][x][0] = texture_data[y][x][1] = texture_data[y][x][2] = 204;
						}
					}
				}

				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 64, 32, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *)texture_data);
				glBegin(GL_QUADS);
					glTexCoord2d(0.0, 0.0); glVertex2d(0.0, 0.0);
					glTexCoord2d(1.0, 0.0); glVertex2d(1024.0, 0.0);
					glTexCoord2d(1.0, 1.0); glVertex2d(1024.0, 512.0);
					glTexCoord2d(0.0, 1.0); glVertex2d(0.0, 512.0);
				glEnd();

				glfwSwapBuffers(window);
				chip8.draw_flag = false;
			}

			std::this_thread::sleep_for(std::chrono::microseconds(1200));
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}