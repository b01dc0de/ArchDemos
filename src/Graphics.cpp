#include "Graphics.h"

namespace Arch
{
    GFXState GraphicsState;

    void GFXState::Init()
    {
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

        delete[] vxcolor_vshader_src;
        delete[] vxcolor_fshader_src;

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
        glBindVertexArray(GraphicsState.vx_array);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(Window);
    }

    void GFXState::Term()
    {

    }
}