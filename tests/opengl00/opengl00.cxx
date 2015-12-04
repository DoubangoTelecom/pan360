#include <windows.h>
#include "stdafx.h"

#include <cstdlib>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

static GLFWwindow* s_pWindow = nullptr;

int _tmain(int argc, _TCHAR* argv[])
{
    // Set callback function
    glfwSetErrorCallback(error_callback);

    /* Initialize the library */
    if (glfwInit() != GL_TRUE) {
        exit(EXIT_FAILURE);
    }

    s_pWindow = glfwCreateWindow(640, 480, "tests/opengl00", NULL, NULL);
    if (!s_pWindow) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(s_pWindow);
    glfwSwapInterval(1);
    glfwSetKeyCallback(s_pWindow, key_callback);

    glClearColor(0.f, 0.f, 0.f, 1.f);

    while (!glfwWindowShouldClose(s_pWindow)) {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(s_pWindow, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();


        glfwSwapBuffers(s_pWindow);
        glfwPollEvents();
    }
    glfwDestroyWindow(s_pWindow);

    glfwTerminate();
    return EXIT_SUCCESS;
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