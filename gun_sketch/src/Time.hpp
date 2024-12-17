#pragma once
#include <assert.h>
#include <Arduino.h>
#include <lgt_LowPower.h>

#include <MyStructs/MyStructs.hpp>

#include "SimpleBroker.hpp"


#define TIMER_BEGIN(x) {                    \
static uint32_t __ebuchiy_timer=millis();   \
uint32_t __ebuchiy_ctimer=millis();         \
if(__ebuchiy_ctimer-__ebuchiy_timer > x){   \
    __ebuchiy_timer=__ebuchiy_ctimer;
#define TIMER_END }}
void set_millis(unsigned long new_millis);

#define INF_TIME 0x3F3F3F3F

uint32_t seconds24h();

void Sleep(uint32_t ms);

typedef uint32_t time;

struct TimeEvent{
    fpb::Event event;
    time remind_time;
};

template <size_t max_size>
class EventTimer{
public:
    void RemindEvent(const fpb::Event& event, time remind_time){
        assert(!timetable.full());
        auto i = timetable.begin();
        // заполняем по порядку от свежайших к старейшим
        for(;i != timetable.end();i++){
            if(remind_time < (*i).remind_time){
                break;
            }
        }
        timetable.insert(i, {.event=event, .remind_time=remind_time});
    }

    void Update(time cur_time){  //return sleep_time
        auto i = timetable.begin();
        while(  i!=timetable.end()
                && cur_time>(*i).remind_time
        ){
            PushEvent((*i).event);
            i = timetable.erase(i);
        }
    }

    time TimeToNextEvent(time cur_time){
        auto i = timetable.begin();
        if(i!=timetable.end())
            if((*i).remind_time > cur_time)
                return (*i).remind_time - cur_time;
            else
                return cur_time - (*i).remind_time;
        else
            return INF_TIME; //любое время
    }
private:
    List<TimeEvent, max_size, uint8_t> timetable;
};
