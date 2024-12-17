#pragma once
#include <stdint.h>
#include <Arduino.h>

#include "IRGeneral.hpp"

#define IN_TOLERANCE(x,y) ((x)>=(y-TOLERANCE) && (x)<(y+TOLERANCE))

template<uint8_t PIN, uint8_t SIZE>
class IRReceiver{
public:
    IRReceiver() = default;
//#pragma GCC push_options
//#pragma GCC optimize ("O3")
    void tickISR(){
        if(_is_faling){
            _is_faling = false;
            _last_time = micros();
            return;
        }else{
            _is_faling = true;
            uint16_t dt = micros() - _last_time;

            if (IN_TOLERANCE(dt, MARK0_TIME)){  // если пришел 0
                _filling_byte >>= 1;
                //_filling_byte &= ~1<<7;
                _bit_num ++;
            }else if (IN_TOLERANCE(dt, MARK1_TIME)){  // если пришла 1
                _filling_byte >>= 1;
                _filling_byte |= 1<<7;
                _bit_num ++;
            }else{  // начало пакета или ошибка(вспышка какаита)
                _bit_num = 0;
            }

            if(_bit_num%8==0 && _bit_num>0){
                buffer[_bit_num/8-1] = _filling_byte;
                //Serial.println("ok")
            }
        }
    }
//#pragma GCC pop_options

    uint16_t gotData(){
        //cli();
        // если время ожидания с приема последнего 
        // бита больше чем время приема единицы
        if(_last_time > POST_PACKAGE_TIME){
            bit_size = _bit_num;
            _bit_num = 0;
            // дописываем в буфер, что осталось в заполняемом байте
            if(bit_size%8){
                buffer[bit_size/8] = _filling_byte>>(8-bit_size%8);
            }
            return bit_size;
        }
        //sei();
        return 0;
    }

    uint8_t buffer[SIZE];   // выходной буфер
    uint16_t bit_size=0;
private:
    volatile uint32_t   _last_time=0;       // предыдущий micros
    volatile uint16_t   _bit_num=0;         // количество принятых битов
    volatile uint8_t    _filling_byte=0;    // байт в который будут битики записываться
    volatile bool       _is_faling = true;  // фалаг фронта(битики в нижнем)
};

