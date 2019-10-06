// -------------------------------------------------------------------------
//                               Donkey Kong Jr
//           a Nintendo's Game & Watch adaptation on Gamebuino META
// -------------------------------------------------------------------------
//                                © 2019 Steph
//                        https://gamebuino.com/@steph
// -------------------------------------------------------------------------
//                              Players' avatar
// -------------------------------------------------------------------------

#ifndef DONKEY_KONG_JR_PLAYER
#define DONKEY_KONG_JR_PLAYER

// loads the official library
#include <Gamebuino-Meta.h>

// defines the steps for unlocking the cage
enum class ArmState : uint8_t {
    None,
    Up,
    Down
};

// defines the steps of the fall in case of failure
// after an attempt to grasp the unlocking key
enum class FallState : uint8_t {
    None,
    Up,
    Down
};

// the avatar definition
class Player
{
    public:

        ArmState  armState;        // cage unlocking step
        FallState fallState;       // fall step
        int8_t    spriteIndex;     // index of the image to display for sprite animation
        int8_t    spriteLastIndex; // last known index of the image to be displayed for sprite animation
        uint32_t  startupTime;     // start timecode at the bottom of the screen
        uint32_t  timer;           // internal clock for event synchronization

        Player(); // constructor

        void reset(); // resets the avatar

        // moves the avatar in one direction, possibly emitting a sound
        void move(const int8_t * direction, bool withSound = true);

        void grab();                 // attempt to grab the key
        void unlock(ArmState state); // engages the cage unlocking
        void comeDown();             // falls to the ground after the cage is unlocked
        void fall(FallState state);  // triggers the fall after a failed unlocking

        bool isJumping();     // determines if the avatar is jumping
        bool isGrabbing();    // determines if the avatar is trying to catch the unlocking key
        bool isUnlocking();   // determines if the avatar is unlocking the cage
        bool hasArmUp();      // determines if the avatar is brandishing the key just before unlocking
        bool hasArmDown();    // determines if the avatar is inserting the key to unlock the cage
        bool isComingDown();  // determines if the avatar is descending with a successful unlocking
        bool isFalling();     // determines if the avatar is falling after an unlocking failure
        bool isFallingUp();   // determines if the avatar is in the upper position of the fall
        bool isFallingDown(); // determines if the avatar is in the lower position of the fall
};

#endif