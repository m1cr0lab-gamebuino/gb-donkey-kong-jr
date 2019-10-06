// -------------------------------------------------------------------------
//                               Donkey Kong Jr
//           a Nintendo's Game & Watch adaptation on Gamebuino META
// -------------------------------------------------------------------------
//                                © 2019 Steph
//                        https://gamebuino.com/@steph
// -------------------------------------------------------------------------

// loads the game engine
#include "src/engine/Game.h"

// includes the official library
#include <Gamebuino-Meta.h>

// -------------------------------------------------------------------------
// Initialization
// -------------------------------------------------------------------------

// creates the game engine
Game game;

void setup() {
    // initializes the META
    gb.begin();
    // default screen buffer won't be used
    // so it must be set to 0x0 pixels
    gb.display.init(0, 0, ColorMode::rgb565);
    // sets the display frame rate
    gb.setFrameRate(Game::FrameRate);
    // initializes the game engine
    game.setup();
}

// -------------------------------------------------------------------------
// Main control loop
// -------------------------------------------------------------------------

void loop() {
    // makes sure that the Gamebuino Meta runs at a constant framerate
    // (as long as the CPU load is not exceeded)
    gb.waitForUpdate();
    // delegates the control loop to the game engine
    game.loop();
}