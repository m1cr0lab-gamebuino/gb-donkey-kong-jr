// -------------------------------------------------------------------------
//                               Donkey Kong Jr
//           a Nintendo's Game & Watch adaptation on Gamebuino META
// -------------------------------------------------------------------------
//                                © 2019 Steph
//                        https://gamebuino.com/@steph
// -------------------------------------------------------------------------
//                                 Game Engine
// -------------------------------------------------------------------------

// loads the header file of the class
#include "Game.h"

// loads external modules
#include "../graphics/assets.h"  // loads the graphic resources
#include "../graphics/sprites.h" // loads the description of all sprites
#include "../sounds/Buzzer.h"    // loads the definition of the sound effects controller

// -------------------------------------------------------------------------
// Key swinging
// -------------------------------------------------------------------------

// here we define the animation sequence of the unlocking key,
// which swings against the branch of a tree.
// each index corresponds to the sprite number of the key defined
// in `src/graphics/sprites.h` by the `keySprite` array
const uint8_t Game::keyFrame[keyFrames] = {0, 1, 2, 3, 3, 2, 1};

// -------------------------------------------------------------------------
// Initialization
// -------------------------------------------------------------------------

Game::Game() {
    state        = GameState::Boot;
    type         = GameType::GameA;
    displayState = DisplayState::Score;
    scoreState   = ScoreState::Score;

    scoreRollTime = 0;

    // the beat of the metronome will fully rhythm the game's progress.
    // it is synchronized on the display frequency at the start
    // and therefore set to 1 second by default.
    // it will be gradually accelerated during the game.
    tick        = FrameRate;

    keyIndex    = 1;
    miss        = lives;
    unlocked    = 0;
    missChecked = false;

    // instantiation of the auxiliary modules by dynamic memory allocation
    score    = new Score();
    player   = new Player();
    creature = new Creature(player, score);
    fruit    = new Fruit(player, creature, score);
    renderer = new Renderer();
}

void Game::setup() {
    // when the game engine is initialized, the scores saved
    // on the microSD are loaded into memory
    score->load();
}

// -------------------------------------------------------------------------
// Destruction
// -------------------------------------------------------------------------

Game::~Game() {
    // all modules instantiated by dynamic allocation must be properly destroyed
    // to free the allocated memory
    delete renderer;
    delete fruit;
    delete creature;
    delete player;
    delete score;
}

// -------------------------------------------------------------------------
// Main control loop
// -------------------------------------------------------------------------

void Game::loop() {
    // we look at the state of the game engine
    // to apply the corresponding procedure and
    // monitor a possible interaction with the player
    switch (state) {
        case GameState::Boot:
            updateBootMode();
            break;
        case GameState::Demo:
            demo();
            rollScore();
            waitForStarting();
            break;
        case GameState::Playing:
            waitForMoving();
            break;
    }

    // monitors the activation or deactivation of sound effects
    waitForSoundToggle();

    // examines a possible change of display initiated by the player
    waitForDisplayToggle();

    // when the game engine is not in the botting phase
    if (state != GameState::Boot) {
        // a game is simulated
        updateJuniorJumping();
        updateJuniorUnlocking();
        updateJuniorFalling();
        updateKey();
        updateFruit();
        updateCreature();
        // but we disable the management of the score and
        // sound effects if the player is not playing
        if (state == GameState::Playing) {
            score->update();
            Buzzer::update();
        }
    }

    // and finally, we trigger the graphic
    // rendering of the game scene
    draw();
}

// -------------------------------------------------------------------------
// Demo mode
// -------------------------------------------------------------------------

void Game::demo() {
    // with each metronome beat...
    if (gb.frameCount % tick == 0) {
        // during the demo phase, the evolution of the avatar
        // is simulated by a purely random behaviour
        uint8_t r = random(4);
        switch (r) {
            case 0:
                player->move(moveLeft, false);
                break;
            case 1:
                player->move(moveRight, false);
                break;
            case 2:
                player->move(moveUp, false);
                break;
            case 3:
                player->move(moveDown, false);
                break;
        }
    }
}

