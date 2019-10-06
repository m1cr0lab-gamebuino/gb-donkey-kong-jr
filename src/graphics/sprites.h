// -------------------------------------------------------------------------
//                               Donkey Kong Jr
//           a Nintendo's Game & Watch adaptation on Gamebuino META
// -------------------------------------------------------------------------
//                                © 2019 Steph
//                        https://gamebuino.com/@steph
// -------------------------------------------------------------------------
//                      Descriptive Data of the Sprites
// -------------------------------------------------------------------------
// The spritesheet incorporates all the sprites of the game in their actual
// posture. However, it so happens that, under these conditions, the
// rectangular envelopes of sprites necessarily overlap. This poses a
// problem when when rendering the graphics, all the pixels of the sprites
// are scanned. How to determine if the pixel is part of the sprite in which
// we are interested, or to one of its neighbors?
// 
// To remove this indeterminacy, the sprites were colored on the spritesheet,
// so that they could be distinguished during a scan. It is therefore
// necessary here to specify, for each sprite, which color has been assigned
// to it on the spritesheet.
// 
// You can take a look at the original image of the spritesheet to see how
// the coloring was done (file: `assets/spritesheet.png`).
// -------------------------------------------------------------------------

#ifndef DONKEY_KONG_JR_SPRITES
#define DONKEY_KONG_JR_SPRITES

// loads the official library
#include <Gamebuino-Meta.h>

// -------------------------------------------------------------------------
// Sprite definition
// -------------------------------------------------------------------------

struct Sprite {
    uint8_t  x;      // the horizontal coordinate of the sprite on the spritesheet
    uint8_t  y;      // the vertical coordinate of the sprite on the spritesheet
    uint8_t  width;  // the width of the sprite
    uint8_t  height; // the height of the sprite
    uint16_t color;  // the color assigned to it on the spritesheet
};

// -------------------------------------------------------------------------
// Game indicators
// -------------------------------------------------------------------------

constexpr uint8_t InfoGameA = 0;
constexpr uint8_t InfoGameB = 1;
constexpr uint8_t InfoMiss  = 2;

constexpr Sprite infoSprite[] = {
    {  92,  3, 23, 5, 0x0000 }, // InfoGameA
    {  92, 13, 23, 5, 0x0000 }, // InfoGameB
    { 108, 23, 15, 5, 0x0000 }  // InfoMiss
};

constexpr Sprite headSprite[] = {
    { 126, 20, 9, 11, 0x0000 },
    { 136, 20, 9, 11, 0x0000 },
    { 146, 20, 9, 11, 0x0000 }
};

// -------------------------------------------------------------------------
// Papa's captivity
// -------------------------------------------------------------------------

constexpr Sprite marioSprite = { 5, 49, 13, 19, 0x1f00 };

constexpr Sprite keySprite[] = {
    { 51, 11,  6, 20, 0xe0fe },
    { 58,  9,  5, 22, 0xe0fe },
    { 62, 11,  8, 17, 0x60fe },
    { 65,  9, 13, 14, 0x40fc }
};

constexpr Sprite cageSprite[] = {
    { 18,  3, 12, 18, 0x1f00 },
    {  3,  3, 12, 18, 0x1f00 },
    {  3, 24, 12, 17, 0x0000 },
    { 18, 24, 19, 17, 0x1f00 }
};

constexpr Sprite smileSprite = { 12, 15, 9, 8, 0x0000 };

// -------------------------------------------------------------------------
// Fruit
// -------------------------------------------------------------------------

constexpr Sprite fruitSprite[] = {
    { 83,  29, 9, 11, 0xe007 },
    { 83,  45, 7, 19, 0xe007 },
    { 82,  78, 9, 16, 0xe007 },
    { 81, 101, 9, 12, 0xe007 }
};

// -------------------------------------------------------------------------
// Junior
// -------------------------------------------------------------------------

// here we will name the avatar's postures to facilitate the reading of the
// directional tables that will condition his movements
constexpr int8_t Forbidden   = -1;
constexpr int8_t Bottom1     =  0;
constexpr int8_t Bottom2     =  1;
constexpr int8_t Bottom3     =  2;
constexpr int8_t Bottom4     =  3;
constexpr int8_t Bottom5     =  4;
constexpr int8_t Bottom6     =  5;
constexpr int8_t BottomJump1 =  6;
constexpr int8_t BottomJump2 =  7;
constexpr int8_t BottomJump3 =  8;
constexpr int8_t BottomJump4 =  9;
constexpr int8_t BottomJump5 = 10;
constexpr int8_t BottomJump6 = 11;
constexpr int8_t Top1        = 12;
constexpr int8_t Top2        = 13;
constexpr int8_t Top3        = 14;
constexpr int8_t Top4        = 15;
constexpr int8_t TopJump1    = 16;
constexpr int8_t TopJump2    = 17;
constexpr int8_t TopJump3    = 18;
constexpr int8_t GrabKey     = 19;
constexpr int8_t Unlock      = 20;
constexpr int8_t ArmUp       = 21;
constexpr int8_t ArmDown     = 22;
constexpr int8_t ComeDown    = 23;
constexpr int8_t Fall1       = 24;
constexpr int8_t Fall2       = 25;

