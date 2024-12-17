#define __PROTEUS__
#include "IRSender.hpp"
#include "Timer.hpp"


IRSender<9> sender;


void setup() {
}

void loop() {
    TIMER_BEGIN(360)
        //digitalWrite(13, 1);
        //digitalWrite(9, 1);
        _delay_ms(180);
        //digitalWrite(13, 0);
        //digitalWrite(9, 0);
        uint16_t bt = 0b0010101110001100;
        sender.write((uint8_t*)&bt, 16);
    TIMER_END
}