// during the demo phase, the display of the score alternates successively
// between the last score reached and the best scores recorded for
// Game A and Game B
void Game::rollScore() {
    // the alternation takes place every 4 seconds
    if (gb.frameCount - scoreRollTime > 4 * FrameRate) {

        switch (scoreState) {
            case ScoreState::Score:
                scoreState = ScoreState::HighScoreA;
                break;
            case ScoreState::HighScoreA:
                scoreState = ScoreState::HighScoreB;
                break;
            case ScoreState::HighScoreB:
                scoreState = ScoreState::Score;
                break;
        }

        scoreRollTime = gb.frameCount;
    }

}

// -------------------------------------------------------------------------
// User interactions
// -------------------------------------------------------------------------

void Game::waitForSoundToggle() {
    // pressing the B button alternates between
    // turning sound effects on or off
    if (gb.buttons.pressed(BUTTON_B)) {
        Buzzer::toggleMute();
    }
}

void Game::waitForDisplayToggle() {
    // pressing the MENU button alternates between the scores and
    // the CPU load or available memory space.
    // this feature was especially useful during the debugging phase,
    // but I finally kept it  🤓
    if (gb.buttons.pressed(BUTTON_MENU)) {
        switch (displayState) {
            case DisplayState::Score:
                displayState = DisplayState::CPU;
                break;
            case DisplayState::CPU:
                displayState = DisplayState::RAM;
                break;
            case DisplayState::RAM:
                displayState = DisplayState::Score;
                break;
        }
    }
}

// during the demo phase, you can start:
// - either a Game A with A button
// - or a Game B with B button
void Game::waitForStarting() {
         if (gb.buttons.pressed(BUTTON_A)) startGame(GameType::GameA);
    else if (gb.buttons.pressed(BUTTON_B)) startGame(GameType::GameB);
}

// during the playing phase, the player can move his avatar by pressing:
// - LEFT  button to go to the left
// - RIGHT button to go to the right
// - UP    button to climb up along the vines
// - DOWN  button to drop off a vine
// - A     button to jump or hang on to a vine
void Game::waitForMoving() {
    // if the avatar has been bitten by a snapjaw or a bird,
    // any movement is blocked
    if (!creature->hasBittenJunior) {

        // we are here in the **only** case where
        // the avatar can climb up along a vine
        if (player->spriteIndex == Top4     &&
            gb.buttons.pressed(BUTTON_LEFT) &&
            gb.buttons.pressed(BUTTON_A)) {
                player->grab();
        }

        // from the moment the avatar has jumped to grab the unlocking key,
        // the player must no longer be able to control it
        if (player->spriteIndex < GrabKey) {

            int8_t * direction = NULL;
            
                 if (gb.buttons.pressed(BUTTON_LEFT))                                       direction = (int8_t *)moveLeft;
            else if (gb.buttons.pressed(BUTTON_RIGHT))                                      direction = (int8_t *)moveRight;
            else if (gb.buttons.pressed(BUTTON_UP)   && player->spriteIndex == BottomJump6) direction = (int8_t *)moveUp;
            else if (gb.buttons.pressed(BUTTON_A)    && player->spriteIndex != BottomJump6) direction = (int8_t *)moveUp;
            else if (gb.buttons.pressed(BUTTON_DOWN) && !player->isJumping())               direction = (int8_t *)moveDown;

            if (direction != NULL) {

                // the player must not be able to cross the position occupied by an enemy!

                if (
                    direction == moveLeft && (
                        creature->isThereAnyBirdNearJunior(bittenByBirdWhenMoveLeft[player->spriteIndex]) ||
                        creature->isThereAnyCrocNearJunior(bittenByCrocWhenMoveLeft[player->spriteIndex])
                    )
                ) return;

                else if (
                    direction == moveRight && (
                        creature->isThereAnyBirdNearJunior(bittenByBirdWhenMoveRight[player->spriteIndex]) ||
                        creature->isThereAnyCrocNearJunior(bittenByCrocWhenMoveRight[player->spriteIndex])
                    )
                ) return;

                // if this was not the case, then the movement is performed normally
                player->move(direction);
            }
        }

    }
}

// -------------------------------------------------------------------------
// Game logic
// -------------------------------------------------------------------------

// the boot phase lasts only one second and simply
// simulates the lighting of the LCD screen
void Game::updateBootMode() {
    if (gb.frameCount > FrameRate) {
        state = GameState::Demo;
        scoreRollTime = gb.frameCount;
    }
}

