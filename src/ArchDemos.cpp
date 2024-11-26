#define GLAD_GL_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "ArchDemos.h"

static bool bRunning = true;

#define SINGLE_FILE_BUILD() (1)
#if SINGLE_FILE_BUILD()
    #include "Engine.cpp"
    #include "GameEntity.cpp"
    #include "Graphics.cpp"
    #include "Utils.cpp"
    #include "Game/Tetris.cpp"
#endif // SINGLE_FILE_BUILD()

int main(int argc, const char* argv[])
{
    static const int ExitCode_Success = 0;
    static const int ExitCode_Error = -1;

    bool bResults = true;

    if (bResults &= Arch::Init(argc, argv))
    {
        bResults &= Arch::MainLoop();
        bResults &= Arch::Term();
    }

    return bResults ? ExitCode_Success : ExitCode_Error;
}
