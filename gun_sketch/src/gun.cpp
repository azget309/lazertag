#include "gun.hpp"

/*
void GunMecanic::Update(uint32_t ms){
    if(fire_timer==0){  // если таймер выстрела прошел
        if(trigger_down>0){  // и вся очередь не выстреллена
            if(bullet_in_hold){  // при этом патрон в патроннике
                is_shooting = true;
                bullet_in_hold = false;
                fire_timer = ms + fire_temp; // обновляем таймер выстреала
            }else{ trigger_down = 0;  // одноразовое срабатывание
            }
        }
    }else if(ms>=fire_timer){
        fire_timer = 0;
        if(trigger_down>0){
            trigger_down--;
        }
        if(burst_size>0){
            BoltBack();
            BoltForward();
        }
    }
}

bool GunMecanic::IsShooting(){
    auto res = is_shooting;
    is_shooting = false;
    return res;
}

void GunMecanic::ChangeBurstSize(int16_t bs){
    burst_size = bs;
}

bool GunMecanic::TriggerDown(){
    if(bullet_in_hold == 0 || burst_size==STOP_FIRE)
        return false;

    if(burst_size>0){
        trigger_down = burst_size;
    }else{
        trigger_down = 1;
    }
    return true;
}

void GunMecanic::TriggerUp(){
    trigger_down = 0;
}

void GunMecanic::BoltBack(){
    bullet_in_hold = 0;
}

void GunMecanic::BoltForward(){
    if(bullet_count>0){
        bullet_in_hold = 1;
        bullet_count--;
    }
}
//*/



/*
void SimpleMagazineGunHandler::Reset(){
    magazine_count = start_magazine_count;
    gun->MagOut();
    gun->MagIn(1);
    gun->BoltBack();
    gun->BoltForward();
    gun->MagIn(ammo_in_magazine);
}

void SimpleMagazineGunHandler::Update(){
    GunEvent ev = {};
    EventBus& event_bus = EventBus::GetInstance();

    for(auto itr : event_bus){
        bool bt_down = itr.type==BUTTON_DOWN? true:false;
        switch(itr.button.id){
        case FIRE_BUTTON_ID:
            FireButtonReaction(bt_down);
            break;
        case BOLT_BUTTON_ID:
            BoltButtonReaction(bt_down);
            break;
        case MAGAZINE_BUTTON_ID:
            MagazineButtonReaction(bt_down);
            break;
        }
    }

    if(gun->IsShooting()){
        ev.type = GUN_FIRE;
        event_bus.push_back((Event&)(ev));
    }
}


void SimpleMagazineGunHandler::FireButtonReaction(bool bt_is_down){
    if(bt_is_down){
        if(!gun->TriggerDown()){
            GunEvent ev = {};
            EventBus& event_bus = EventBus::GetInstance();
            ev.type = GUN_FREE_TRIG_DOWN;
            event_bus.push_back((Event&)(ev));
        }
    }else{
        gun->TriggerUp();
    }
}

void SimpleMagazineGunHandler::BoltButtonReaction(bool bt_is_down){
    if(bt_is_down){
        gun->BoltBack();
    }else{
        gun->BoltForward();
    }
}

void SimpleMagazineGunHandler::MagazineButtonReaction(bool bt_is_down){
    if(bt_is_down){
        gun->MagOut();
    }else{
        if(magazine_count>0){
            magazine_count--;
            gun->MagIn(ammo_in_magazine);
        }else{
            gun->MagIn(0);
        }
    }
}




void CasualMagazineGunHandler::Reset(){
    reload_timer = 0;
    this->SimpleMagazineGunHandler::Reset();
}

void CasualMagazineGunHandler::Update(){
    if(reload_timer!=0 && millis() > reload_timer){
        reload_timeout = 0;
        gun->MagIn(ammo_in_magazine);
    }
    this->SimpleMagazineGunHandler::Update();
}

void CasualMagazineGunHandler::MagazineButtonReaction(bool bt_is_down){
    if(!bt_is_down && reload_timer==0 && magazine_count>0){
        reload_timer = millis()+reload_timeout;
        gun->MagOut();
        magazine_count--;
    }
}




void PumpGunHandler::Update(){
    if(reload_timer!=0 && millis() > reload_timer){
        reload_timer = 0;
        gun->MagIn(ammo_in_last_mag);
        ammo_in_last_mag = 0;
    }

    this->SimpleMagazineGunHandler::Update();
}

void PumpGunHandler::MagazineButtonReaction(bool bt_is_down){
    if(bt_is_down && reload_timer==0 && magazine_count>0){
        reload_timer = millis() + reload_timeout;
        ammo_in_last_mag = gun->MagOut();
        if(ammo_in_last_mag < ammo_in_magazine){
            magazine_count --;
            ammo_in_last_mag ++;
        }
    }else{

    }
}



GunHandler* NewGunHandler(GunParam* param, Gun* gun){
    switch(param->supply_type){
        case CASUAL_MAGAZINE:
            return new CasualMagazineGunHandler(
                gun,
                param->mag_count,
                param->bullet_count,
                param->reload_time
                );
            break;
        case SIMPLE_MAGAZINE:
            return new SimpleMagazineGunHandler(
                gun,
                param->mag_count,
                param->bullet_count
                );
            break;
        case PUMP:
            return new PumpGunHandler(
                gun,
                param->mag_count,
                param->bullet_count,
                param->reload_time
                );
            break;
        default:
            return nullptr;
            break;
    }
}


*/





