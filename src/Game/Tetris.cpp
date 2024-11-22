#include "Tetris.h"
#include "Graphics.h"

namespace Arch
{
    namespace Tetris
    {
        Entity2D TetrisGrid;

        static const Color TetrisGridColor{ 0.1176f, 0.1176f, 0.1804f, 1.0f };
        static const Color TetrisEmptyCellColor{ 0.1608f, 0.1647f, 0.2314f, 1.0 };

        static const Color EtcColors[] =
        {
            { 0.1686f, 0.1490f, 0.1725f, 1.0f}, // Dark purple
            { 0.2588f, 0.1922f, 0.3686f, 1.0f}, // Purple
            { 0.8510f, 0.2863f, 0.3490f, 1.0f}, // Magenta-ish
            { 0.2863f, 0.4667f, 0.5529f, 1.0f}, // Dark sky blue
            { 0.9882f, 0.9804f, 0.2706f, 1.0f}, // Sun yellow
            { 0.5686f, 0.3765f, 0.6627f, 1.0f}  // Lavender
        };

        static const Color CellColors[] =
        {
            TetrisEmptyCellColor, // Empty
            {0.0000, 0.9020, 0.9961, 1.0f}, // I / Light blue
            {0.0941, 0.0039, 1.0000, 1.0f}, // J / Dark blue
            {1.0000, 0.4510, 0.0314, 1.0f}, // L / Orange
            {1.0000, 0.8706, 0.0000, 1.0f}, // O / Yellow
            {0.4000, 0.9922, 0.0000, 1.0f}, // S / Green
            {0.9961, 0.0627, 0.2353, 1.0f}, // Z / Red
            {0.7216, 0.0078, 0.9922, 1.0f}, // T / Magenta
        };

        void Init()
        {
            for (int Idx = 0; Idx < GridSize; Idx++)
            {
                Grid[Idx] = BLOCK_EMPTY;
            }

            TetrisGrid.Pos = Vec2{ 0.0f, 0.0f };
            TetrisGrid.Size = Vec2{ 100.0f, 960.0f };
            TetrisGrid.RotZ = 0.0f;
        }

        void Update()
        {
        }

        void Draw()
        {
            { // Draw Grid Background
            }

            { // Draw individual grid cells
            }
        }

        void Term()
        {
        }
    }
}