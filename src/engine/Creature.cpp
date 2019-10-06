// -------------------------------------------------------------------------
//                               Donkey Kong Jr
//           a Nintendo's Game & Watch adaptation on Gamebuino META
// -------------------------------------------------------------------------
//                                © 2019 Steph
//                        https://gamebuino.com/@steph
// -------------------------------------------------------------------------
//                           Enemy Management Module
// -------------------------------------------------------------------------

// loads the header file of the class
#include "Creature.h"

// loads the sound effects manager
#include "../sounds/Buzzer.h"

// clears all enemies
void Creature::reset() {
    uint8_t i;
    for (i=0; i<crocsSize; i++) crocs[i] = {false, false};
    for (i=0; i<birdsSize; i++) birds[i] = {false, false};
}

// any enemies in the vicinity of this position are deleted
// to allow the player to restart from his starting position
void Creature::clear() {
    birds[0] = {false, false};
    birds[1] = {false, false};
    crocs[crocsSize - 1] = {false, false};
    crocs[crocsSize - 2] = {false, false};
}

// attempts to destroy a bird at the specified position
// and awards a reward in case of success
void Creature::tryToKillBirdAt(uint8_t i) {
    if (birds[i].exists) {
        birds[i] = {false, false};
        score->inc(rewardWhenKilledBird);
    }
}

// attempts to destroy a snapjaw at the specified position
// and awards a reward in case of success
void Creature::tryToKillCrocAt(uint8_t i) {
    if (crocs[i].exists) {
        crocs[i] = {false, false};
        if (i == 2) {
            score->inc(rewardWhenKilledTopCroc);
        } else if (i == 9) {
            score->inc(rewardWhenKilledBottomCroc);
        }
    }
}

// determines the presence of a bird at the specified position
// and penalizes the player if he tries to cross this position
bool Creature::isThereAnyBirdNearJunior(int8_t i) {
    if (i != -1 && birds[i].exists) {
        hasBittenJunior = true;
        bittenTime = gb.frameCount;
        Buzzer::repeat(Sound::Lost, 5, 8);
        return true;
    }
    return false;
}

// determines the presence of a snapjaw at the specified position
// and penalizes the player if he tries to cross this position
bool Creature::isThereAnyCrocNearJunior(int8_t i) {
    if (i != -1 && crocs[i].exists) {
        hasBittenJunior = true;
        bittenTime = gb.frameCount;
        Buzzer::repeat(Sound::Lost, 5, 8);
        return true;
    }
    return false;
}

// controls the progress of enemies
void Creature::update(bool isPlaying) {
    this->isPlaying = isPlaying;
    paceMaker       = !paceMaker;
    bip             = false;
    hasBittenJunior = false;

    // first, controls the progress of birds
    updateBirds();
    // then, if Junior wasn't bitten, controls the progress of snapjaws
    if (!hasBittenJunior) updateCrocs();

    // emits a sound effect at each step of the enemies' progress
    if (isPlaying && bip) Buzzer::play(Sound::Tick);
}

// controls the progress of birds
void Creature::updateBirds() {
    // moves birds already present
    moveBirdForward();
    // and examines the opportunity to create a new one
    spawnBird();
}

// controls the progress of snapjaws
void Creature::updateCrocs() {
    // moves snapjaws already present
    moveCrocForward();
    // and examines the opportunity to create a new one
    spawnCroc();
}

// examines the opportunity to make a new bird appear
void Creature::spawnBird() {
    // counts the number of birds already present
    uint8_t spawned = 0;
    for (uint8_t i=0; i<birdsSize; i++) {
        if (birds[i].exists) spawned++;
    }

    // determines the maximum number of birds that can
    // be present simultaneously on the game scene
    uint8_t maxBirds = 1 + (score->getValue() / 200);
    maxBirds = maxBirds < 5 ? maxBirds : 4;

    // if the limit is not yet reached, a new bird may appear
    if (spawned < maxBirds && random(4) == 0) {
        if (!birds[0].exists) {
            if (!birds[1].exists || birds[1].now != paceMaker) {
                birds[0] = {true, paceMaker};
                bip = true;
            }
        }
    }
}

