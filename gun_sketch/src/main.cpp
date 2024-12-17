#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include <lgt_LowPower.h>

#include <util/delay.h>
#include <Gyver433.h>

#include "SimpleBroker.hpp"
#include "Time.hpp"


#define __PROTEUS__
#include "IR/IRSender.hpp"
#include "IR/IRReceiver.hpp"

#include "Protocols/MT2Protocol.h"

#include "gun.hpp"

static EventTimer<10> millis_event_timer;

static IRSender<17> sender;
static IRReceiver<2, 42> receiver;

void broker_setup(){
    fpb::Event ev = {.gun={.type=fpb::gun_event, .state=0}};
    PushEvent(ev);
    for(int i=0; i<4; i++){
        millis_event_timer.RemindEvent(
                fpb::Event{.gun={.type=fpb::gun_event, .state=0}},
                millis()+i*1000+400);
    }
}


void ir_isr(){ receiver.tickISR(); }


void setup(){
    Serial.begin(9600);
    Serial.println("HI, world");
    attachInterrupt(0, ir_isr, CHANGE);
    broker_setup();
}


void loop(){
    auto cur_time = millis();
    millis_event_timer.Update(cur_time);
    while(UpdateEvents());
    time nt = millis_event_timer.TimeToNextEvent(cur_time);
    if(receiver.gotData()){
        if(CheckCRC(receiver.buffer, receiver.bit_size)){
            MT2_PackageBlob* proto = (MT2_PackageBlob*)receiver.buffer;
            switch(proto.){
                case MT2_PackageType::HIT: {
                    Serial.println("hit:");
                    auto hit = proto.To<MT2_Hit>();
                    Serial.print("id: ");   Serial.println(hit.getId(), BIN);
                    Serial.print("team: "); Serial.println(hit.getTeamColor(), BIN);
                    Serial.print("damag: ");Serial.println(hit.getDamage(), BIN);
                    break;}
                case MT2_PackageType::COMMAND:{
                    Serial.println("cmd:");
                    MT2_Message msg = proto.To<MT2_Message>();
                    Serial.println(msg.msg_blob->command_id);
                    break;}
                case MT2_PackageType::RESPONSE_DATA:{
                    Serial.println("data:");
                    MT2_Message dp = proto.To<MT2_Message>();
                    switch(dp.msg_blob->data_type){
                        case TIME_DATA:{
                            Serial.println("time:");
                            break;}
                        case FULL_CONFIG_DATA:{
                            MT2_Configure dp = proto.To<MT2_Configure>();
                            Serial.println("conf:");
                            Serial.println(dp.getConfigure()._CRC);
                            break;}
                        default:
                            break;
                    }
                    break;}
                default:
                    break;
            }
        }
    }

    Serial.flush();
    //LowPower.powerDown(SLEEP_128MS, ADC_OFF, BOD_OFF);
    //LowPower.idle(SLEEP_64MS, ADC_ON, TIMER3_OFF, TIMER2_ON, TIMER1_OFF, TIMER0_ON, SPI_OFF, USART0_ON, TWI_OFF, PCIC_OFF, FLASHCTL_OFF);
    //set_millis(millis()+128);
}



void ButtonEventUpdate(const fpb::Event& event){

}

void IRSendEventUpdate(const fpb::Event& event){

}

void RFReceiveEventUpdate(const fpb::Event& event){

}

void GunEventUpdate(const fpb::Event& event){
    MT2_ConfigureBlob blob;
    blob._CRC = 0x12;
    Serial.println("Send:");
    sender.write(&blob, MT2_CLONE_DATA_LEN);
}

void AudioEventUpdate(const fpb::Event& event){

}

