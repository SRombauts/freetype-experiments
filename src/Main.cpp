/**
 * @file    Main.cpp
 * @brief   Tests of freetype with glText under OpenGL (glfw)s.
 *
 * Copyright (c) 2014 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <GLFW/glfw3.h>
#include <ft2build.h>
#include <gltext.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

static void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main() {
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // Open a window
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    std::cout << "fullscreen (" << width << " x " << height << ")\n";

    // Load a font
    gltext::Font font;
    try {
        font = gltext::Font("data/DroidSans.ttf", 32, 512, 512);
        font.setDisplaySize(width, height);
        // NOTE: cache is hard to use properly; you need to calculate the right cache size in the above gltext::Font() constructor
        font.cacheCharacters("1234567890!@#$%^&*()abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,./;'[]\\<>?:\"{}|-=_+");
    } catch (std::exception& e) {
        std::cout << "gltext::Font: exception '" << e.what() << "'\n";
        exit(EXIT_FAILURE);
    }

    while (!glfwWindowShouldClose(window)) {
        // clear the buffer with a color
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f); // show problem with font transparancy (alpha blending)
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Enable and configure blending for font antialiasing and transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        // Draw some text with our font
        font.setPenPosition(16, 16);
        font.draw("Hello, gltext!");
        
        // Swap back & front buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
