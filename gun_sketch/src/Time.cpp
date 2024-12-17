#include "Time.hpp"

extern volatile unsigned long timer0_millis;
void set_millis(unsigned long new_millis){
  uint8_t oldSREG = SREG;
  cli();
  timer0_millis = new_millis;
  SREG = oldSREG;
}

uint32_t seconds24h(){
    return (millis()/1000)%(86400);
}
