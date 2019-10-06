#ifndef DONKEY_KONG_JR_SCORE
#define DONKEY_KONG_JR_SCORE

#include <Gamebuino-Meta.h>

class Score
{
    private:

        uint16_t target;
        uint16_t value;
        uint16_t highA;
        uint16_t highB;

    public:

        uint16_t getValue();
        uint16_t getHighA();
        uint16_t getHighB();

        void inc(uint8_t amount);
        bool isIncreasing();
        void reset();

        Score();

        void update();
        void load();
        void saveA();
        void saveB();
};

#endif