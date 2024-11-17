#include "Tetris.h"

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

        void Init()
        {
            for (int GridIdx = 0; GridIdx < GridSize; GridIdx++)
            {
                Grid[GridIdx] = BLOCK_EMPTY;
            }
        }

        void DrawGrid()
        {
        }
    }
}