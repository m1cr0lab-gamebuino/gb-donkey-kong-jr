// -------------------------------------------------------------------------
//                               Donkey Kong Jr
//           a Nintendo's Game & Watch adaptation on Gamebuino META
// -------------------------------------------------------------------------
//                                © 2019 Steph
//                        https://gamebuino.com/@steph
// -------------------------------------------------------------------------
//                          Graphics Rendering Engine
// -------------------------------------------------------------------------

// loads the header file of the class
#include "Renderer.h"

// loads external modules
#include "assets.h"         // loads the graphic resources
#include "../engine/Game.h" // loads the definition of the game engine

// preparation related to the DMA controller of the display device
namespace Gamebuino_Meta {
    #define DMA_DESC_COUNT 3
    extern volatile uint32_t dma_desc_free_count;
    static inline void wait_for_transfers_done(void) {
        while (dma_desc_free_count < DMA_DESC_COUNT);
    }
    static SPISettings tftSPISettings = SPISettings(24000000, MSBFIRST, SPI_MODE0);
};

// constructor
Renderer::Renderer() {
    buffer1 = new uint16_t[ScreenWidth * SliceHeight];
    buffer2 = new uint16_t[ScreenWidth * SliceHeight];
    drawPending = false;
}

// destructor
Renderer::~Renderer() {
    delete buffer2;
    delete buffer1;
}

// initiates memory forwarding to the DMA controller
void Renderer::customDrawBuffer(uint8_t x, uint8_t y, uint16_t * buffer, uint8_t w, uint8_t h) {
    drawPending = true;
    gb.tft.setAddrWindow(x, y, x + w - 1, y + h - 1);
    SPI.beginTransaction(Gamebuino_Meta::tftSPISettings);
    gb.tft.dataMode();
    gb.tft.sendBuffer(buffer, w*h);
}

// waits for the memory transfer to be completed
// and close the transaction with the DMA controller
void Renderer::waitForPreviousDraw() {
    if (drawPending) {
        Gamebuino_Meta::wait_for_transfers_done();
        gb.tft.idleMode();
        SPI.endTransaction();
        drawPending = false;
    }
}

// -------------------------------------------------------------------------
// Graphic rendering
// -------------------------------------------------------------------------
// This is where the process of rendering the game scene is triggered.
// The principle followed here does not correspond to the standard use of
// the `gb.display` API. Indeed, we wish here to exploit the full resolution
// of the META in RGB565, i.e. 160x128 pixels in 16-bit color depth.
// 
// The problem here is the memory space required for such a resolution.
// To compose a suitable framebuffer, it would be necessary to have:
// 160 x 128 x 16 / 2 bytes = 40 KB of RAM. We obviously do not have this
// memory space on the META.
// 
// We will therefore have to divide the rendering process into successive
// partial calculations which consist in cutting the display surface into
// equal horizontal slices. Each slice is run in turn and the rendering
// calculation is done on a framebuffer of the same size as the slice.
// 
// The preparation of the framebuffer consists in writing 16-bit binary
// integers corresponding to the color codes of the pixels on the slice.
// Once calculated, the framebuffer is transmitted to the display device
// through the DMA controller in order to parallelize the data transfer,
// while the CPU takes over to calculate the framebuffer for the next slice.
// This process is carried out through the `gb.tft` API.
// 
// The rendering calculation is, in the first instance, delegated to the
// graphical rendering engine that will perform the division of the
// display surface into slices and prepare a framebuffer for each slice.
// For each of these slices, the graphics rendering engine will ask the game
// engine to render the scene on the corresponding framebuffer, integrating
// all the appropriate graphics elements (background, sprites, score, etc.).
// 
// The game engine sends back the information about these elements to the
// graphics rendering engine, which will determine if there is a non-empty
// intersection between the surface of these elements and that of the
// current slice.
// 
// The data is finally written to the framebuffer by the graphics rendering
// engine when it is not trivial and requires a precise calculation of the
// coordinates of the pixels to be reproduced (which is not the case for the
// background scenery).
// -------------------------------------------------------------------------

