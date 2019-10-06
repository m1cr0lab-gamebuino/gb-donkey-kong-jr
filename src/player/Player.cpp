// -------------------------------------------------------------------------
//                               Donkey Kong Jr
//           a Nintendo's Game & Watch adaptation on Gamebuino META
// -------------------------------------------------------------------------
//                                © 2019 Steph
//                        https://gamebuino.com/@steph
// -------------------------------------------------------------------------
//                              Players' avatar
// -------------------------------------------------------------------------

// loads the header file of the class
#include "Player.h"

// loads external modules
#include "../graphics/sprites.h" // loads the description of all sprites
#include "../sounds/Buzzer.h"    // loads the definition of the sound effects controller

// constructor
Player::Player() {
    reset();
}

// resets the avatar
void Player::reset() {
    armState        = ArmState::None;
    fallState       = FallState::None;
    spriteIndex     = Bottom1;
    spriteLastIndex = Bottom1;
    startupTime     = gb.frameCount;
    timer           = 0;
}

// moves the avatar in one direction, possibly emitting a sound
void Player::move(const int8_t * direction, bool withSound) {
    // determines the next posture of the avatar
    int8_t next = direction[spriteIndex];
    // and if this posture is allowed...
    if (next != Forbidden) {
        // stores the current posture
        spriteLastIndex = spriteIndex;
        // and takes the new posture
        spriteIndex = next;
        // if a jump has been initiated,
        // the time at which it occurs is saved
        if (isJumping()) timer = gb.frameCount;
        // if a sound is to be played, now is the time to do so
        if (withSound) Buzzer::play(Sound::Move);
    }
}

// attempt to grab the key
void Player::grab() {
    spriteIndex = GrabKey;     // the correct posture is set
    timer = gb.frameCount;     // the time at which it occurs is saved
    Buzzer::play(Sound::Move); // and the corresponding sound effect is played
}

// engages the cage unlocking
void Player::unlock(ArmState state) {
    armState    = state;         // the first unlocking step is set
    spriteIndex = Unlock;        // the correct posture is set
    timer       = gb.frameCount; // and the time at which it occurs is saved
}

// falls to the ground after the cage is unlocked
void Player::comeDown() {
    armState    = ArmState::None; // the second unlocking step is set
    spriteIndex = ComeDown;       // the correct posture is set
    timer       = gb.frameCount;  // and the time at which it occurs is saved
}

// triggers the fall after a failed unlocking
// and set the appropriate step of the fall
void Player::fall(FallState state) {
    // set the appropriate step of the fall
    fallState = state;
    // depending on this step....
    switch (state) {
        case FallState::Up:
            spriteIndex = Fall1; // the correct posture is set
            break;
        case FallState::Down:
            spriteIndex = Fall2; // the correct posture is set
            // and a sound effect is played on the ending step
            Buzzer::repeat(Sound::Lost, 5, 8);
            break;
    }
}

// determines if the avatar is jumping
bool Player::isJumping() {
    return spriteIndex == BottomJump2 ||
           spriteIndex == BottomJump5 ||
           spriteIndex == TopJump2    ||
           spriteIndex == TopJump3;
}

// determines if the avatar is trying to catch the unlocking key
bool Player::isGrabbing() {
    return spriteIndex == GrabKey;
}

// determines if the avatar is unlocking the cage
bool Player::isUnlocking() {
    return spriteIndex == Unlock;
}

// determines if the avatar is brandishing the key just before unlocking
bool Player::hasArmUp() {
    return armState == ArmState::Up;
}

// determines if the avatar is inserting the key to unlock the cage
bool Player::hasArmDown() {
    return armState == ArmState::Down;
}

// determines if the avatar is descending with a successful unlocking
bool Player::isComingDown() {
    return spriteIndex == ComeDown;
}

// determines if the avatar is falling after an unlocking failure
bool Player::isFalling() {
    return fallState == FallState::Up || fallState == FallState::Down;
}

// determines if the avatar is in the upper position of the fall
bool Player::isFallingUp() {
    return fallState == FallState::Up;
}

// determines if the avatar is in the lower position of the fall
bool Player::isFallingDown() {
    return fallState == FallState::Down;
}