constexpr Sprite juniorSprite[] = {
    {  17, 102, 19, 21, 0x0000 }, // Bottom1,
    {  42, 104, 18, 19, 0x0000 }, // Bottom2,
    {  62, 103, 20, 20, 0x0000 }, // Bottom3,
    {  87, 103, 19, 20, 0x0000 }, // Bottom4,
    { 109, 104, 21, 19, 0x0000 }, // Bottom5,
    { 130, 104, 18, 19, 0x0000 }, // Bottom6,
    {  20,  84, 16, 19, 0x3f8a }, // BottomJump1
    {  42,  85, 19, 20, 0x3f8a }, // BottomJump2
    {  67,  84, 13, 19, 0x0000 }, // BottomJump3
    {  91,  85, 13, 18, 0x0000 }, // BottomJump4
    { 110,  83, 18, 21, 0x0000 }, // BottomJump5
    { 132,  82, 18, 21, 0x0000 }, // BottomJump6
    { 131,  56, 19, 19, 0x0000 }, // Top1,
    { 108,  56, 20, 19, 0x0000 }, // Top2,
    {  88,  55, 19, 20, 0x0000 }, // Top3,
    {  66,  53, 18, 21, 0x0000 }, // Top4,
    { 113,  38, 13, 19, 0x3f8a }, // TopJump1,
    {  89,  35, 19, 21, 0x3f8a }, // TopJump2,
    {  67,  35, 20, 17, 0x0000 }, // TopJump3,
    {  50,  30, 19, 29, 0x3f8a }, // GrabKey,
    {  37,  14, 12, 22, 0x3f8a }, // Unlock,
    {  44,   2, 10, 18, 0x0000 }, // ArmUp,
    {  32,  14,  7, 13, 0x0000 }, // ArmDown,
    {  32,  37, 20, 27, 0x0000 }, // ComeDown,
    {  11,  63, 24, 24, 0x0000 }, // Fall1,
    {   2,  90, 13, 26, 0x0000 }  // Fall2
};

// -------------------------------------------------------------------------
// Here are precisely the directional tables that will determine the
// avatar's movements.
// -------------------------------------------------------------------------

constexpr int8_t moveLeft[] = {
//  to:             from:
    Forbidden,   // Bottom1
    Bottom1,     // Bottom2
    Bottom2,     // Bottom3
    Bottom3,     // Bottom4
    Bottom4,     // Bottom5
    Bottom5,     // Bottom6
    Forbidden,   // BottomJump1
    Forbidden,   // BottomJump2
    BottomJump2, // BottomJump3
    BottomJump3, // BottomJump4
    Forbidden,   // BottomJump5
    Forbidden,   // BottomJump6
    Top2,        // Top1
    Top3,        // Top2
    Top4,        // Top3
    Forbidden,   // Top4
    Forbidden,   // TopJump1
    Forbidden,   // TopJump2
    Forbidden    // TopJump3
};

constexpr int8_t moveRight[] = {
//  to:             from:
    Bottom2,     // Bottom1
    Bottom3,     // Bottom2
    Bottom4,     // Bottom3
    Bottom5,     // Bottom4
    Bottom6,     // Bottom5
    Forbidden,   // Bottom6
    BottomJump2, // BottomJump1
    Forbidden,   // BottomJump2
    BottomJump4, // BottomJump3
    BottomJump5, // BottomJump4
    Forbidden,   // BottomJump5
    Forbidden,   // BottomJump6
    Forbidden,   // Top1
    Top1,        // Top2
    Top2,        // Top3
    Top3,        // Top4
    Forbidden,   // TopJump1
    Forbidden,   // TopJump2
    Forbidden    // TopJump3
};

constexpr int8_t moveUp[] = {
//  to:             from:
    BottomJump1, // Bottom1
    BottomJump2, // Bottom2
    BottomJump3, // Bottom3
    BottomJump4, // Bottom4
    BottomJump5, // Bottom5
    BottomJump6, // Bottom6
    Forbidden,   // BottomJump1
    Forbidden,   // BottomJump2
    Forbidden,   // BottomJump3
    Forbidden,   // BottomJump4
    Forbidden,   // BottomJump5
    Top1,        // BottomJump6
    Forbidden,   // Top1
    TopJump1,    // Top2
    TopJump2,    // Top3
    TopJump3,    // Top4
    Forbidden,   // TopJump1
    Forbidden,   // TopJump2
    Forbidden    // TopJump3
};

constexpr int8_t moveDown[] = {
//  to:             from:
    Forbidden,   // Bottom1
    Forbidden,   // Bottom2
    Forbidden,   // Bottom3
    Forbidden,   // Bottom4
    Forbidden,   // Bottom5
    Forbidden,   // Bottom6
    Bottom1,     // BottomJump1
    Bottom2,     // BottomJump2
    Bottom3,     // BottomJump3
    Bottom4,     // BottomJump4
    Bottom5,     // BottomJump5
    Bottom6,     // BottomJump6
    BottomJump6, // Top1
    Forbidden,   // Top2
    Forbidden,   // Top3
    Forbidden,   // Top4
    Top2,        // TopJump1
    Top3,        // TopJump2
    Top4         // TopJump3
};

