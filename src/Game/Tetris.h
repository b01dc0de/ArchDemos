#ifndef ARCHDEMOS_GAME_TETRIS_H
#define ARCHDEMOS_GAME_TETRIS_H

#include "Common.h"

namespace Arch
{
    namespace Tetris
    {
        enum TetrominoType
        {
            BLOCK_EMPTY,
            BLOCK_I,
            BLOCK_J,
            BLOCK_L,
            BLOCK_O,
            BLOCK_S,
            BLOCK_Z,
            BLOCK_T,
            BLOCK_NUM
        };

        static const int GridWidth = 10;
        static const int GridHeight = 20;
        static const int GridSize = GridWidth * GridHeight;
        TetrominoType Grid[GridSize];

        void Init();
        void Update();
        void Draw();
        void Term();
    }
}

#endif // ARCHDEMOS_GAME_TETRIS_H