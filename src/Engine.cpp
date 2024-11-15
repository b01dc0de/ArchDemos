#include "Engine.h"

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

    struct GFXState
    {
        GLuint vx_buffer = 0;
        GLuint vxcolor_v_shader = 0;
        GLuint vxcolor_f_shader = 0;
        GLuint vxcolor_program = 0;

        GLint mvp_loc = 0;
        GLint vpos_loc = 0;
        GLint vcol_loc = 0;
        GLuint vx_array = 0;
    } GraphicsState;

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
        FileHandle = fopen(FullFilename.c_str(), "rb");
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

            GLchar* vxcolor_vshader_src = ReadFileContents("src/glsl/vxcolor_v.glsl");
            GLchar* vxcolor_fshader_src = ReadFileContents("src/glsl/vxcolor_f.glsl");

            glGenBuffers(1, &GraphicsState.vx_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, GraphicsState.vx_buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(TriVxs), TriVxs, GL_STATIC_DRAW);

            GraphicsState.vxcolor_v_shader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(GraphicsState.vxcolor_v_shader, 1, &vxcolor_vshader_src, nullptr);
            glCompileShader(GraphicsState.vxcolor_v_shader);

            GraphicsState.vxcolor_f_shader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(GraphicsState.vxcolor_f_shader, 1, &vxcolor_fshader_src, nullptr);
            glCompileShader(GraphicsState.vxcolor_f_shader);

            GraphicsState.vxcolor_program = glCreateProgram();
            glAttachShader(GraphicsState.vxcolor_program, GraphicsState.vxcolor_v_shader);
            glAttachShader(GraphicsState.vxcolor_program, GraphicsState.vxcolor_f_shader);
            glLinkProgram(GraphicsState.vxcolor_program);

            GraphicsState.mvp_loc = glGetUniformLocation(GraphicsState.vxcolor_program, "MVP");
            GraphicsState.vpos_loc = glGetAttribLocation(GraphicsState.vxcolor_program, "vPos");
            GraphicsState.vcol_loc = glGetAttribLocation(GraphicsState.vxcolor_program, "vCol");

            glGenVertexArrays(1, &GraphicsState.vx_array);
            glBindVertexArray(GraphicsState.vx_array);
            glEnableVertexAttribArray(GraphicsState.vpos_loc);
            glVertexAttribPointer(GraphicsState.vpos_loc, 3, GL_FLOAT, GL_FALSE, sizeof(VxColor), (void*)offsetof(VxColor, Pos));
            glEnableVertexAttribArray(GraphicsState.vcol_loc);
            glVertexAttribPointer(GraphicsState.vcol_loc, 3, GL_FLOAT, GL_FALSE, sizeof(VxColor), (void*)offsetof(VxColor, Col));

            glfwSwapInterval(1);

            delete[] vxcolor_vshader_src;
            delete[] vxcolor_fshader_src;
        }

        return bResult;
    }

    static bool MainLoop()
    {
        bool bResult = true;

        bool bRunning = true;
        while (bRunning)
        {
            { // Draw
                int FrameWidth = 0, FrameHeight = 0;
                glfwGetFramebufferSize(Window, &FrameWidth, &FrameHeight);
                const float AspectRatio = (float)FrameWidth / (float)FrameHeight;
                glViewport(0, 0, FrameWidth, FrameHeight);

                glClearColor(BgColors[BgIdx].R, BgColors[BgIdx].G, BgColors[BgIdx].B, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                HMM_Mat4 MVP = HMM_M4D(1.0f);

                glUseProgram(GraphicsState.vxcolor_program);
                glUniformMatrix4fv(GraphicsState.mvp_loc, 1, GL_FALSE, (const GLfloat*)&MVP);
                glBindVertexArray(GraphicsState.vx_array);
                glDrawArrays(GL_TRIANGLES, 0, 3);

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
}
