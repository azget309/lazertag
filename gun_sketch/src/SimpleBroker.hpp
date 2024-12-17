#pragma once
#include "Events.h"

#undef EV_XX
#define EV_XX(sn, on) extern void sn##Update(const fpb::Event& event);
#include "EventList.tlb"

uint8_t UpdateEvents(); // возвращает количество новых эвентов
void PushEvent(const fpb::Event& event);

