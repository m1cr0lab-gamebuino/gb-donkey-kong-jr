// -------------------------------------------------------------------------
//                               Donkey Kong Jr
//           a Nintendo's Game & Watch adaptation on Gamebuino META
// -------------------------------------------------------------------------
//                                © 2019 Steph
//                        https://gamebuino.com/@steph
// -------------------------------------------------------------------------
//                                 Game Engine
// -------------------------------------------------------------------------

#ifndef DONKEY_KONG_JR_GAME
#define DONKEY_KONG_JR_GAME

// loads external modules
#include "Creature.h"              // handles birds and snapjaws (ennemies)
#include "Fruit.h"                 // handles the use of exotic fruit as a defence
#include "../player/Player.h"      // handles the player avatar behaviour
#include "../graphics/Renderer.h"  // graphics rendering engine
#include "../data/Score.h"         // handles the score and the saving of highscore

// loads the official library
#include <Gamebuino-Meta.h>

// defines the console states
enum class GameState : uint8_t {
    Boot,
    Demo,
    Playing
};

// defines the types of games
enum class GameType : uint8_t {
    GameA,
    GameB
};

// defines the nature of the information to be displayed
// on the counter at the top right of the screen
enum class DisplayState : uint8_t {
    Score,
    CPU,
    RAM
};

// defines which score should be displayed
enum class ScoreState : uint8_t {
    Score,      // current / last score
    HighScoreA, // high score for Game A
    HighScoreB  // high score for Game B
};

// the game engine definition
class Game
{
    private:

        static constexpr uint8_t  minTick         = 8;   // lower bound of the metronome (in number of frames)
        static constexpr uint8_t  lives           = 3;   // number of lives available for the player
        static constexpr uint8_t  cageParts       = 4;   // the number of cage elements to be unlocked
        static constexpr uint8_t  minRewardUnlock = 5;   // minimum reward for unlocking
        static constexpr uint8_t  maxRewardUnlock = 20;  // maximum reward for unlocking
        static constexpr uint16_t minTimeToUnlock = 150; // elapsed time corresponding to the minimum reward (in number of frames)
        static constexpr uint16_t maxTimeToUnlock = 250; // elapsed time corresponding to the maximum reward (in number of frames)
        static constexpr uint8_t  keyFrames       = 7;   // number of frames in the animation sequence of the unlocking key
        static const     uint8_t  keyFrame[keyFrames];   // animation sequence of the unlocking key

        GameState    state;        // the console state (Boot | Demo | Playing)
        GameType     type;         // the type of game (GameA | GameB)
        DisplayState displayState; // type of information to be displayed on the counter (Score | CPU | RAM)
        ScoreState   scoreState;   // nature of the score to be displayed on the counter (current / last or high scores)

        uint32_t scoreRollTime; // last timecode for switching score displays (curent / last and high scores)

        uint8_t  tick;        // number of frames corresponding to the beat of the metronome
        uint8_t  keyIndex;    // index of the current image for the animation of the unlocking key
        uint8_t  miss;        // number of failures
        uint8_t  unlocked;    // number of cage parts that have been unlocked
        bool     missChecked; // flag indicating that the last failure has been taken into account

        Score    * score;    // score management module
        Player   * player;   // player avatar management module
        Creature * creature; // ennemy management module
        Fruit    * fruit;    // exotic fruit management module
        Renderer * renderer; // graphics rendering engine

        // player interactions
        void waitForSoundToggle();   // monitors the activation or deactivation of sound effects
        void waitForDisplayToggle(); // monitors the display switchover at the counter (score | CPU | RAM)
        void waitForStarting();      // monitors the launch of a game
        void waitForMoving();        // monitors the behaviour of the player's avatar

        void demo();      // controls the progress of the game during the demonstration phase
        void rollScore(); // controls the permutation of the information to be displayed on the counter

        void startGame(GameType type); // launches a new game
        void stopGame();               // ends the current game

        void updateBootMode();        // controls the temporary boot phase
        void updateJuniorJumping();   // controls the player's avatar during a jump
        void updateJuniorUnlocking(); // controls the player's avatar when unlocking the cage
        void updateJuniorFalling();   // controls the player's avatar during a fall
        void updateKey();             // controls the swing of the unlocking key
        void updateCreature();        // controls the interactions between the player's avatar and enemies
        void updateFruit();           // controls the use of exotic fruit

        void draw();                                                // starts the graphic rendering of the game scene
        void drawStageOnSlice(uint8_t sliceY, uint16_t * buffer);   // renders the graphic background of the game scene on the current screen slice
        void drawSpritesOnSlice(uint8_t sliceY, uint16_t * buffer); // renders all sprites on the current screen slice
        void drawScoreOnSlice(uint8_t sliceY, uint16_t * buffer);   // renders the score on the current screen slice

    public:

        // overall display frequency of the game
        static constexpr uint8_t FrameRate = 25;

        Game();  // constructor
        ~Game(); // destructor

        void setup(); // game engine initialization
        void loop();  // main control looop

        // triggers the graphic rendering of the game scene on a slice of the screen
        void drawOnSlice(uint8_t sliceY, uint16_t * buffer);
};

#endif