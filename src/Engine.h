#ifndef ARCHDEMOS_ENGINE_H
#define ARCHDEMOS_ENGINE_H

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
}

#endif // ARCHDEMOS_ENGINE_H