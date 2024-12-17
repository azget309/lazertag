#include "SimpleBroker.hpp"

#include "MyStructs/MyStructs.hpp"

using namespace fpb;

#define EV_Q_SZ 15
static Queue<Event, EV_Q_SZ> ev_qe1, ev_qe2;
static auto in_qe=&ev_qe1, ou_qe=&ev_qe2;

void PushEvent(const Event& ev){
    in_qe->push_back(ev);
}

uint8_t UpdateEvents(){
    while(!ou_qe->empty()){
        auto ev = ou_qe->pop_front();
        switch(ev.type){
            #undef EV_XX
            #define EV_XX(sn, on) case on##_event: sn##Update(ev); break;
            #include "EventList.tlb"
            default:break;
        }
    }
    
    auto tmp = in_qe;
    in_qe = ou_qe;
    ou_qe = tmp;
    return ou_qe->size();
}
