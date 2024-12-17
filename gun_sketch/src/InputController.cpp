#include "InputController.hpp"
#include "Events.h"
#include "SimpleBroker.hpp"
#include "input_settings.hpp"

void InputController::Update(){
    fire_btn.tick();
    cocked_btn.tick();
    reload_btn.tick();

    if(fire_btn.press()){
        PushEvent(fpb::Event{.button={
            .type=fpb::button_event,
            .button_id=FIRE_BUTTON_ID,
            .is_down=1
            }});
    }else if(fire_btn.release()){
        PushEvent(fpb::Event{.button={
            .type=fpb::button_event,
            .button_id=FIRE_BUTTON_ID,
            .is_down=0
            }});
    }

    if(cocked_btn.press()){
        PushEvent(fpb::Event{.button={
            .type=fpb::button_event,
            .button_id=BOLT_BUTTON_ID,
            .is_down=1
            }});
    }else if(cocked_btn.release()){
        PushEvent(fpb::Event{.button={
            .type=fpb::button_event,
            .button_id=BOLT_BUTTON_ID,
            .is_down=0
            }});
    }

    if(reload_btn.press()){
        PushEvent(fpb::Event{.button={
            .type=fpb::button_event,
            .button_id=MAGAZINE_BUTTON_ID,
            .is_down=1
            }});
    }else if(reload_btn.release()){
        PushEvent(fpb::Event{.button={
            .type=fpb::button_event,
            .button_id=MAGAZINE_BUTTON_ID,
            .is_down=0
            }});
    }
}
