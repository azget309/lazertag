#pragma once
#define TIMER_BEGIN(x)                      \
static uint32_t __ebuchiy_timer=millis();   \
uint32_t __ebuchiy_ctimer=millis();         \
if(__ebuchiy_ctimer-__ebuchiy_timer > x){   \
    __ebuchiy_timer=__ebuchiy_ctimer;
#define TIMER_END }