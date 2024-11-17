#include "Engine.h"
#include "Graphics.h"

namespace Arch
{
    int Width = 1280;
    int Height = 960;
    const char* WindowTitle = "ArchDemos";
    GLFWwindow* Window = nullptr;

    int Arch_ArgCount = 0;
    const char** Arch_ArgVals = nullptr;

    static const int NumBgColors = ARRAY_SIZE(BgColors);

    void ErrorCallback(int ErrNo, const char* ErrDesc)
    {
        OutputDebugString(ErrDesc);
    }

    void KeyCallback(GLFWwindow* Window, int Key, int ScanCode, int Action, int Mods)
    {
        if (Key == GLFW_KEY_ESCAPE && Action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(Window, GLFW_TRUE);
        }
        else if (Key == GLFW_KEY_ENTER && Action == GLFW_PRESS)
        {
            GraphicsState.BgIdx = (GraphicsState.BgIdx + 1) % NumBgColors;
            GraphicsState.QuadColIdx = (GraphicsState.BgIdx + 1) % NumBgColors;
        }
    }

    bool Init(int ArgCount, const char** ArgVals)
    {
        Arch_ArgCount = ArgCount;
        Arch_ArgVals = ArgVals;

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

            GraphicsState.Init();

            glfwSwapInterval(1);
        }

        return bResult;
    }

    bool bRunning = false;
    void Update()
    {
        glfwPollEvents();
        bRunning = !glfwWindowShouldClose(Window);
    }

    bool MainLoop()
    {
        bool bResult = true;

        bRunning = true;
        while (bRunning)
        {
            GraphicsState.Draw();

            Update();
        }

        return bResult;
    }

    bool Term()
    {
        bool bResult = true;

        if (Window) { glfwDestroyWindow(Window); }
        glfwTerminate();

        return bResult;
    }
}
