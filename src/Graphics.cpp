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

        vShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vShader, 1, &vshader_src, nullptr);
        glCompileShader(vShader);

        fShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fShader, 1, &fshader_src, nullptr);
        glCompileShader(fShader);

        Program = glCreateProgram();
        glAttachShader(Program, vShader);
        glAttachShader(Program, fShader);
        glLinkProgram(Program);

        delete[] vshader_src;
        delete[] fshader_src;
    }

    void GFXState::Init()
    {
        PipelineColor.Init("src/glsl/vxcolor_v.glsl", "src/glsl/vxcolor_f.glsl");
        PipelineUnicolor.Init("src/glsl/vxunicolor_v.glsl", "src/glsl/vxunicolor_f.glsl");

        PipelineColor_Loc_MVP = glGetUniformLocation(PipelineColor.Program, "MVP");
        PipelineColor_Loc_vPos = glGetAttribLocation(PipelineColor.Program, "vPos");
        PipelineColor_Loc_vCol = glGetAttribLocation(PipelineColor.Program, "vCol");

        {
            Tri.Pos = Vec2{ -570.0f, 405.0f };
            Tri.Size = Vec2{ 100.0f, 100.0f };
            Tri.RotZ = 0.0f;

            Quad.Pos = Vec2{ 570.0f, 410.0f};
            Quad.Size = Vec2{ 100.0f, 100.0f };
            Quad.RotZ = 0.0f;

            UniQuad.Pos = Vec2{ 250.0f, -250.0f };
            UniQuad.Size = Vec2{ 50.0f, 50.0f };
            UniQuad.RotZ = 0.0f;
        }

        {
            PipelineUnicolor_Loc_MVP = glGetUniformLocation(PipelineUnicolor.Program, "MVP");
            PipelineUnicolor_Loc_uCol = glGetUniformLocation(PipelineUnicolor.Program, "uCol");
            PipelineUnicolor_Loc_vPos = glGetAttribLocation(PipelineUnicolor.Program, "vPos");

            glGenBuffers(1, &UnicolorQuad_VxBuff);
            glBindBuffer(GL_ARRAY_BUFFER, UnicolorQuad_VxBuff);
            glBufferData(GL_ARRAY_BUFFER, sizeof(MinQuadVxs), MinQuadVxs, GL_STATIC_DRAW);
            glGenVertexArrays(1, &UnicolorQuad_VAO);
            glBindVertexArray(UnicolorQuad_VAO);

            glEnableVertexAttribArray(PipelineUnicolor_Loc_vPos);
            glVertexAttribPointer(PipelineUnicolor_Loc_vPos, 3, GL_FLOAT, GL_FALSE, sizeof(VxMin), (void*)offsetof(VxMin, Pos));

            glBindVertexArray(0);
        }

        { // Tri
            glGenBuffers(1, &Tri_VxBuff);
            glBindBuffer(GL_ARRAY_BUFFER, Tri_VxBuff);
            glBufferData(GL_ARRAY_BUFFER, sizeof(TriVxs), TriVxs, GL_STATIC_DRAW);
            glGenVertexArrays(1, &Tri_VAO);
            glBindVertexArray(Tri_VAO);

            glEnableVertexAttribArray(PipelineColor_Loc_vPos);
            glVertexAttribPointer(PipelineColor_Loc_vPos, 3, GL_FLOAT, GL_FALSE, sizeof(VxColor), (void*)offsetof(VxColor, Pos));
            glEnableVertexAttribArray(PipelineColor_Loc_vCol);
            glVertexAttribPointer(PipelineColor_Loc_vCol, 3, GL_FLOAT, GL_FALSE, sizeof(VxColor), (void*)offsetof(VxColor, Col));

            glBindVertexArray(0);
        }

        { // Quad
            glGenBuffers(1, &Quad_VxBuff);
            glBindBuffer(GL_ARRAY_BUFFER, Quad_VxBuff);
            glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVxs), QuadVxs, GL_STATIC_DRAW);
            glGenVertexArrays(1, &Quad_VAO);
            glBindVertexArray(Quad_VAO);

            glEnableVertexAttribArray(PipelineColor_Loc_vPos);
            glVertexAttribPointer(PipelineColor_Loc_vPos, 3, GL_FLOAT, GL_FALSE, sizeof(VxColor), (void*)offsetof(VxColor, Pos));
            glEnableVertexAttribArray(PipelineColor_Loc_vCol);
            glVertexAttribPointer(PipelineColor_Loc_vCol, 3, GL_FLOAT, GL_FALSE, sizeof(VxColor), (void*)offsetof(VxColor, Col));

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
            glUseProgram(PipelineColor.Program);

            Tri.RotZ = fTime;
            MVP = VP * Tri.GetModelTransform();
            glUniformMatrix4fv(PipelineColor_Loc_MVP, 1, GL_FALSE, (const GLfloat*)&MVP);

            glBindVertexArray(Tri_VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        { // Draw Quad
            glUseProgram(PipelineColor.Program);

            Quad.RotZ = -2.0f * fTime;
            MVP = VP * Quad.GetModelTransform();
            glUniformMatrix4fv(PipelineColor_Loc_MVP, 1, GL_FALSE, (const GLfloat*)&MVP);

            glBindVertexArray(Quad_VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        {
            glUseProgram(PipelineUnicolor.Program);

            UniQuad.RotZ = 0.0f;
            MVP = VP * UniQuad.GetModelTransform();
            glUniformMatrix4fv(PipelineColor_Loc_MVP, 1, GL_FALSE, (const GLfloat*)&MVP);
            glUniform4fv(PipelineUnicolor_Loc_uCol, 1, (const GLfloat*)&BgColors[QuadColIdx]);

            glBindVertexArray(UnicolorQuad_VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glfwSwapBuffers(Window);
    }

    void GFXState::Term()
    {

    }
}