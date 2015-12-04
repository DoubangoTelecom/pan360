// jpeg_load.cpp : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include <cstdlib>
#include <stdint.h>

#include <pan360/p360_api.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define JPEG_FILENAME "./jpeg_224x224.jpg" // "./jpeg_690x690.jpg"

static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void window_size_callback(GLFWwindow* window, int width, int height);

static GLFWwindow* s_pWindow = nullptr;
static uint64_t s_uFrameNum = 0;

int _tmain(int argc, _TCHAR* argv[])
{
    printf("tests/jpeg_load\n\n");

    // Set callback function
    glfwSetErrorCallback(error_callback);

    /* Initialize PAN360 library */
    if (P360_ERROR_CODE_IS_NOK(P360Init())) {
        exit(EXIT_FAILURE);
    }

    /* Initialize GLFW library */
    if (glfwInit() != GL_TRUE) {
        exit(EXIT_FAILURE);
    }

    // Create window
    s_pWindow = glfwCreateWindow(640, 480, "tests/jpeg_load", NULL, NULL);
    if (!s_pWindow) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(s_pWindow);
    glfwSwapInterval(1);
    glfwSetKeyCallback(s_pWindow, key_callback);
    glfwSetWindowSizeCallback(s_pWindow, window_size_callback);

    // Init GLEW after setting the current GL context
    GLenum glewErr = glewInit();
    if (GLEW_OK != glewErr) {
        P360_DEBUG_ERROR("Error: %s", glewGetErrorString(glewErr));
        exit(EXIT_FAILURE);
    }


    // Default background color
    glClearColor(0.f, 0.f, 0.f, 1.f);

    // Decode the jpeg image
    P360ObjWrapper<P360Image*> image;
    if (P360_ERROR_CODE_IS_NOK(P360ImageDecoder::decodeFile(JPEG_FILENAME, &image))) {
        exit(EXIT_FAILURE);
    }
    // for now only RGB24 is supported
    if (image->getPixelFormat() != P360_PIXEL_FORMAT_R8G8B8) {
        exit(EXIT_FAILURE);
    }

    // Build 2D texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)image->getWidth(), (GLsizei)image->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->getDataPtr());

    glBindTexture(GL_TEXTURE_2D, 0);

    // Endless loop
    while (!glfwWindowShouldClose(s_pWindow)) {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(s_pWindow, &width, &height);

        ratio = width / (float)height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, textureID);
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, width, height, 0); // same as glOrtho(0, width, height, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glEnable(GL_TEXTURE_2D);
        if ((image->getWidth() & 3)) { // multiple of 4?
            glPixelStorei(GL_UNPACK_ROW_LENGTH, (GLint)image->getWidth());
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        }

        glBegin(GL_QUADS);
        glTexCoord2i(0, 0);
        glVertex2i(0, 0);
        glTexCoord2i(0, 1);
        glVertex2i(0, height);
        glTexCoord2i(1, 1);
        glVertex2i(width, height);
        glTexCoord2i(1, 0);
        glVertex2i(width, 0);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        glfwSwapBuffers(s_pWindow);
        glfwPollEvents();
    }

    /* Free resources */

    glfwDestroyWindow(s_pWindow);

    glfwTerminate();
    return EXIT_SUCCESS;
}

static void window_size_callback(GLFWwindow* window, int width, int height)
{

}

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
