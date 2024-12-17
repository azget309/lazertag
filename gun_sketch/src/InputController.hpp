#pragma once

#include <EncButton.h>
#include "input_settings.hpp"

class InputController{
public:
    void Update();
private:
    Button fire_btn = Button(FIRE_BUTTON_ID);
    Button cocked_btn = Button(BOLT_BUTTON_ID);
    Button reload_btn = Button(MAGAZINE_BUTTON_ID);
};
