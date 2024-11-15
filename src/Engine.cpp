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

    static const Color BgColors[] = {
        { 0.890f, 0.412f, 0.557f, 1.0f },
        { 0.408f, 0.369f, 0.600f, 1.0f },
        { 0.306f, 0.259f, 0.600f, 1.0f },
        { 0.125f, 0.133f, 0.267f, 1.0f },
        { 0.000f, 0.561f, 0.325f, 1.0f },
        { 0.063f, 0.388f, 0.435f, 1.0f }
    };

    struct VxColor
    {
        Vec3 Pos;
        Vec3 Col;
    };

    static const VxColor TriVxs[] =
    {
        VxColor{{-0.6f, -0.4f, 0.5f}, {1.0f, 0.0f, 0.0f}},
        VxColor{{0.6f, -0.4f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        VxColor{{0.0f, 0.6f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    };

    static const int NumBgColors = ARRAY_SIZE(BgColors);
    static int BgIdx = 0;

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

    static const char* GetBaseDirectory()
    {
        using std::string;
        static string BaseDir{};
        static const char* BaseDirName = "ArchDemos";
        if (BaseDir.empty() && Arch_ArgCount > 0)
        {
            BaseDir = string(Arch_ArgVals[0]);
            string::size_type DirPos = BaseDir.find(BaseDirName);
            if (DirPos != string::npos) { BaseDir = BaseDir.substr(0, DirPos + strlen(BaseDirName) + 1); }
            // TODO: Clean this up, this is unsafe (+ naive!) right now
        }
        return BaseDir.c_str();
    }

    static GLchar* ReadFileContents(const char* Filename)
    {
        GLchar* Result = nullptr;

        std::string FullFilename = std::string(GetBaseDirectory());
        FullFilename += Filename;

        FILE* FileHandle = nullptr;
        fopen_s(&FileHandle, FullFilename.c_str(), "rb");
        if (FileHandle)
        {
            size_t FileSize = 0;
            fseek(FileHandle, 0, SEEK_END);
            FileSize = ftell(FileHandle);
            fseek(FileHandle, 0, SEEK_SET);

            Result = (GLchar*)malloc(FileSize + 1);
            fread(Result, FileSize, 1, FileHandle);
            Result[FileSize] = '\0';

            fclose(FileHandle);
        }

        return Result;
    }

    static bool Init(int ArgCount, const char** ArgVals)
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
    static void Update()
    {
        glfwPollEvents();
        bRunning = !glfwWindowShouldClose(Window);
    }

    static bool MainLoop()
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

    static bool Term()
    {
        bool bResult = true;

        if (Window) { glfwDestroyWindow(Window); }
        glfwTerminate();

        return bResult;
    }
}
