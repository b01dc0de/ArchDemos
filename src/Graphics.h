#ifndef ARCHDEMOS_GRAPHICS_H
#define ARCHDEMOS_GRAPHICS_H

#include "Common.h"

namespace Arch
{
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

        void Init();
        void Draw();
        void Term();
    };

    extern GFXState GraphicsState;
}

#endif // ARCHDEMOS_GRAPHICS_H