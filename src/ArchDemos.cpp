#define GLAD_GL_IMPLEMENTATION
#include "ArchDemos.h"

static bool bRunning = true;

namespace Arch
{
    int Width = 1280;
    int Height = 960;
    const char* WindowTitle = "ArchDemos";
    GLFWwindow* Window = nullptr;

    static void ErrorCallback(int ErrNo, const char* ErrDesc);
    static void KeyCallback(GLFWwindow* Window, int Key, int ScanCode, int Action, int Mods);

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

    struct Color
    {
        float R;
        float G;
        float B;
        float A;
    };

    static const Color BgColors[] = {
        { 0.890f, 0.412f, 0.557f, 1.0f },
        { 0.408f, 0.369f, 0.600f, 1.0f },
        { 0.306f, 0.259f, 0.600f, 1.0f },
        { 0.125f, 0.133f, 0.267f, 1.0f },
        { 0.000f, 0.561f, 0.325f, 1.0f },
        { 0.063f, 0.388f, 0.435f, 1.0f }
    };

#define ARRAY_SIZE(A) (sizeof(A) / sizeof(A[0]))
    static const int NumBgColors = ARRAY_SIZE(BgColors);
    static int BgIdx = 0;

    static bool MainLoop()
    {
        bool bResult = true;

        bool bRunning = true;
        while (bRunning)
        {
            { // Draw
                int FrameWidth = 0, FrameHeight = 0;
                glfwGetFramebufferSize(Window, &FrameWidth, &FrameHeight);
                glViewport(0, 0, FrameWidth, FrameHeight);
                glClear(GL_COLOR_BUFFER_BIT);
                glClearColor(BgColors[BgIdx].R, BgColors[BgIdx].G, BgColors[BgIdx].B, 1.0f);
                glfwSwapBuffers(Window);
            }

            { // Update
                glfwPollEvents();
                bRunning = !glfwWindowShouldClose(Window);
            }
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
        else if (Key == GLFW_KEY_ENTER && Action == GLFW_PRESS)
        {
            BgIdx = (BgIdx + 1) % NumBgColors;
        }
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
