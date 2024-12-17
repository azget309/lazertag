#pragma once
#include "MyStructs/MyStructs.hpp"
#include "Events.h"

namespace fpb{

class Process{
public:
    virtual void ProcessEvent(const Event& event) = 0;
private:
};


class RProcess : public Process{
public:
    virtual void Update() = 0;
private:
};


template<int max_event_count=20>
class Broker : public RProcess{
public:
    Broker(){
        for(int i=0; i<EventType::__SIZE__; i++){
            listeners[i] = nullptr;
        }
    }

    void SetListener(EventType tp, Process* lstnr){
        listeners[tp] = lstnr;
    }
    
    void ProcessEvent(const Event& ev) override{
        psh_lst->push_back(ev);
    }

    void Update() override{
        while(!lsn_lst->empty()){
            Event ev = lsn_lst->pop_front();
            if(listeners[ev.type]!=nullptr){
                listeners[ev.type]->ProcessEvent(ev);
            }
        }
        auto tmp = lsn_lst;
        lsn_lst = psh_lst;
        psh_lst = tmp;
    }
private:
    Process* listeners[EventType::__SIZE__] = {};
    Queue<Event, max_event_count> list1;
    Queue<Event, max_event_count> list2;
    Queue<Event, max_event_count> *psh_lst = &list1, *lsn_lst = &list2;
};
}

