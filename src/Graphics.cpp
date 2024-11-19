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

    static const float QuadSize = 1.0f;
    static const VxMin MinQuadVxs[] =
    {
        VxMin{-QuadSize, -QuadSize, 0.5f},
        VxMin{QuadSize, -QuadSize, 0.5f},
        VxMin{QuadSize, QuadSize, 0.5f},
        VxMin{-QuadSize, QuadSize, 0.5f},
        // TODO: Remove following after Index buffer is impl.
        VxMin{-QuadSize, -QuadSize, 0.5f},
        VxMin{QuadSize, QuadSize, 0.5f},
    };
    using IxT = unsigned short;
    static const IxT QuadIxs[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    static const VxTexture TexQuadVxs[] =
    {
        VxTexture{{-QuadSize, -QuadSize, DefaultZ}, {0.0f, 1.0f}},
        VxTexture{{+QuadSize, -QuadSize, DefaultZ}, {1.0f, 1.0f}},
        VxTexture{{+QuadSize, +QuadSize, DefaultZ}, {1.0f, 0.0f}},
        VxTexture{{-QuadSize, +QuadSize, DefaultZ}, {0.0f, 0.0f}},
        // TODO: Remove following once indices are setup
        VxTexture{{-QuadSize, -QuadSize, DefaultZ}, {0.0f, 1.0f}},
        VxTexture{{+QuadSize, +QuadSize, DefaultZ}, {1.0f, 0.0f}},
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

    void PipelineState::Term()
    {
        glDeleteShader(vShader);
        glDeleteShader(fShader);
        glDeleteProgram(Program);
    }

    void Mesh::Init(size_t InNumVxs, size_t VxSize, const void* VxData)
    {
        NumVxs = InNumVxs;

        glGenBuffers(1, &VxBuff);
        glBindBuffer(GL_ARRAY_BUFFER, VxBuff);
        glBufferData(GL_ARRAY_BUFFER, VxSize * InNumVxs, VxData, GL_STATIC_DRAW);

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
    }

    void Mesh::BindVAO()
    {
        glBindVertexArray(VAO);
    }

    void Mesh::Draw()
    {
        BindVAO();
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)NumVxs);
    }

    void Mesh::Term()
    {
        glDeleteBuffers(1, &VxBuff);
        glDeleteVertexArrays(1, &VAO);
    }

    void GFXState::Init()
    {
        PipelineColor.Init("src/glsl/vxcolor_v.glsl", "src/glsl/vxcolor_f.glsl");
        PipelineColor_Loc_MVP = glGetUniformLocation(PipelineColor.Program, "MVP");
        PipelineColor_Loc_vPos = glGetAttribLocation(PipelineColor.Program, "vPos");
        PipelineColor_Loc_vCol = glGetAttribLocation(PipelineColor.Program, "vCol");

        PipelineUnicolor.Init("src/glsl/vxunicolor_v.glsl", "src/glsl/vxunicolor_f.glsl");
        PipelineUnicolor_Loc_MVP = glGetUniformLocation(PipelineUnicolor.Program, "MVP");
        PipelineUnicolor_Loc_uCol = glGetUniformLocation(PipelineUnicolor.Program, "uCol");
        PipelineUnicolor_Loc_vPos = glGetAttribLocation(PipelineUnicolor.Program, "vPos");

        PipelineTexture.Init("src/glsl/vxtexture_v.glsl", "src/glsl/vxtexture_f.glsl");
        PipelineTexture_Loc_MVP = glGetUniformLocation(PipelineTexture.Program, "MVP");
        PipelineTexture_Loc_vPos = glGetAttribLocation(PipelineTexture.Program, "vPos");
        PipelineTexture_Loc_vUV = glGetAttribLocation(PipelineTexture.Program, "vUV");

        {
            Tri.Pos = Vec2{ -570.0f, 405.0f };
            Tri.Size = Vec2{ 100.0f, 100.0f };
            Tri.RotZ = 0.0f;

            Quad.Pos = Vec2{ 570.0f, 410.0f};
            Quad.Size = Vec2{ 100.0f, 100.0f };
            Quad.RotZ = 0.0f;

            UniQuad.Pos = Vec2{ 250.0f, -250.0f };
            UniQuad.Size = Vec2{ 25.0f, 25.0f };
            UniQuad.RotZ = 0.0f;

            TexQuad.Pos = Vec2{ -250.0f, -250.0f };
            TexQuad.Size = Vec2{ 100.0f, 100.0f };
            TexQuad.RotZ = 0.0f;
        }

        {
            Mesh_Tri.Init(ARRAY_SIZE(TriVxs), sizeof(VxColor), TriVxs);
            Mesh_Tri.BindVAO(); 
            glEnableVertexAttribArray(PipelineColor_Loc_vPos);
            glVertexAttribPointer(PipelineColor_Loc_vPos, 3, GL_FLOAT, GL_FALSE, sizeof(VxColor), (void*)offsetof(VxColor, Pos));
            glEnableVertexAttribArray(PipelineColor_Loc_vCol);
            glVertexAttribPointer(PipelineColor_Loc_vCol, 3, GL_FLOAT, GL_FALSE, sizeof(VxColor), (void*)offsetof(VxColor, Col));

            Mesh_Quad.Init(ARRAY_SIZE(QuadVxs), sizeof(VxColor), QuadVxs);
            Mesh_Quad.BindVAO();
            glEnableVertexAttribArray(PipelineColor_Loc_vPos);
            glVertexAttribPointer(PipelineColor_Loc_vPos, 3, GL_FLOAT, GL_FALSE, sizeof(VxColor), (void*)offsetof(VxColor, Pos));
            glEnableVertexAttribArray(PipelineColor_Loc_vCol);
            glVertexAttribPointer(PipelineColor_Loc_vCol, 3, GL_FLOAT, GL_FALSE, sizeof(VxColor), (void*)offsetof(VxColor, Col));

            Mesh_UniQuad.Init(ARRAY_SIZE(MinQuadVxs), sizeof(VxMin), MinQuadVxs);
            Mesh_UniQuad.BindVAO();
            glEnableVertexAttribArray(PipelineUnicolor_Loc_vPos);
            glVertexAttribPointer(PipelineUnicolor_Loc_vPos, 3, GL_FLOAT, GL_FALSE, sizeof(VxMin), (void*)offsetof(VxMin, Pos));

            Mesh_TexQuad.Init(ARRAY_SIZE(TexQuadVxs), sizeof(VxTexture), TexQuadVxs);
            Mesh_TexQuad.BindVAO();
            glEnableVertexAttribArray(PipelineTexture_Loc_vPos);
            glVertexAttribPointer(PipelineTexture_Loc_vPos, 3, GL_FLOAT, GL_FALSE, sizeof(VxTexture), (void*)offsetof(VxTexture, Pos));
            glEnableVertexAttribArray(PipelineTexture_Loc_vUV);
            glVertexAttribPointer(PipelineTexture_Loc_vUV, 2, GL_FLOAT, GL_FALSE, sizeof(VxTexture), (void*)offsetof(VxTexture, UV));
        }

        { // Texture
            int TestTextureWidth = 0, TestTextureHeight = 0, NumColorChannels = 0;
            using ByteT = unsigned char;
            std::string RelativeFilename = GetBaseDirectory() + std::string{"assets/test/DebugTexture16x16.bmp"};
            ByteT* TextureData = stbi_load(RelativeFilename.c_str(), &TestTextureWidth, &TestTextureHeight, &NumColorChannels, 0);

            if (TextureData)
            {
                glGenTextures(1, &TestTextureID);
                glBindTexture(GL_TEXTURE_2D, TestTextureID);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TestTextureWidth, TestTextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
                glGenerateMipmap(GL_TEXTURE_2D);

                stbi_image_free(TextureData);
            }
        }

        { // Global OpenGL
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
        { // Draw Tri
            glUseProgram(PipelineColor.Program);

            Tri.RotZ = fTime;
            Mat4 MVP = VP * Tri.GetModelTransform();
            glUniformMatrix4fv(PipelineColor_Loc_MVP, 1, GL_FALSE, (const GLfloat*)&MVP);

            Mesh_Tri.Draw();
        }

        { // Draw Quad
            glUseProgram(PipelineColor.Program);

            Quad.RotZ = -2.0f * fTime;
            Mat4 MVP = VP * Quad.GetModelTransform();
            glUniformMatrix4fv(PipelineColor_Loc_MVP, 1, GL_FALSE, (const GLfloat*)&MVP);

            Mesh_Quad.Draw();
        }

        { // Draw UniQuad
            glUseProgram(PipelineUnicolor.Program);

            Mat4 MVP = VP * UniQuad.GetModelTransform();
            glUniformMatrix4fv(PipelineColor_Loc_MVP, 1, GL_FALSE, (const GLfloat*)&MVP);
            glUniform4fv(PipelineUnicolor_Loc_uCol, 1, (const GLfloat*)&BgColors[QuadColIdx]);

            Mesh_UniQuad.Draw();
        }

        { // Draw TexQuad
            glUseProgram(PipelineTexture.Program);

            Mat4 MVP = VP * TexQuad.GetModelTransform();
            glUniformMatrix4fv(PipelineTexture_Loc_MVP, 1, GL_FALSE, (const GLfloat*)&MVP);

            //glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, TestTextureID);

            Mesh_TexQuad.Draw();
        }

        glfwSwapBuffers(Window);
    }

    void GFXState::Term()
    {
        PipelineColor.Term();
        PipelineUnicolor.Term();
        Mesh_Tri.Term();
        Mesh_Quad.Term();
        Mesh_UniQuad.Term();
    }
}