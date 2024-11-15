#include "Graphics.h"

namespace Arch
{
    GFXState GraphicsState;

    struct VxColor
    {
        Vec3 Pos;
        Vec3 Col;
    };

    static const float DefaultZ = 0.5f;

    static const VxColor TriVxs[] =
    {
        VxColor{{-0.6f, -0.4f, DefaultZ}, {1.0f, 0.0f, 0.0f}},
        VxColor{{0.6f, -0.4f, DefaultZ}, {0.0f, 1.0f, 0.0f}},
        VxColor{{0.0f, 0.6f, DefaultZ}, {0.0f, 0.0f, 1.0f}},
    };

    static const VxColor QuadVxs[] =
    {
        VxColor{{-0.5f, -0.5f, DefaultZ}, {1.0f, 0.0f, 0.0f}},
        VxColor{{0.5f, -0.5f, DefaultZ}, {0.0f, 1.0f, 0.0f}},
        VxColor{{0.5f, 0.5f, DefaultZ}, {0.0f, 0.0f, 1.0f}},
        VxColor{{-0.5f, 0.5f, DefaultZ}, {1.0f, 1.0f, 1.0f}},
        // TODO: Remove these dupes
        VxColor{{-0.5f, -0.5f, DefaultZ}, {0.0f, 1.0f, 1.0f}},
        VxColor{{0.5f, 0.5f, DefaultZ}, {1.0f, 1.0f, 0.0f}},
    };

    void PipelineState::Init(const char* VertexSrcFilename, const char* FragmentSrcFilename)
    {
        GLchar* vshader_src = (GLchar*)ReadFileContents(VertexSrcFilename);
        GLchar* fshader_src = (GLchar*)ReadFileContents(FragmentSrcFilename);

        vshader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vshader, 1, &vshader_src, nullptr);
        glCompileShader(vshader);

        fshader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fshader, 1, &fshader_src, nullptr);
        glCompileShader(fshader);

        program = glCreateProgram();
        glAttachShader(program, vshader);
        glAttachShader(program, fshader);
        glLinkProgram(program);

        delete[] vshader_src;
        delete[] fshader_src;
    }

    void GFXState::Init()
    {
        vxcolor.Init("src/glsl/vxcolor_v.glsl", "src/glsl/vxcolor_f.glsl");
        //vxunicolor.Init("src/glsl/vxunicolor_v.glsl", "src/glsl/vxunicolor_f.glsl");

        mvp_loc = glGetUniformLocation(vxcolor.program, "MVP");
        vpos_loc = glGetAttribLocation(vxcolor.program, "vPos");
        vcol_loc = glGetAttribLocation(vxcolor.program, "vCol");

        { // Tri
            glGenBuffers(1, &GraphicsState.tri_vx_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, GraphicsState.tri_vx_buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(TriVxs), TriVxs, GL_STATIC_DRAW);
            glGenVertexArrays(1, &GraphicsState.tri_vx_array);
            glBindVertexArray(GraphicsState.tri_vx_array);

            glEnableVertexAttribArray(GraphicsState.vpos_loc);
            glVertexAttribPointer(GraphicsState.vpos_loc, 3, GL_FLOAT, GL_FALSE, sizeof(VxColor), (void*)offsetof(VxColor, Pos));
            glEnableVertexAttribArray(GraphicsState.vcol_loc);
            glVertexAttribPointer(GraphicsState.vcol_loc, 3, GL_FLOAT, GL_FALSE, sizeof(VxColor), (void*)offsetof(VxColor, Col));
        }

        { // Quad
            glGenBuffers(1, &GraphicsState.quad_vx_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, GraphicsState.quad_vx_buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVxs), QuadVxs, GL_STATIC_DRAW);
            glGenVertexArrays(1, &GraphicsState.quad_vx_array);
            glBindVertexArray(GraphicsState.quad_vx_array);

            glEnableVertexAttribArray(GraphicsState.vpos_loc);
            glVertexAttribPointer(GraphicsState.vpos_loc, 3, GL_FLOAT, GL_FALSE, sizeof(VxColor), (void*)offsetof(VxColor, Pos));
            glEnableVertexAttribArray(GraphicsState.vcol_loc);
            glVertexAttribPointer(GraphicsState.vcol_loc, 3, GL_FLOAT, GL_FALSE, sizeof(VxColor), (void*)offsetof(VxColor, Col));
        }
    }

    void GFXState::Draw()
    {
        int FrameWidth = 0, FrameHeight = 0;
        glfwGetFramebufferSize(Window, &FrameWidth, &FrameHeight);
        const float AspectRatio = (float)FrameWidth / (float)FrameHeight;
        glViewport(0, 0, FrameWidth, FrameHeight);

        glClearColor(BgColors[BgIdx].R, BgColors[BgIdx].G, BgColors[BgIdx].B, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        HMM_Mat4 MVP = HMM_M4D(1.0f);

        glUseProgram(GraphicsState.vxcolor.program);
        glUniformMatrix4fv(GraphicsState.mvp_loc, 1, GL_FALSE, (const GLfloat*)&MVP);

        glBindVertexArray(GraphicsState.tri_vx_array);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(GraphicsState.quad_vx_array);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(Window);
    }

    void GFXState::Term()
    {

    }
}