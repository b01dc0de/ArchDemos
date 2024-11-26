#ifndef ARCHDEMOS_GRAPHICS_H
#define ARCHDEMOS_GRAPHICS_H

#include "Common.h"

namespace Arch
{
    struct VxColor
    {
        Vec3 Pos;
        Vec3 Col;
    };

    struct VxMin
    {
        Vec3 Pos;
    };

    struct VxTexture
    {
        Vec3 Pos;
        Vec2 UV;
    };

    struct PipelineState
    {
        GLuint vShader = 0;
        GLuint fShader = 0;
        GLuint Program = 0;
        // TODO(CKA):
        //  - Would be nice to have a LocateUniforms(...) and LocateVxAttribs(...) functions
        //      that take a list of strings and automatically locate and register each within a
        //      managed list of shader locations

        void Init(const char* VertexSrcFilename, const char* FragmentSrcFilename);
        void Term();
    };

    struct Mesh
    {
        size_t NumVxs = 0;
        GLuint VxBuff = 0;
        GLuint VAO = 0;

        void Init(size_t InNumVxs, size_t VxSize, const void* VxData);
        void BindVAO();
        void Draw();
        void Term();
    };

    //struct VxAttribs { };
    //struct UniformLocs { };

    struct Entity2D
    {
        Vec2 Pos{ 0.0f, 0.0f };
        Vec2 Size{ 1.0f, 1.0f };
        float RotZ = 0.0f;

        Mat4 GetModelTransform()
        {
            static const Vec3 AxisZ{ 0.0f, 0.0f, 1.0f };
            Vec3 TransV{Pos.X, Pos.Y, 0.0f};
            Vec3 ScaleV{Size.X, Size.Y, 1.0f};
            Mat4 Model = HMM_Translate(TransV) * HMM_Rotate_RH(RotZ, AxisZ) * HMM_Scale(ScaleV);
            return Model;
        }
    };

    struct Texture2D
    {
        const char* Filename = nullptr;
        int Width = 0;
        int Height = 0;
        GLuint ID = 0;

        bool Init(const char* InFilename);
        bool IsValid();
        void Term();
    };

    struct TextureManager
    {
        static Array<Texture2D> TextureList;

        static Texture2D* FindTexture(const char* Filename);
        static Texture2D* GetTexture(const char* Filename);
        static void Term();
    };

    enum DrawCmdType
    {
        DRAWCMDTYPE_UNICOLOR,
        DRAWCMDTYPE_TEXTURE,
        DRAWCMDTYPE_COUNT
    };
    struct DrawCmd2D
    {
        DrawCmdType Type = DRAWCMDTYPE_COUNT;
        Entity2D Entity;
        union
        {
            Color Unicolor;
            Texture2D* Tex2D;
        } Params;
    };

    struct GFXState
    {
        PipelineState PipelineColor;
        GLint PipelineColor_Loc_MVP = 0;
        GLint PipelineColor_Loc_vPos = 0;
        GLint PipelineColor_Loc_vCol = 0;

        PipelineState PipelineUnicolor;
        GLint PipelineUnicolor_Loc_MVP = 0;
        GLint PipelineUnicolor_Loc_uCol = 0;
        GLint PipelineUnicolor_Loc_vPos = 0;

        PipelineState PipelineTexture;
        GLint PipelineTexture_Loc_MVP = 0;
        GLint PipelineTexture_Loc_vPos = 0;
        GLint PipelineTexture_Loc_vUV = 0;

        Texture2D* FallbackTexture = nullptr;

        Mesh Mesh_Tri;
        Mesh Mesh_Quad;
        Mesh Mesh_UniQuad;
        Mesh Mesh_TexQuad;

        int BgIdx = 0;
        int QuadColIdx = 1;

        Entity2D Tri;
        Entity2D Quad;
        Entity2D UniQuad;
        Entity2D TexQuad;

        Entity2D TetrisGrid;

        void Init();
        void Draw();
        void Term();
    };

    static const Color BgColors[] = {
        { 0.890f, 0.412f, 0.557f, 1.0f }, // Pink-ish
        { 0.408f, 0.369f, 0.600f, 1.0f }, // Lavender-ish
        { 0.306f, 0.259f, 0.600f, 1.0f }, // Violet-ish
        { 0.125f, 0.133f, 0.267f, 1.0f }, // Indigo-ish
        { 0.000f, 0.561f, 0.325f, 1.0f }, // Green-ish
        { 0.063f, 0.388f, 0.435f, 1.0f } // Teal-ish
    };

    extern GFXState GraphicsState;
}

#endif // ARCHDEMOS_GRAPHICS_H
