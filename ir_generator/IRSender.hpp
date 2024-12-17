#pragma once

#include <stdint.h>
#include <util/delay.h>
#include <Arduino.h>
#include "IRGeneral.hpp"

#define __IR_T (1000/IR_FRQ)

template <uint8_t PIN>
class IRSender{
public:
    IRSender(){
        pinMode(PIN, OUTPUT);
        #ifdef __PROTEUS__
        fastWrite(PIN, 1);
        #endif
    }
    template<class T>
    void write(const T &data){
        uint8_t *ptr = (uint8_t*) &data;
        write(ptr, sizeof(T)*8);
    }

    void write(const uint8_t *buffer, const uint16_t len){
        Pulse(PRE_MARK_TIME);
        _delay_us(PRE_SPACE_TIME);

        for(uint16_t i=0; i<len/8; i++){
            SendByte(buffer[i], 8);
        }
        SendByte(buffer[len/8], len%8);
    }

private:

    inline void SendByte(uint8_t byte, uint8_t len){
        for(uint8_t b=0; b<len; b++){
            if(byte & (1<<b)){
                Pulse(MARK1_TIME);
            }else{
                Pulse(MARK0_TIME);
            }
            _delay_us(SPACE_TIME);
        }
    }

    void Pulse(uint16_t dur){
        uint32_t T = micros()+dur;
        bool pipka = 1;
#ifndef __PROTEUS__
        while(micros() <= T){
            fastWrite(PIN, pipka);
            pipka = !pipka;
            _delay_us(__IR_T/2);
        }
#else
        fastWrite(PIN, 0);
        while(micros()<=T){
            _delay_us(__IR_T/2);
        }
#endif
        fastWrite(PIN, 1);

    }

    void fastWrite(const uint8_t pin, bool val) {
    #if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) || defined(__LGT8F328P__)
        if (pin < 8) bitWrite(PORTD, pin, val);
        else if (pin < 14) bitWrite(PORTB, (pin - 8), val);
        else if (pin < 20) bitWrite(PORTC, (pin - 14), val);
    #elif defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny13__)
        bitWrite(PORTB, pin, val);
    #else
        digitalWrite(pin, val);
    #endif
    }

};

/*
#pragma once

#include <Arduino.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "IRGeneral.hpp"


class IRSender{
public:
    IRSender(volatile uint8_t* ddrx, volatile uint8_t* portx, uint8_t pin);
    void Send(uint32_t data, uint8_t len);
    void Update();
private:
    volatile uint8_t* _port;
    uint8_t _pin;

    uint32_t    _timer;
    uint32_t    _data_buf;
    int8_t      _send_bit;
private:
    void Pulse(uint16_t dur);
};

*/

