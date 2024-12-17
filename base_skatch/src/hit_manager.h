#include <ArduinoJson.h>

#include "MyStructs/MyStructs.hpp"
#include "Protocols/RFHit.hpp"

struct SaveHit{
    RF_Hit hit;
    uint8_t pray;
};

Queue<SaveHit, 500> hit_queue;
String HitsToJson(){
    String out = "{\"hits\":[";
    while(!hit_queue.empty()){
        SaveHit hit = hit_queue.pop_front();
        out += String(hit.pray)+":[]";
    }
    out += "]}";
    return out;
}
