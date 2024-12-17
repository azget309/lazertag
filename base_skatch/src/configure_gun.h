#pragma once
#include <stdint.h>
#include <ArduinoJson.h>
#include "Protocols/MT2Protocol.h"


bool JsonToConfig(String json, MT2_Configure& blob);


