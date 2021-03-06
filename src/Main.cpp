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
#ifdef TEST_MY_GLTEXT
#include <gltext/Font.h>
#include <gltext/Text.h>
#else
#include <gltext.hpp>
#endif
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
    int result = EXIT_SUCCESS;
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
    std::cout << "window (" << width << " x " << height << ")\n";

    try {
        // Load a TrueType font, specifying its point size, and the texture cache size
#ifdef TEST_MY_GLTEXT
        // Load a TrueType font, specifying its point size, and the characters cache size
        gltext::Font font("data/DroidSans.ttf", 40, 81);
        // Testing optimizing cache by rendering small characters before bigger ones
        font.cache(",._'^\"-+=*<>: ");
        font.cache("acenmorsuvwxz");
        font.cache("lbdfitghkpqy");
        font.cache("éèêàîïü;");
        font.cache("0123456789");
        font.cache("ABCDEFGHIKLMNOPRSTUVWXYZ");
        font.cache("!?/\\@#%$&€()[]{}");
        font.cache("jJQ|");
        gltext::Text text = font.assemble("Sébastien Rombauts");
#else
        gltext::Font font("data/DroidSans.ttf", 32, 512, 512);
        // specify the screen size for perfect pixel rendering
        font.setDisplaySize(width, height);
        // NOTE: cache is hard to use properly; you need to calculate the right cache size in the above gltext::Font() constructor
        font.cacheCharacters("1234567890!@#$%^&*()abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,./;'[]\\<>?:\"{}|-=_+");
#endif

        while (!glfwWindowShouldClose(window)) {
            // clear the buffer with a color
         // glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // black background
            glClearColor(0.0f, 0.0f, 0.8f, 1.0f); // blue background
         // glClearColor(1.0f, 0.0f, 1.0f, 1.0f); // white: can show problem with font transparency (alpha blending)
            glClear(GL_COLOR_BUFFER_BIT);

            // Face Culling : We use the OpenGL default Counter Clockwise Winding order (GL_CCW)
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);

            // Enable and configure blending for font antialiasing and transparency
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

            // Draw some text with the loaded font
#ifdef TEST_MY_GLTEXT
            font.drawCache(0.0f, 0.0f, 256.0f/width, 256.0f/height);

         // text.setPosition(16, 16, 0);
            text.draw();
#else
            font.setPenPosition(16, 16);
            font.draw("Hello, gltext!");
#endif        
            // Swap back & front buffers
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    } catch (std::exception& e) {
        std::cout << "gltext::Font: exception '" << e.what() << "'\n";
        result = EXIT_FAILURE;
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return result;
}
