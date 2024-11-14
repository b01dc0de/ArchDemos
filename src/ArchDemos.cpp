#define GLAD_GL_IMPLEMENTATION
#include "ArchDemos.h"

static bool bRunning = true;

static void ErrorCallback(int ErrNo, const char* ErrDesc)
{
    OutputDebugString(ErrDesc);
}

static void KeyCallback(GLFWwindow* Window, int Key, int ScanCode, int Action, int Mods)
{
    if (Key == GLFW_KEY_ESCAPE && Action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(Window, GLFW_TRUE);
    }
}

namespace Arch
{
    int Width = 1280;
    int Height = 960;
    const char* WindowTitle = "ArchDemos";
    GLFWwindow* Window = nullptr;

    static bool Init()
    {
        bool bResult = true;

        if (!glfwInit())
        {
            OutputDebugString("ERROR: glfwInit FAILED!\n");
            bResult = false;
        }

        Window = glfwCreateWindow(Width, Height, WindowTitle, nullptr, nullptr);
        if (!Window)
        {
            OutputDebugString("ERROR: glfwCreateWindow FAILED!\n");
            bResult = false;
        }
        else
        {
            // Set callbacks
            glfwSetErrorCallback(ErrorCallback);
            glfwSetKeyCallback(Window, KeyCallback);

            glfwMakeContextCurrent(Window);
            gladLoadGL(glfwGetProcAddress);

            glfwSwapInterval(1);
        }

        return bResult;
    }

    static bool MainLoop()
    {
        bool bResult = true;

        bool bRunning = true;
        while (bRunning)
        {
            glfwPollEvents();

            int FrameWidth = 0, FrameHeight = 0;
            glfwGetFramebufferSize(Window, &FrameWidth, &FrameHeight);
            glViewport(0, 0, FrameWidth, FrameHeight);
            glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(Window);

            bRunning = !glfwWindowShouldClose(Window);
        }

        return bResult;
    }

    static bool Term()
    {
        bool bResult = true;

        if (Window) { glfwDestroyWindow(Window); }
        glfwTerminate();

        return bResult;
    }
}

int main(int argc, const char* argv[])
{
    (void)argc;
    (void)argv;

    bool bResults = true;

    if (bResults &= Arch::Init())
    {
        bResults &= Arch::MainLoop();
        bResults &= Arch::Term();
    }

    return bResults ? 0 : 1;
}
