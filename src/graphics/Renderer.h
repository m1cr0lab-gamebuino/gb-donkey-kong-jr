// -------------------------------------------------------------------------
//                               Donkey Kong Jr
//           a Nintendo's Game & Watch adaptation on Gamebuino META
// -------------------------------------------------------------------------
//                                © 2019 Steph
//                        https://gamebuino.com/@steph
// -------------------------------------------------------------------------
//                          Graphics Rendering Engine
// -------------------------------------------------------------------------
// This version does not rely on the use of `gb.display`, but rather uses
// the low-level `gb.tft` API to fully exploit the RGB565 color space.
// 
// The graphic rendering is cleverly calculated on two partial framebuffers,
// which are alternately transmitted to the DMA controller, which acts as
// an intermediary with the display device.
// 
// The screen surface is sliced to decompose the rendering calculation on
// the framebuffers and save memory space.
// 
// Many thanks to Andy (https://gamebuino.com/@aoneill) for the magical
// routines related to the DMA controller.
// -------------------------------------------------------------------------

#ifndef DONKEY_KONG_JR_RENDERER
#define DONKEY_KONG_JR_RENDERER

// loads descriptive data of sprites
#include "sprites.h"

// loads the official library
#include <Gamebuino-Meta.h>

// forward declaration of the game engine
// to break an inclusion cycle
class Game;

// the class definition
class Renderer
{
    private:

        // alternating framebuffers
        uint16_t * buffer1;
        uint16_t * buffer2;

        // flag for an ongoing data transfer
        bool drawPending;

        // initiates memory forwarding to the DMA controller
        void customDrawBuffer(uint8_t x, uint8_t y, uint16_t * buffer, uint8_t w, uint8_t h);
        // waits for the memory transfer to be completed
        // and close the transaction with the DMA controller
        void waitForPreviousDraw();

    public:

        static constexpr uint8_t ScreenWidth     = 160; // META screen width
        static constexpr uint8_t ScreenHeight    = 128; // META screen height
        static constexpr uint8_t SliceHeight     = 8;   // height of a screen slice

        static constexpr uint8_t DigitWidth      = 5; // width of each digit's sprite
        static constexpr uint8_t DigitHeight     = 9; // height of each digit's sprite
        static constexpr uint8_t DigitOffset     = 2; // offset in pixels between two digits
        static constexpr uint8_t DigitSheetWidth = 10 * DigitWidth; // total width of the digits spritesheet

        Renderer();  // constructor
        ~Renderer(); // destructor

        // starts the graphic rendering of the game scene
        void draw(Game * engine);
        // renders a specific sprite on the current screen slice
        void drawSpriteOnSlice(Sprite sprite, uint8_t sliceY, uint16_t * buffer);
        // renders a specific digit on the current screen slice
        void drawDigitOnSlice(uint8_t digit, uint8_t index, uint8_t sliceY, uint16_t * buffer);
};

#endif