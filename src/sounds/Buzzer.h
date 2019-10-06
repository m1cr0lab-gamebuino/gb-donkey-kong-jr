// -------------------------------------------------------------------------
//                               Donkey Kong Jr
//           a Nintendo's Game & Watch adaptation on Gamebuino META
// -------------------------------------------------------------------------
//                                © 2019 Steph
//                        https://gamebuino.com/@steph
// -------------------------------------------------------------------------
//                          Sound Effects Controller
// -------------------------------------------------------------------------
// I had originally planned to use sounds in WAV format for the game's sound
// effects... but I quickly changed my mind and found that they consumed too
// much CPU at the runtime.
// 
// So I decided to generate procedural sounds, much less greedy, and I used
// the great Valden (https://gamebuino.com/@valden) tool available at these
// addresses:
// https://gamebuino.com/creations/gamebuino-sound-fx-creation-tool-gsfx-gui
// https://github.com/EspeuteClement/GSFX_GUI
// 
// Thank you Valden for this tool which made my task much easier  👍
// -------------------------------------------------------------------------

#ifndef DONKEY_KONG_JR_BUZZER
#define DONKEY_KONG_JR_BUZZER

// loads the official library
#include <Gamebuino-Meta.h>

// the procedural sound effects I set up with the Valden tool
constexpr Gamebuino_Meta::Sound_FX FxTick[]  = {{Gamebuino_Meta::Sound_FX_Wave::SQUARE,0, 128, -100, -128, 126,  1}};
constexpr Gamebuino_Meta::Sound_FX FxMove[]  = {{Gamebuino_Meta::Sound_FX_Wave::SQUARE,0, 128,  -32,    0,  32,  2}};
constexpr Gamebuino_Meta::Sound_FX FxScore[] = {{Gamebuino_Meta::Sound_FX_Wave::SQUARE,0, 128, -128,    0,   5,  1}};
constexpr Gamebuino_Meta::Sound_FX FxLost[]  = {{Gamebuino_Meta::Sound_FX_Wave::SQUARE,0, 128,   -1,    0, 189, 10}};

// naming of sound effects to facilitate their management
enum class Sound : uint8_t {
    None,
    Tick,
    Move,
    Score,
    Lost
};

// the controller definition
class Buzzer
{
    private:
    
        static Sound    toRepeat; // the sound effect to be repeated at regular intervals
        static uint8_t  counter;  // counts the number of repetitions already performed
        static uint32_t timer;    // internal clock
        static uint8_t  delay;    // delay that punctuates the start of playback for each repetition
        static bool     mute;     // indicates whether or not the sound is muted

    public:

        static bool hasRepeated; // indicates that the repetition of a sound effect is over

        // control loop
        static void update();
        // plays a sound without delay
        static void play(Sound sound);
        // triggers the repetition of a sound at regular intervals
        static void repeat(Sound sound, uint8_t count, uint8_t delay);
        // switch the mute indicator
        static void toggleMute();
};

#endif