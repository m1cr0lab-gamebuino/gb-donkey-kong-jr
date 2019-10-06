#include "Score.h"
#include "../sounds/Buzzer.h"

Score::Score() {
    reset();
}

uint16_t Score::getValue() {
    return value;
}

uint16_t Score::getHighA() {
    return highA;
}

uint16_t Score::getHighB() {
    return highB;
}

void Score::reset() {
    value  = 0;
    target = 0;
}

void Score::inc(uint8_t amount) {
    if (amount == 1) {
        target = ++value;
        Buzzer::play(Sound::Score);
    } else {
        target += amount;
    }
}

bool Score::isIncreasing() {
    return value < target;
}

void Score::update() {
    if (value < target && gb.frameCount % 2 == 0) {
        value++;
        Buzzer::play(Sound::Score);
    }
}

void Score::load() {
    value = gb.save.get(0);
    highA = gb.save.get(1);
    highB = gb.save.get(2);
}

void Score::saveA() {
    if (value > highA) highA = value;
    gb.save.set(0, value);
    gb.save.set(1, highA);
}

void Score::saveB() {
    if (value > highB) highB = value;
    gb.save.set(0, value);
    gb.save.set(2, highB);
}