// launch of a new game
void Game::startGame(GameType type) {
    this->type  = type;
    state       = GameState::Playing;
    scoreState  = ScoreState::Score;

    tick        = type == GameType::GameA ? FrameRate : 3 * FrameRate / 5;
    miss        = 0;
    unlocked    = 0;
    missChecked = false;

    score->reset();
    creature->reset();
    player->reset();
    fruit->reset();
}

// stop of the current game
void Game::stopGame() {
    state         = GameState::Demo;
    scoreRollTime = gb.frameCount;
    tick          = FrameRate;

    // the achieved score is eventually saved
    // on the microSD as a highscore
    switch (type) {
        case GameType::GameA:
            score->saveA();
            break;
        case GameType::GameB:
            score->saveB();
            break;
    }
}

void Game::updateJuniorJumping() {
    // if the player has made a jump and has not been bitten in the same time,
    // the duration of the jump is controlled so that the avatar falls
    // to the ground after a short time
    if (player->isJumping() && !creature->hasBittenJunior && gb.frameCount - player->timer > tick >> 1) {
        // the `false` value indicates that the sound effect should not be played
        player->move(moveDown, false);
    }
}

void Game::updateJuniorUnlocking() {
    uint32_t elapsedTime = gb.frameCount - player->timer;
    // if the player is unlocking the cage...
    if (player->isUnlocking()) {
        // and after a short break...
        if (elapsedTime > tick) {
            // if the avatar is about to insert the key into the lock...
            if (player->hasArmUp()) {
                // we calculate the time taken by the player to get there
                uint32_t timeToUnlock = gb.frameCount - player->startupTime;
                uint8_t  scoring      = 0;
                unlocked++;
                // and then we calculate his reward accordingly
                     if (timeToUnlock < minTimeToUnlock) scoring = maxRewardUnlock;
                else if (timeToUnlock > maxTimeToUnlock) scoring = minRewardUnlock;
                else scoring = maxRewardUnlock - (maxRewardUnlock - minRewardUnlock) * (timeToUnlock - minTimeToUnlock) / (maxTimeToUnlock - minTimeToUnlock);
                // if the player has unlocked all four parts of the cage,
                // he receives an additional reward
                if (unlocked == cageParts) scoring += maxRewardUnlock;
                // the score is updated (asynchronously)
                score->inc(scoring);
                // and we lower the avatar's arm to insert the key
                player->unlock(ArmState::Down);
            // otherwise, if the avatar was already in the key insertion position,
            // as soon as the score has finished updating...
            } else if (player->hasArmDown() && !score->isIncreasing()) {
                // we lower the avatar down
                player->comeDown();
            }
        }
    // if the avatar is already descending,
    } else if (player->isComingDown()) {
        // and after a short break...
        if (elapsedTime > tick) {
            // if all parts of the cage have been unlocked...
            if (unlocked == cageParts) {
                // the unlock counter is reset to zero
                unlocked = 0;
                // and the metronome beat is slightly accelerated
                     if (type == GameType::GameA && tick > minTick) tick -= 2;
                else if (type == GameType::GameB && tick > minTick) tick--;
            }
            // then the avatar returns to his starting position
            player->reset();
            // and any enemies in the vicinity of this position are deleted
            // to allow the player to restart from his starting position
            creature->clear();
        }
    }
}

// when the player misses the key by jumping,
// he is precipitated into a fall that is done in two steps:
// - FallingUp   (the upper position of the fall)
// - FallingDown (the lower position of the fall)
void Game::updateJuniorFalling() {
    uint32_t elapsedTime = gb.frameCount - player->timer;
    // if the player has finished his fall and the sound effect
    // of the failure has already been played...
    if (player->isFallingDown() && Buzzer::hasRepeated) {
        // the avatar returns to his starting position
        player->reset();
        // again, any enemies in the vicinity of this position
        // are deleted to allow the player to leave again
        creature->clear();
        missChecked = false;
        // if the number of failures reaches the fatal limit, the game ends
        if (miss == lives) stopGame();
    // otherwise, if the player has just started to fall...
    } else if (player->isFallingUp() && elapsedTime > tick) {
        // after a short break, he moves on to the second phase of his fall
        player->fall(FallState::Down);
        // and if the failure has not yet been taken into account...
        if (!missChecked) {
            // it's time to do it!
            miss++;
            missChecked = true;
        }
    }
}

