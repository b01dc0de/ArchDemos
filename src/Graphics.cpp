#include "Graphics.h"

namespace Arch
{
    GFXState GraphicsState;

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

    static const float QuadSize = 0.5f;
    static const VxMin MinQuadVxs[] =
    {
        VxMin{-QuadSize, -QuadSize, 0.5f,},
        VxMin{QuadSize, -QuadSize, 0.5f,},
        VxMin{QuadSize, QuadSize, 0.5f},
        VxMin{-QuadSize, QuadSize, 0.5f},
        VxMin{-QuadSize, -QuadSize, 0.5f,},
        VxMin{QuadSize, QuadSize, 0.5f},
    };
    using IxT = unsigned short;
    static const IxT QuadIxs[] =
    {
        0, 1, 2,
        0, 2, 3
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
        vxunicolor.Init("src/glsl/vxunicolor_v.glsl", "src/glsl/vxunicolor_f.glsl");

        vxcolor_mvp_loc = glGetUniformLocation(vxcolor.program, "MVP");
        vxcolor_vpos_loc = glGetAttribLocation(vxcolor.program, "vPos");
        vxcolor_vcol_loc = glGetAttribLocation(vxcolor.program, "vCol");

        {
            Tri.Pos = Vec2{ -125.0f, 125.0f };
            Tri.Size = Vec2{ 100.0f, 100.0f };
            Tri.RotZ = 0.0f;

            Quad.Pos = Vec2{ 500.0f, 250.0f};
            Quad.Size = Vec2{ 200.0f, 200.0f };
            Quad.RotZ = 0.0f;

            UniQuad.Pos = Vec2{ 250.0f, -250.0f };
            UniQuad.Size = Vec2{ 50.0f, 50.0f };
            UniQuad.RotZ = 0.0f;
        }

        {
            vxunicolor_mvp_loc = glGetUniformLocation(vxunicolor.program, "MVP");
            vxunicolor_ucol_loc = glGetUniformLocation(vxunicolor.program, "uCol");
            vxunicolor_vpos_loc = glGetAttribLocation(vxunicolor.program, "vPos");

            glGenBuffers(1, &unicolor_quad_vx_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, unicolor_quad_vx_buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(MinQuadVxs), MinQuadVxs, GL_STATIC_DRAW);
            glGenVertexArrays(1, &unicolor_quad_vx_array);
            glBindVertexArray(unicolor_quad_vx_array);

            glEnableVertexAttribArray(vxunicolor_vpos_loc);
            glVertexAttribPointer(vxunicolor_vpos_loc, 3, GL_FLOAT, GL_FALSE, sizeof(VxMin), (void*)offsetof(VxMin, Pos));

            glBindVertexArray(0);
        }

        { // Tri
            glGenBuffers(1, &GraphicsState.tri_vx_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, GraphicsState.tri_vx_buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(TriVxs), TriVxs, GL_STATIC_DRAW);
            glGenVertexArrays(1, &GraphicsState.tri_vx_array);
            glBindVertexArray(GraphicsState.tri_vx_array);

            glEnableVertexAttribArray(GraphicsState.vxcolor_vpos_loc);
            glVertexAttribPointer(GraphicsState.vxcolor_vpos_loc, 3, GL_FLOAT, GL_FALSE, sizeof(VxColor), (void*)offsetof(VxColor, Pos));
            glEnableVertexAttribArray(GraphicsState.vxcolor_vcol_loc);
            glVertexAttribPointer(GraphicsState.vxcolor_vcol_loc, 3, GL_FLOAT, GL_FALSE, sizeof(VxColor), (void*)offsetof(VxColor, Col));

            glBindVertexArray(0);
        }

        { // Quad
            glGenBuffers(1, &GraphicsState.quad_vx_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, GraphicsState.quad_vx_buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVxs), QuadVxs, GL_STATIC_DRAW);
            glGenVertexArrays(1, &GraphicsState.quad_vx_array);
            glBindVertexArray(GraphicsState.quad_vx_array);

            glEnableVertexAttribArray(GraphicsState.vxcolor_vpos_loc);
            glVertexAttribPointer(GraphicsState.vxcolor_vpos_loc, 3, GL_FLOAT, GL_FALSE, sizeof(VxColor), (void*)offsetof(VxColor, Pos));
            glEnableVertexAttribArray(GraphicsState.vxcolor_vcol_loc);
            glVertexAttribPointer(GraphicsState.vxcolor_vcol_loc, 3, GL_FLOAT, GL_FALSE, sizeof(VxColor), (void*)offsetof(VxColor, Col));

            glBindVertexArray(0);
        }
    }

    Mat4 GetOrthoVP(float Width, float Height)
    {
        float HalfWidth = Width * 0.5f;
        float HalfHeight = Height * 0.5f;
        Mat4 ViewProj = HMM_Orthographic_RH_NO(-HalfWidth, HalfWidth, -HalfHeight, HalfHeight, -1.0f, 1.0f);
        return ViewProj;
    }

    void GFXState::Draw()
    {
        int FrameWidth = 0, FrameHeight = 0;
        glfwGetFramebufferSize(Window, &FrameWidth, &FrameHeight);
        const float AspectRatio = (float)FrameWidth / (float)FrameHeight;
        glViewport(0, 0, FrameWidth, FrameHeight);

        glClearColor(BgColors[BgIdx].R, BgColors[BgIdx].G, BgColors[BgIdx].B, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        Mat4 VP = GetOrthoVP((float)FrameWidth, (float)FrameHeight);
        float fTime = (float)glfwGetTime();
        HMM_Mat4 MVP = HMM_M4D(1.0f);
        { // Draw Tri
            glUseProgram(GraphicsState.vxcolor.program);

            Tri.RotZ = fTime;
            MVP = VP * Tri.GetModelTransform();
            glUniformMatrix4fv(GraphicsState.vxcolor_mvp_loc, 1, GL_FALSE, (const GLfloat*)&MVP);

            glBindVertexArray(GraphicsState.tri_vx_array);
            glDrawArrays(GL_TRIANGLES, 0, 3);

        }

        { // Draw Quad
            glUseProgram(GraphicsState.vxcolor.program);

            Quad.RotZ = -2.0f * fTime;
            MVP = VP * Quad.GetModelTransform();
            glUniformMatrix4fv(GraphicsState.vxcolor_mvp_loc, 1, GL_FALSE, (const GLfloat*)&MVP);

            glBindVertexArray(GraphicsState.quad_vx_array);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        {
            glUseProgram(vxunicolor.program);

            UniQuad.RotZ = 0.0f;
            MVP = VP * UniQuad.GetModelTransform();
            glUniformMatrix4fv(vxcolor_mvp_loc, 1, GL_FALSE, (const GLfloat*)&MVP);
            glUniform4fv(vxunicolor_ucol_loc, 1, (const GLfloat*)&BgColors[QuadColIdx]);

            glBindVertexArray(unicolor_quad_vx_array);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glfwSwapBuffers(Window);
    }

    void GFXState::Term()
    {

    }
}