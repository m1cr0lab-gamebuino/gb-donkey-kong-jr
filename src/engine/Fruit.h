// -------------------------------------------------------------------------
//                               Donkey Kong Jr
//           a Nintendo's Game & Watch adaptation on Gamebuino META
// -------------------------------------------------------------------------
//                                © 2019 Steph
//                        https://gamebuino.com/@steph
// -------------------------------------------------------------------------
//                                Exotic Fruit
// -------------------------------------------------------------------------

#ifndef DONKEY_KONG_JR_FRUIT
#define DONKEY_KONG_JR_FRUIT

// loads external modules
#include "../player/Player.h"   // handles the player's avatar behaviour
#include "../engine/Creature.h" // handles the enemies
#include "../data/Score.h"      // handles the score and the saving of highscore

#include <Gamebuino-Meta.h>

class Fruit
{
    private:

        Player   * player;   // reference to the player's avatar
        Creature * creature; // reference to the module which controls enemies
        Score    * score;    // reference to the score management module

    public:

        bool    appeared;    // fruit appearance indicator
        uint8_t spriteIndex; // index of the image to display for sprite animation

        // constructor
        Fruit(Player * player, Creature * creature, Score * score) :
            appeared(false),
            spriteIndex(0),
            player(player),
            creature(creature),
            score(score)
            {}

        void reset();  // resets the fruit state
        void update(); // controls the fruit behaviour
};

#endif