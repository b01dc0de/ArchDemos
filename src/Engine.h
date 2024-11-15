#ifndef ENGINE_H
#define ENGINE_H

#define ARRAY_SIZE(A) (sizeof(A) / sizeof(A[0]))

namespace Arch
{
    bool Init(int ArgCount, const char** ArgVals);
    bool MainLoop();
    bool Term();

    struct Color
    {
        float R;
        float G;
        float B;
        float A;
    };

    struct Vec3
    {
        float X;
        float Y;
        float Z;
    };
}

#endif // ENGINE_H