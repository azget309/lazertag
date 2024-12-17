#pragma once
#include <stdint.h>
namespace fpb{

enum EventType{
    #undef EV_XX
    #define EV_XX(sn, on) on##_event,
    #include "EventList.tlb"
    __SIZE__
};

struct ButtonEvent{
    EventType type;
    uint8_t button_id;
    uint8_t is_down;
};

struct AudioEvent{
    EventType type;
    uint8_t sound_id;
};

struct GunEvent{
    EventType type;
    uint8_t state;
};

struct IRSendEvent{
    EventType type;
    uint8_t len;
    uint8_t buffer[4];
};

struct RFReceiveEvent{
    EventType type;
    uint8_t len;
    uint8_t data[4];
};


union Event{
    EventType type;
    #undef EV_XX
    #define EV_XX(sn, on) sn on;
    #include "EventList.tlb"
};
}