// the key swings at the end of a branch,
// until the player is able to grasp it
void Game::updateKey() {
    if (gb.frameCount % (tick >> 1) == 0) {

        // as soon as the player tries to grab it...
        if (player->isGrabbing()) {
            // if it is in the right position...
            if (keyIndex == 0) {
                // it is a success and the player enters the unlocking phase
                player->unlock(ArmState::Up);
            // otherwise...
            } else {
                // it is a failure and the player enters the falling phase
                player->fall(FallState::Up);
                missChecked = false;
            }
        }

        // if there has been no attempt to grasp the key,
        // it continues to swing
        ++keyIndex %= keyFrames;
    }
}

// enemies evolve autonomously in the game
void Game::updateCreature() {
    // if the player is in a potentially vulnerable situation in relation to his enemies...
    if (!player->isUnlocking() && !player->isComingDown() && !player->isFalling()) {

        // if he has actually been bitten,
        // it is a failure and the corresponding
        // sound effect has been triggered...
        if (creature->hasBittenJunior) {

            uint32_t elapsedTime = gb.frameCount - creature->bittenTime;

            // as soon as the sound effect has ended...
            if (Buzzer::hasRepeated) {
                // we reset the situation for a new beginning,
                // as in the case of a fall
                creature->hasBittenJunior = false;
                player->reset();
                creature->clear();
                missChecked = false;
                // if the number of failures reaches the fatal limit, the game ends
                if (miss == lives) stopGame();
            // otherwise, if the failure has not yet been taken into account...
            } else if (!missChecked) {
                // it's time to do it!
                miss++;
                missChecked = true;
            }
        // and if the player has not been bitten...
        } else if (gb.frameCount % (tick >> 1) == 0) {
            // enemies continue to move normally
            creature->update(state == GameState::Playing);
        }
    }
}

