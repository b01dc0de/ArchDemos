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

    struct PipelineState
    {
        GLuint vshader = 0;
        GLuint fshader = 0;
        GLuint program = 0;

        void Init(const char* VertexSrcFilename, const char* FragmentSrcFilename);
    };

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

    struct GFXState
    {
        GLuint tri_vx_buffer = 0;
        GLuint tri_vx_array = 0;

        GLuint quad_vx_buffer = 0;
        GLuint quad_vx_array = 0;

        GLuint unicolor_quad_vx_buffer = 0;
        GLuint unicolor_quad_vx_array = 0;
        GLuint unicolor_quad_ix_buffer = 0;

        PipelineState vxcolor;
        PipelineState vxunicolor;

        GLint vxcolor_mvp_loc = 0;
        GLint vxcolor_vpos_loc = 0;
        GLint vxcolor_vcol_loc = 0;

        int BgIdx = 0;

        Entity2D Tri;
        Entity2D Quad;

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