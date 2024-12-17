#include <Arduino.h>

#include <EncButton.h>

#include <util/delay.h>
#define G433_SPEED 2000
#define G433_FAST
#include <Gyver433.h>

#define __PROTEUS__

#include "Protocols/MT2Protocol.h"
#include "IR/IRReceiver.hpp"
#include "core/Button.h"


Gyver433_TX<4, G433_XOR> rf_tx;
IRReceiver<2, 40> ir_rx;
ButtonT<18> controll_button;


void setup(){
    Serial.begin(9600);
    Serial.println(F("DET Started"));
}


void loop(){
    static char buf[9] = "HI_HA:  "; static uint8_t i = 0;
    buf[7] = (i%10)+'0';
    buf[6] = (i/10)+'0';
    rf_tx.write((uint8_t*)buf, sizeof(buf));
    i=(i+1)%100;
    delay(2000);
}
