#pragma once
#include <stdint.h>
#include "Protocols/MT2Protocol.h"

#define AUTO_BURST 255
#define NO_AUTOLOADED 0
#define STOP_FIRE -1

//*
enum ReloadType{
    MAGAZINE, ON_BUTTON, PUMP,
};

class GunMecanic{
public:
    GunMecanic(MT2_Configure& config):
        gun_config(config),
        fire_temp(config.fire_temp),
        burst_size(config.burst_size){};

    void ChangeBurstSize(int16_t burst_size);
    virtual bool TriggerDown();
    virtual void TriggerUp();
    virtual void BoltBack();
    virtual void BoltForward();
    virtual void MagOut();
    virtual void MagIn();

    bool IsShooting();
    virtual void Update(uint32_t ms);
private:
    MT2_Configure&   gun_config;
    uint32_t    fire_timer=0;       // время возврата затвора
    uint16_t    fire_temp;          // темп огня
    int16_t     burst_size;         // тип огня
    int16_t     trigger_down=0;     // кол.во оставшихся выстрелов в отсечке
    uint8_t     bullet_count=0;     // оставшиеся пули
    bool        bullet_in_hold=0;   // есть ли патрон в патроннике
    bool        is_shooting=false;  // произашел ли выстрел
};
//*/

/*
class GunHandler{
public:
    GunHandler(Gun* g) : gun(g){};
    virtual void Update() = 0;
    virtual void Reset() = 0;
protected:
    virtual void FireButtonReaction(bool down_or_up) = 0;
    virtual void BoltButtonReaction(bool down_or_up) = 0;
    virtual void MagazineButtonReaction(bool down_or_up) = 0;
    Gun* gun;
};

class SimpleMagazineGunHandler : public GunHandler{
public:
    SimpleMagazineGunHandler(
            Gun* g,
            uint8_t magazine_count,
            uint8_t ammo_in_magazine
        ) : GunHandler(g),
            start_magazine_count(magazine_count),
            ammo_in_magazine(ammo_in_magazine)
        {Reset();}
    void Update() override;
protected:
    void Reset() override;
    void FireButtonReaction(bool down_or_up) override;
    void BoltButtonReaction(bool down_or_up) override;
    void MagazineButtonReaction(bool down_or_up) override;

    uint8_t start_magazine_count, magazine_count;
    uint8_t ammo_in_magazine;
};


class CasualMagazineGunHandler : public SimpleMagazineGunHandler{
public:
    CasualMagazineGunHandler(
            Gun* gun,
            uint8_t magazine_count,
            uint8_t ammo_in_magazine,
            uint16_t reload_timeout
            ) :
        SimpleMagazineGunHandler(gun,magazine_count,ammo_in_magazine),
        reload_timeout(reload_timeout)
        {Reset();};
    void Update() override;
protected:
    void Reset() override;
    void MagazineButtonReaction(bool down_or_up) override;
    uint16_t reload_timeout;
    uint32_t reload_timer=0;
};



class PumpGunHandler : public CasualMagazineGunHandler{
public:
    PumpGunHandler(
            Gun* gun,
            uint8_t magazine_count,
            uint8_t ammo_in_magazine,
            uint16_t reload_timeout
            ) :
        CasualMagazineGunHandler(gun,magazine_count,
                ammo_in_magazine,reload_timeout)
        {Reset();}
    void Update() override;
private:
    void MagazineButtonReaction(bool down_or_up) override;
private:
    uint8_t ammo_in_last_mag=0;
};

//*/




