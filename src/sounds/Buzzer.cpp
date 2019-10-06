// -------------------------------------------------------------------------
//                               Donkey Kong Jr
//           a Nintendo's Game & Watch adaptation on Gamebuino META
// -------------------------------------------------------------------------
//                                © 2019 Steph
//                        https://gamebuino.com/@steph
// -------------------------------------------------------------------------
//                          Sound Effects Controller
// -------------------------------------------------------------------------

// loads the header file of the class
#include "Buzzer.h"

// initializes the properties of the controller
Sound    Buzzer::toRepeat    = Sound::None;
uint8_t  Buzzer::counter     = 0;
uint32_t Buzzer::timer       = 0;
uint8_t  Buzzer::delay       = 0;
bool     Buzzer::mute        = false;
bool     Buzzer::hasRepeated = false;

// control loop
void Buzzer::update() {
    // if there are any sounds left to play...
    if (counter > 0) {
        // and once the break is over...
        if (gb.frameCount - timer > delay) {
            // we continue the replay
            repeat(toRepeat, counter, delay);
        }
    // otherwise, the repetitive loop is interrupted
    } else if (!hasRepeated && toRepeat != Sound::None && gb.frameCount - timer > delay) {
        hasRepeated = true;
        toRepeat    = Sound::None;
    }
}

// plays a specific sound without delay
void Buzzer::play(Sound sound) {
    // only if the sound has not been muted
    if (mute) return;

    switch (sound) {
        case Sound::Tick:
            gb.sound.fx(FxTick);
            break;
        case Sound::Move:
            gb.sound.fx(FxMove);
            break;
        case Sound::Score:
            gb.sound.fx(FxScore);
            break;
        case Sound::Lost:
            gb.sound.fx(FxLost);
            break;
    }
}

// initiates the repetition of a sound effect with a regular interval
void Buzzer::repeat(Sound sound, uint8_t count, uint8_t delay) {
    play(sound);
    toRepeat      = sound;
    counter       = count - 1;
    timer         = gb.frameCount;
    Buzzer::delay = delay;
    hasRepeated   = false;
}

// switch the mute indicator
void Buzzer::toggleMute() {
    mute = !mute;
}

// -------------------------------------------------------------------------
// WAV sounds consume toooooo much CPU...  🙁  set up FX are much better!
// -------------------------------------------------------------------------
// I left the WAV sounds on the repository so you could eventually test the
// difference. But you'll see that you'll give up the idea like me...
// 
// When installing the game, you will need to copy the `sounds` folder to
// the root of the game folder.
// -------------------------------------------------------------------------
// void Buzzer::play(Sound sound) {
//     switch (sound) {
//         case Sound::Tick:
//             gb.sound.play("sounds/tick.wav");
//             break;
//         case Sound::Move:
//             gb.sound.play("sounds/move.wav");
//             break;
//         case Sound::Score:
//             gb.sound.play("sounds/score.wav");
//             break;
//         case Sound::Lost:
//             gb.sound.play("sounds/lost.wav");
//             break;
//     }
// }