// examines the opportunity to make a new snapjaw appear
void Creature::spawnCroc() {
    // counts the number of snapjaws already present
    uint8_t spawned = 0;
    for (uint8_t i=0; i<crocsSize; i++) {
        if (crocs[i].exists) spawned++;
    }

    // determines the maximum number of snapjaws that can
    // be present simultaneously on the game scene
    uint8_t maxCrocs = 2 + (score->getValue() / 200);
    maxCrocs = maxCrocs < 8 ? maxCrocs : 7;

    // if the limit is not yet reached, a new snapjaw may appear
    if (spawned < maxCrocs && random(6) == 0) {
        if (!crocs[0].exists) {
            if (!crocs[1].exists || crocs[1].now != paceMaker) {
                crocs[0] = {true, paceMaker};
                bip = true;
            }
        }
    }
}

// moves the birds forward and examines Junior's presence on their way
void Creature::moveBirdForward(){
    uint8_t i;
    Beat    b;

    if (isPlaying) {
        for (i=0; i<birdsSize; i++) {

            b = birds[i];

            // if Junior is spotted on his way, he gets bitten 
            if (b.exists && b.now == paceMaker && bittenByBird[i] == junior->spriteIndex) {
                hasBittenJunior = true;
                bittenTime = gb.frameCount;
                Buzzer::repeat(Sound::Lost, 5, 8);
                return;
            }
            
        }
    }

    // finally, we move all the birds a notch ahead

    b = birds[birdsSize - 1];
    if (b.exists && b.now == paceMaker) birds[birdsSize - 1] = {false, paceMaker};

    for (i = birdsSize - 1; i > 0; i--) {
        b = birds[i - 1];
        if (b.exists && b.now == paceMaker) {
            birds[i] = b;
            birds[i - 1] = {false, paceMaker};
            bip = true;
        }
    }
}

// moves the snapjaws forward and examines Junior's presence on their way
void Creature::moveCrocForward(){
    uint8_t scoring = 0;
    uint8_t i;
    Beat    b;

    if (isPlaying) {
        for (i=0; i<crocsSize; i++) {

            b = crocs[i];

            // if Junior manages to avoid the enemy's advance by jumping, he is rewarded
            if (b.exists && b.now == paceMaker) {
                     if (i == 10 && junior->spriteIndex == BottomJump2 && junior->spriteLastIndex == Bottom2) scoring++;
                else if (i ==  7 && junior->spriteIndex == BottomJump5 && junior->spriteLastIndex == Bottom5) scoring++;
                else if (i ==  2 && junior->spriteIndex == TopJump2    && junior->spriteLastIndex == Top3)    scoring++;
                else if (i ==  1 && junior->spriteIndex == TopJump3    && junior->spriteLastIndex == Top4)    scoring++;
            }

            // if Junior is spotted on his way, he gets bitten 
            if (b.exists && b.now == paceMaker && bittenByCroc[i] == junior->spriteIndex) {
                hasBittenJunior = true;
                bittenTime = gb.frameCount;
                Buzzer::repeat(Sound::Lost, 5, 8);
                return;
            }
            
        }

        // if a reward has been collected, it is added to the score
        if (scoring) {
            score->inc(scoring);
        }
    }

    // finally, we move all the snapjaws a notch ahead

    b = crocs[crocsSize - 1];
    if (b.exists && b.now == paceMaker) crocs[crocsSize - 1] = {false, paceMaker};

    for (i = crocsSize - 1; i > 0; i--) {
        b = crocs[i - 1];
        if (b.exists && b.now == paceMaker) {
            crocs[i] = b;
            crocs[i - 1] = {false, paceMaker};
            // disable sound effect when scoring because
            // the increase in score already produces a series of sound effects
            bip = !scoring;
        }
    }
}

// renders the sprites of all the enemies present on the current screen slice
void Creature::drawSpritesOnSlice(Renderer * renderer, uint8_t sliceY, uint16_t * buffer) {
    uint8_t i;
    
    for (i=0; i<crocsSize; i++) {
        if (crocs[i].exists) renderer->drawSpriteOnSlice(crocSprite[i], sliceY, buffer);
    }

    for (i=0; i<birdsSize; i++) {
        if (birds[i].exists) renderer->drawSpriteOnSlice(birdSprite[i], sliceY, buffer);
    }
}