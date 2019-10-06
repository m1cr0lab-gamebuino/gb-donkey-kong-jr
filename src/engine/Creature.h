// -------------------------------------------------------------------------
//                               Donkey Kong Jr
//           a Nintendo's Game & Watch adaptation on Gamebuino META
// -------------------------------------------------------------------------
//                                © 2019 Steph
//                        https://gamebuino.com/@steph
// -------------------------------------------------------------------------
//                           Enemy Management Module
// -------------------------------------------------------------------------
// The enemies Junior will meet on his way are birds and snapjaws that will
// try to bite him when he crosses their paths. The modeling of enemies will
// be done here by simply defining their respective paths throughout the
// game scene using positional arrays.
// 
// Each cell in the array will describe whether an enemy is in this position,
// taking into account the pace driven by the metronome of the game engine.
// -------------------------------------------------------------------------

#ifndef DONKEY_KONG_JR_CREATURE
#define DONKEY_KONG_JR_CREATURE

// loads external modules
#include "../graphics/Renderer.h" // graphics rendering engine
#include "../graphics/sprites.h"  // descriptive data of sprites
#include "../player/Player.h"     // handles the player's avatar behaviour
#include "../data/Score.h"        // handles the score and the saving of highscore

// loads the official library
#include <Gamebuino-Meta.h>

// spatial-temporal descriptive unit
// of the presence of an enemy
struct Beat {
    bool exists; // determines if the enemy exists at this position
    bool now;    // determines if its presence is correlated with the metronome of the game engine
};

// the module definition
class Creature
{
    private:
        
        // rewards obtained when Junior eliminates an enemy with the help of exotic fruit
        static constexpr uint8_t rewardWhenKilledBird       = 6;
        static constexpr uint8_t rewardWhenKilledTopCroc    = 3;
        static constexpr uint8_t rewardWhenKilledBottomCroc = 9;

        static constexpr uint8_t birdsSize = 8;  // path length of the birds
        static constexpr uint8_t crocsSize = 13; // path length of the snapjaws

        Beat   birds[birdsSize] = { {false, false} }; // positional bird array
        Beat   crocs[crocsSize] = { {false, false} }; // positional snapjaw array

        bool   paceMaker; // synchronization indicator with the metronome of the game engine
        bool   bip;       // indicates whether a sound effect should be played when moving enemies
        bool   isPlaying; // indicates whether the game has been launched

        Player * junior;  // reference to the player's avatar
        Score  * score;   // reference to the score management module

        void updateBirds(); // controls bird progression
        void updateCrocs(); // controls snapjaw progression

        void spawnBird(); // makes a new bird appear
        void spawnCroc(); // makes a new snapjaw appear

        void moveBirdForward(); // moves the birds forward
        void moveCrocForward(); // moves the snapjaws forward

    public:

        bool     hasBittenJunior; // indicates whether Junior has been bitten
        uint32_t bittenTime;      // the time at which it occurs

        // constructor
        Creature(Player * player, Score * score) :
            junior(player),
            score(score),
            paceMaker(false),
            bip(false),
            isPlaying(false),
            hasBittenJunior(false),
            bittenTime(0)
            {}

        void reset(); // clears all enemies
        void clear(); // deletes enemies too close to Junior's starting point

        bool isThereAnyBirdNearJunior(int8_t i); // determines the presence of a bird at the specified position
        bool isThereAnyCrocNearJunior(int8_t i); // determines the presence of a snapjaw at the specified position

        void tryToKillBirdAt(uint8_t i); // attempts to destroy a bird at the specified position
        void tryToKillCrocAt(uint8_t i); // attempts to destroy a snapjaw at the specified position

        // controls the progress of enemies
        void update(bool isPlaying);

        // renders the sprites of all the enemies present on the current screen slice
        void drawSpritesOnSlice(Renderer * renderer, uint8_t sliceY, uint16_t * buffer);
};

#endif