// the exotic fruit can be used by the player to
// defend himself from his enemies, but this procedure
// is delegated to a specialized module
void Game::updateFruit() {
    if (gb.frameCount % (tick >> 2) == 0) {
        fruit->update();
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
// The rendering calculation is, in the first instance, delegated to a
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

void Game::draw() {
    // this is why we pass a callback reference to the game engine
    // when we invoke the rendering engine
    renderer->draw(this);
}

// this method will be used by the rendering engine to ask the game engine
// to draw the elements of the game scene on the current slice
void Game::drawOnSlice(uint8_t sliceY, uint16_t * buffer) {
    // first, we draw the background scenery of the game scene
    drawStageOnSlice(sliceY, buffer);
    // and when we are no longer on the boot screen...
    if (state != GameState::Boot) {
        // we draw all the sprites present on the slice
        drawSpritesOnSlice(sliceY, buffer);
        creature->drawSpritesOnSlice(renderer, sliceY, buffer);
        // and finally the score counter at the top right of the screen
        drawScoreOnSlice(sliceY, buffer);
    }
}

// graphic rendering of the background scenery
void Game::drawStageOnSlice(uint8_t sliceY, uint16_t * buffer) {
    uint8_t w = Renderer::ScreenWidth;
    // the graphical data of the background is determined
    // according to whether we are in the boot phase or not
    // (the background is not the same)
    uint16_t * colormap = (uint16_t *)(state == GameState::Boot ? STAGE_COLORMAP + w * Renderer::ScreenHeight : STAGE_COLORMAP);
    // the display of the background does not involve any transparent pixels.
    // it is therefore much smarter and faster to proceed by copying
    // memory blocks of bytes with the `memcpy` function of the C language.
    memcpy(buffer, colormap + sliceY * w, 2 * w * Renderer::SliceHeight);
}

// graphic rendering of all sprites present on the slice
void Game::drawSpritesOnSlice(uint8_t sliceY, uint16_t * buffer) {
    uint8_t i;

    // Mario stands still no matter what happens
    renderer->drawSpriteOnSlice(marioSprite, sliceY, buffer);

    // if Junior has been bitten, apply a blinking effect,
    // otherwise the sprite is drawn normally
    if (!creature->hasBittenJunior || gb.frameCount % FrameRate < FrameRate >> 1) {
        renderer->drawSpriteOnSlice(juniorSprite[player->spriteIndex], sliceY, buffer);
    }

    // draws the exotic fruit
    if (fruit->appeared) {
        renderer->drawSpriteOnSlice(fruitSprite[fruit->spriteIndex], sliceY, buffer);
    }

    // draws Junior's arm that unlocks the cage
    if (player->isUnlocking()) {
             if (player->hasArmUp())   renderer->drawSpriteOnSlice(juniorSprite[ArmUp], sliceY, buffer);
        else if (player->hasArmDown()) renderer->drawSpriteOnSlice(juniorSprite[ArmDown], sliceY, buffer);
    }

    // draws the swinging key
    if (!player->isUnlocking() && !player->isComingDown()) {
        renderer->drawSpriteOnSlice(keySprite[keyFrame[keyIndex]], sliceY, buffer);
    }

    // when Papa has been delivered...
    if (unlocked == cageParts) {
        // draws him smiling with blinking effect
        if (gb.frameCount % FrameRate < FrameRate >> 1) {
            renderer->drawSpriteOnSlice(smileSprite, sliceY, buffer);
        }
    // otherwise...
    } else {
        // draws the cage parts...
        for (i = 0; i < cageParts; i++) {
            // with blinking effect on unlocked parts
            if (i >= unlocked || gb.frameCount % FrameRate < FrameRate >> 1) {
                renderer->drawSpriteOnSlice(cageSprite[i], sliceY, buffer);
            }
        }
    }

    // draws the game type information (Game A | Game B)
    if (state == GameState::Playing) {
        renderer->drawSpriteOnSlice(infoSprite[type == GameType::GameA ? InfoGameA : InfoGameB], sliceY, buffer);
    } else if (state == GameState::Demo && displayState == DisplayState::Score) {
        switch (scoreState) {
            case ScoreState::HighScoreA:
                renderer->drawSpriteOnSlice(infoSprite[InfoGameA], sliceY, buffer);
                break;
            case ScoreState::HighScoreB:
                renderer->drawSpriteOnSlice(infoSprite[InfoGameB], sliceY, buffer);
                break;
        }
    }

    // draws the missing lives (the failures)
    if (miss) {
        renderer->drawSpriteOnSlice(infoSprite[InfoMiss], sliceY, buffer);
        for (i = 0; i < miss; i++) renderer->drawSpriteOnSlice(headSprite[i], sliceY, buffer);
    }
}

// graphic rendering of the score counter
void Game::drawScoreOnSlice(uint8_t sliceY, uint16_t * buffer) {

    // the value that will be displayed
    uint16_t value;

    switch (displayState) {
        // if it is a question of displaying a score...
        case DisplayState::Score:
            switch (scoreState) {
                // the value is the current score is displayed
                case ScoreState::Score:
                    value = score->getValue();
                    break;
                // the value is the best score is displayed for Game A
                case ScoreState::HighScoreA:
                    value = score->getHighA();
                    break;
                // the value is the best score is displayed for Game B
                case ScoreState::HighScoreB:
                    value = score->getHighB();
                    break;
            }
            break;
        // otherwise, if we want to display the CPU load
        case DisplayState::CPU:
            value = gb.getCpuLoad();
            break;
        // otherwise, if we want to display the memory space still available
        case DisplayState::RAM:
            value = gb.getFreeRam();
            break;
    }

    // the number that represents the value to be displayed
    // will be decomposed into digits, and each digit will be
    // displayed as a simple sprite
    String   s = String(value);
    uint8_t  n = s.length();
    for (uint8_t i = 0; i < n; i++) renderer->drawDigitOnSlice(s.charAt(i) - '0', n - i - 1, sliceY, buffer);
}

// -------------------------------------------------------------------------
// an example for another method of decomposing the number to be displayed:
// but this technique is to be avoided because it's too CPU-intensive!  😂
// -------------------------------------------------------------------------
// void Game::drawScoreOnSlice(uint8_t sliceY, uint16_t * buffer) {
//     uint16_t value = score->getValue();
//
//     if (value == 0) {
//         renderer->drawDigitOnSlice(0, 0, sliceY, buffer);
//         return;
//     }
//
//     uint8_t  size = 1 + log10(value);
//     uint16_t rem  = value;
//     uint8_t  div;
//     uint16_t pwr;
//     while (size) {
//         pwr = pow(10, size - 1);
//         div = rem / pwr;
//         rem -= div * pwr;
//         size--;
//         renderer->drawDigitOnSlice(div, size, sliceY, buffer);
//     }
// }