// -------------------------------------------------------------------------
// Creatures
// -------------------------------------------------------------------------

constexpr Sprite crocSprite[] = {
    {  38,  66, 10, 10, 0x00f8 },
    {  57,  67,  8, 10, 0x00f8 },
    {  80,  66, 10,  9, 0x00f8 },
    { 103,  67,  9,  8, 0x00f8 },
    { 124,  66, 10,  9, 0x00f8 },
    { 148,  66,  8, 11, 0x00f8 },
    { 145, 106, 11, 18, 0x00f8 },
    { 123, 115, 10,  8, 0x00f8 },
    { 104, 114, 10,  9, 0x00f8 },
    {  81, 115, 10,  8, 0x00f8 },
    {  57, 114, 10,  9, 0x00f8 },
    {  35, 115, 10,  8, 0x00f8 },
    {  12, 116, 11,  7, 0x00f8 }
};

constexpr Sprite birdSprite[] = {
    {   2, 74,  8, 12, 0x00f8 },
    {  13, 88, 11,  9, 0x00f8 },
    {  35, 88, 11,  9, 0x00f8 },
    {  58, 92, 11,  9, 0x00f8 },
    {  81, 91, 10, 10, 0x00f8 },
    { 104, 91, 10,  8, 0x00f8 },
    { 125, 88, 10,  9, 0x00f8 },
    { 147, 80, 11, 10, 0x00f8 }
};

// -------------------------------------------------------------------------
// We define here which are the postures of the player's avatar that put him
// in a position to be bitten according to the enemies' positional index.
// -------------------------------------------------------------------------

constexpr int8_t bittenByCroc[] = {
//  Junior index || crocSprite[i]
    Forbidden,   //     i = 0
    Top4,        //     i = 1
    Top3,        //     i = 2
    Top2,        //     i = 3
    Top1,        //     i = 4
    Forbidden,   //     i = 5
    Bottom6,     //     i = 6
    Bottom5,     //     i = 7
    Bottom4,     //     i = 8
    Bottom3,     //     i = 9
    Bottom2,     //     i = 10
    Bottom1,     //     i = 11
    Forbidden    //     i = 12
};

constexpr int8_t bittenByBird[] = {
//  Junior index || crocSprite[i]
    Forbidden,   //     i = 0
    BottomJump1, //     i = 1
    BottomJump2, //     i = 2
    BottomJump3, //     i = 3
    BottomJump4, //     i = 4
    BottomJump5, //     i = 5
    BottomJump6, //     i = 6
    Forbidden    //     i = 7
};

constexpr int8_t bittenByCrocWhenMoveLeft[] = {
    -1, // Bottom1
    11, // Bottom2
    10, // Bottom3
     9, // Bottom4
     8, // Bottom5
     7, // Bottom6
    -1, // BottomJump1
    -1, // BottomJump2
    -1, // BottomJump3
    -1, // BottomJump4
    -1, // BottomJump5
    -1, // BottomJump6
     4, // Top1
     3, // Top2
     2, // Top3
    -1, // Top4
    -1, // TopJump1
    -1, // TopJump2
    -1  // TopJump3
};

constexpr int8_t bittenByCrocWhenMoveRight[] = {
    11, // Bottom1
    10, // Bottom2
     9, // Bottom3
     8, // Bottom4
     7, // Bottom5
    -1, // Bottom6
    -1, // BottomJump1
    -1, // BottomJump2
    -1, // BottomJump3
    -1, // BottomJump4
    -1, // BottomJump5
    -1, // BottomJump6
    -1, // Top1
     4, // Top2
     3, // Top3
     2, // Top4
    -1, // TopJump1
    -1, // TopJump2
    -1  // TopJump3
};

constexpr int8_t bittenByBirdWhenMoveLeft[] = {
    -1, // Bottom1
    -1, // Bottom2
    -1, // Bottom3
    -1, // Bottom4
    -1, // Bottom5
    -1, // Bottom6
    -1, // BottomJump1
     2, // BottomJump2
     3, // BottomJump3
     4, // BottomJump4
     5, // BottomJump5
     6, // BottomJump6
    -1, // Top1
    -1, // Top2
    -1, // Top3
    -1, // Top4
    -1, // TopJump1
    -1, // TopJump2
    -1  // TopJump3
};

constexpr int8_t bittenByBirdWhenMoveRight[] = {
    -1, // Bottom1
    -1, // Bottom2
    -1, // Bottom3
    -1, // Bottom4
    -1, // Bottom5
    -1, // Bottom6
     2, // BottomJump1
     3, // BottomJump2
     4, // BottomJump3
     5, // BottomJump4
     6, // BottomJump5
    -1, // BottomJump6
    -1, // Top1
    -1, // Top2
    -1, // Top3
    -1, // Top4
    -1, // TopJump1
    -1, // TopJump2
    -1  // TopJump3
};

#endif