// starts the graphic rendering of the game scene
void Renderer::draw(Game * engine) {
    // the number of horizontal slices to be cut is calculated
    uint8_t slices = ScreenHeight / SliceHeight;
    // declares a pointer that will alternate between the two memory buffers
    uint16_t * buffer;
    // declares the top border of current slice
    uint8_t sliceY;
    // then we go through each slice one by one
    for (uint8_t sliceIndex = 0; sliceIndex < slices; sliceIndex++) {
        // buffers are switched according to the parity of sliceIndex
        buffer = sliceIndex % 2 == 0 ? buffer1 : buffer2;
        // the top border of the current slice is calculated
        sliceY = sliceIndex * SliceHeight;

        // delegates the slice rendering to the game engine
        engine->drawOnSlice(sliceY, buffer);

        // then we make sure that the sending of the previous buffer
        // to the DMA controller has taken place
        if (sliceIndex != 0) waitForPreviousDraw();
        // after which we can then send the current buffer
        customDrawBuffer(0, sliceY, buffer, ScreenWidth, SliceHeight);
    }

    // always wait until the DMA transfer is completed
    // for the last slice before entering the next cycle
    waitForPreviousDraw();
}

// renders a specific sprite on the current screen slice
void Renderer::drawSpriteOnSlice(Sprite sprite, uint8_t sliceY, uint16_t * buffer) {
    // we check first of all that the intersection between
    // the sprite and the current slice is not empty
    if (sliceY < sprite.y + sprite.height && sprite.y < sliceY + SliceHeight) {
        // determines the boundaries of the sprite surface within the current slice
        uint8_t  xmin = sprite.x;
        uint8_t  xmax = sprite.x + sprite.width - 1;
        uint8_t  ymin = sprite.y < sliceY ? sliceY : sprite.y;
        uint8_t  ymax = sprite.y + sprite.height >= sliceY + SliceHeight ? sliceY + SliceHeight - 1 : sprite.y + sprite.height - 1;

        // uint8_t  px, py;
        uint16_t syo, dyo;
        uint16_t color;
        // goes through the sprite pixels to be drawn
        for (uint8_t py = ymin; py <= ymax; py++) {
            // calculates the y offset on source and destination
            syo = py * ScreenWidth;
            dyo = (py - sliceY) * ScreenWidth;
            for (uint8_t px = xmin; px <= xmax; px++) {
                // picks the pixel color from the spritesheet
                color = SPRITESHEET_COLORMAP[px + syo];
                // and if it belongs to the sprite
                if (color == sprite.color) {
                    // plots a black pixel into the rendering buffer
                    buffer[px + dyo] = 0x0000;
                }
            }
        }
    }
}

// renders a specific digit on the current screen slice.
// here the framebuffer writing is not done pixel by pixel,
// but by blocks of pixels to speed up the processing
void Renderer::drawDigitOnSlice(uint8_t digit, uint8_t index, uint8_t sliceY, uint16_t * buffer) {
    // calculates the coordinates of the sprite
    uint8_t dx = 149 - index * (DigitWidth + DigitOffset);
    uint8_t dy = 6;

    // we check first of all that the intersection between
    // the sprite and the current slice is not empty
    if (sliceY < dy + DigitHeight && dy < sliceY + SliceHeight) {
        // determines the boundaries of the sprite surface within the current slice
        uint8_t ymin = dy < sliceY ? sliceY : dy;
        uint8_t ymax = dy + DigitHeight >= sliceY + SliceHeight ? sliceY + SliceHeight - 1 : dy + DigitHeight - 1;
        // determines the memory address of the byte block to be copied
        uint16_t * source = (uint16_t *)DIGIT_COLORMAP + digit * DigitWidth;
        // determines the destination memory address in the framebuffer
        uint16_t * dest = buffer + dx;
        // the 16-bit color codes are copied byte by byte,
        // so a factor of 2 is required here
        uint8_t size = DigitWidth << 1;
        // then each memory block is scanned
        for (uint8_t py = ymin; py <= ymax; py++) {
            // and copied to the framebuffer
            memcpy(dest + (py - sliceY) * ScreenWidth, source + (py - dy) * DigitSheetWidth, size);
        }
    }
}