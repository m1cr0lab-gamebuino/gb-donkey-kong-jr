// -------------------------------------------------------------------------
//                               Donkey Kong Jr
//           a Nintendo's Game & Watch adaptation on Gamebuino META
// -------------------------------------------------------------------------
//                                © 2019 Steph
//                        https://gamebuino.com/@steph
// -------------------------------------------------------------------------
//                                Exotic Fruit
// -------------------------------------------------------------------------

// loads the header file of the class
#include "Fruit.h"

// resets the fruit state
void Fruit::reset() {
    appeared    = false;
    spriteIndex = 0;
}

// controls the fruit behaviour
void Fruit::update() {

    // if the fruit appeared on the game scene...
    if (appeared) {
        // and depending on the animation stage...
        switch (spriteIndex) {
            // the fruit is still hanging on the tree
            case 0:
                // and Junior just jumped to knock it down
                if (player->spriteIndex == TopJump2) spriteIndex++;
                break;
            // the fruit falls and is at the first level
            case 1:
                creature->tryToKillCrocAt(2);
                spriteIndex++;
                break;
            // the fruit falls and is at the second level
            case 2:
                creature->tryToKillBirdAt(4);
                spriteIndex++;
                break;
            // the fruit falls and is at the third level
            case 3:
                creature->tryToKillCrocAt(9);
                // he ends his fall and disappears too
                reset();
                break;
        }
    }
    
    // if the fruit is not yet present on the game scene...
    else {
        // it is eventually made to appear
        appeared    = random(24) == 0;
        spriteIndex = 0;
    }
}