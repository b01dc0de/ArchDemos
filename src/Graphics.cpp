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

    void GFXState::Init()
    {
        GLchar* vxcolor_vshader_src = (GLchar*)ReadFileContents("src/glsl/vxcolor_v.glsl");
        GLchar* vxcolor_fshader_src = (GLchar*)ReadFileContents("src/glsl/vxcolor_f.glsl");

        { // VxColor
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

            delete[] vxcolor_vshader_src;
            delete[] vxcolor_fshader_src;
        }

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

        glUseProgram(GraphicsState.vxcolor_program);
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