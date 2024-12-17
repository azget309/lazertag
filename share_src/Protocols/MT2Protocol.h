#pragma once
#include <cstdint>
#include <stdint.h>

#define MT2_HIT_LEN 14
#define MT2_MSG_LEN (3*8)


enum MT2_FireSelector{
    SEMI_AUTO   = 0x00,
    BURST       = 0x01,
    FULL_AUTO   = 0x02
};

enum MT2_TeamColor{
    RED = 0x00,
    BLUE = 0x01,
    YELLOW = 0x02,
    GREAN = 0x03,
};

enum MT2_DamageID{
    D1  = 0x0,
    D2  = 0x1,
    D4  = 0x2,
    D5  = 0x3,
    D7  = 0x4,
    D10 = 0x5,
    D15 = 0x6,
    D17 = 0x7,
    D20 = 0x8,
    D25 = 0x9,
    D30 = 0xA,
    D35 = 0xB,
    D40 = 0xC,
    D50 = 0xD,
    D75 = 0xE,
    D100= 0xF
};

enum MT2_DataID {
//    _reserv_1               = 4,
//    _reserv_2               = 5,
//    _reserv_3               = 6,
    team_id                 = 7,
//    _reserv_4               = 8,
    ammo_in_box             = 9,
    health_in_box           = 10,
//    _reserv_5               = 11,
    hit_led_timeout         = 12,
    sound_set_id            = 13,
    overheat_limit_in_round = 14,
    reload_type             = 15,
//    _reserv_7               = 16,
    damage                  = 17,
    clip_size               = 18,
    clip_count              = 19,
    fire_type               = 20,
    burst_size              = 21,
    fire_temp               = 22,
    reload_time             = 23,
    ir_power                = 24,
    ir_range                = 25,
    tagger_settings         = 26,
    respawn_health          = 27,
//    _reserv_8               = 28,
    respawn_delay_10sec     = 29,
    armour                  = 30,
    game_settings0          = 31,
    game_settings1          = 32,
    stun_time               = 33,
    time_to_start           = 34,
    time_to_respawn         = 35,
    game_timeout            = 36,
    max_respawn_count       = 37,
//    _reserv_9               = 38,
};




enum MT2_PackageType{
    HIT = 0x7f,
    ADD_HEALTH      = 0x80,
    ADD_AMMO        = 0x81,
                  
    COMMAND         = 0x83,
                  
    APPLAY_DATA     = 0x85,     /* применить указанные данные */
    REQUEST_DATA    = 0x86,     /* запрос на указанный тип данных */
    RESPONSE_DATA   = 0x87,
                  
                  
    CLIPS_PICKUP    = 0x8A,
    HEALTH_PICKUP   = 0x8B,
    FLAG_PICKUP     = 0x8C,
/* ============= MY ==============*/
    GUN_CONNECT_REQ = 0x90,
    GUN_CONNECT_RES = 0x91,
    DISCONNECT_GUN  = 0x92,
};

enum MT2_CommandType{
    ADMIN_KILL          =   0x00,
    PAUSE_UNPAUSE       =   0x01,
    START_GAME          =   0x02,
    RESTORE_DEFAULTS    =   0x03,
    RESPAWN             =   0x04,
    NEW_GAME_IMMEDIATE  =   0x05,
    FULL_AMMO           =   0x06,
    END_GAME            =   0x07,
    RESET_CLOCK         =   0x08,

    INITIALIZE_PLAYER   =   0x0A,
    EXPLODE_PLAYER      =   0x0B,
    NEW_GAME_READY      =   0x0C,
    FULL_HEALTH         =   0x0D,

    FULL_ARMOR          =   0x0F,




    CLEAR_SCORES        =   0x14,
    TEST_SENSORS        =   0x15,
    STUN_PLAYER         =   0x16,
    DISARM_PLAYER       =   0x17,    
};

enum MT2_DataType{
    FULL_CONFIG_DATA        = 0x01,

    //SCORE_DATA_PART1    = 0x03,
    //SCORE_DATA_PART2    = 0x04,
    //SCORE_DATA_PART3    = 0x05,

    TIME_DATA           = 0x06,
    HITS_DATA           = 0x10,
};



#pragma pack(push, 1)
struct MT2_HitBlob{
/*01*/  uint8_t id;
/*02*/  uint8_t team_id:2;
        uint8_t damage_id:4;
        uint8_t __stuff:2;
};


struct MT2_MessageBlob{
/*01*/  uint8_t msg_type;
/*02*/  union{
            uint8_t data;
            uint8_t health_cnt;
            uint8_t ammo_cnt;
            uint8_t command_id;
            uint8_t data_type;
            uint8_t clips_box_id;
            uint8_t health_box_id;
            uint8_t flag_id;

            uint8_t device_id;
        };
/*03*/  uint8_t __stuff_0xE8;
};


struct MT2_Data{
/*04*/  uint8_t src_id;
/*05*/  uint8_t dst_id;
};


struct MT2_ConfigureBlob{
/*06*/  uint8_t _reserv_3;
/*07*/  uint8_t team_id;
/*08*/  uint8_t _reserv_4;
/*09*/  uint8_t ammo_in_box;
/*10*/  uint8_t health_in_box;
/*11*/  uint8_t _reserv_5;
/*12*/  uint8_t hit_led_timeout;  // sec
/*13*/  uint8_t sound_set_id;
/*14*/  uint8_t overheat_limit_in_round;
/*15*/  uint8_t reload_type; // RESERVED в прошлом
/*16*/  uint8_t _reserv_7;
/*17*/  uint8_t damage;
/*18*/  uint8_t clip_size; // ammo_in_mag
/*19*/  uint8_t clip_count; // mag_count
/*20*/  uint8_t fire_type;
/*21*/  uint8_t burst_size;
/*22*/  uint8_t fire_temp;
/*23*/  uint8_t reload_time;  // sec
/*24*/  uint8_t ir_power;
/*25*/  uint8_t ir_range;
// ============ tagger settings ==========
/*26*/  uint8_t muzzle_flash:1;
        uint8_t overheat_feature_on:1;
        uint8_t _reserv_tgconf:6;

/*27*/  uint8_t respawn_health;
/*28*/  uint8_t _reserv_8;
/*29*/  uint8_t respawn_delay_10sec;  // 10 sec
/*30*/  uint8_t armour;

// game settings
/*31*/  uint8_t _reserv_gcf_0:1;
        uint8_t hit_led_enable:1;
        uint8_t friendly_fire:1;
        uint8_t unlimited_clips:1;
        uint8_t zombie_mode:1;
        uint8_t medics_enable:1;
        uint8_t game_box_respawn_reset:1;
        uint8_t game_box_are_not_used_up:1;
/*32*/  uint8_t _reserv_gcf_1:1;
        uint8_t _reserv_gcf_2:1;
        uint8_t capture_the_flag_display:1;
        uint8_t respawn_enable:1;
        uint8_t tagger_nick_enable:1;
        uint8_t old_ir_level_field:1;
        uint8_t full_ammo_on_respawn:1;
        uint8_t enable_game_mode:1;

/*33*/  uint8_t stun_time;
/*34*/  uint8_t time_to_start;
/*35*/  uint8_t time_to_respawn;
/*36*/  uint8_t game_timeout;
/*37*/  uint8_t max_respawn_count;
/*38*/  uint8_t _reserv_9;
};


struct MT2_SystemTimeBlob{
/*4-5*/ uint32_t time:24;
};


struct MT2_OneHitData{
    uint32_t hit_time:17;
    uint32_t gun_id:7;
    uint32_t damage:4;
    uint32_t team_id:2;
    uint32_t is_dead:1;
};


struct MT2_OneChangeParamData{
    MT2_DataID data_id;
    uint8_t new_data;
};


struct MT2_PackageBlob{
    union{
        MT2_MessageBlob msg;
        MT2_HitBlob     hit;
    };
    union{
        MT2_ConfigureBlob   conf;
        MT2_SystemTimeBlob  time;
        MT2_DeviceInfoBlob  dev_info;
        
        uint8_t*            data;
    };
};

#pragma pack(pop)

const bool CheckCRC(void* blob, uint16_t size);                                                                             

const uint8_t   MT2_DecodeDamage(uint8_t damage_id);                                                                          
const uint16_t  MT2_DecodeHealth(uint8_t byte);                                                                              
const uint16_t  MT2_DecodeGunRPM(uint8_t byte);                                                                              
const uint16_t  MT2_DecodeStunTime(uint8_t byte);     

/*

class MT2_Hit{
public:
    MT2_Hit(void* blob):hit_blob((MT2_HitBlob*)blob){};

    const uint8_t getId(){return hit_blob->id;}
    const uint8_t getDamage(){return _MT2_DAMAGE[hit_blob->damage_id];}
    const MT2_TeamColor getTeamColor(){return (MT2_TeamColor)hit_blob->team_id;}
    
    void setId(uint8_t id){hit_blob->id = id;}
    void setDamage(MT2_DamageID damage){hit_blob->damage_id = damage;}
    void setTeam(MT2_TeamColor team){hit_blob->team_id = team;}
private:
    static const uint8_t _MT2_DAMAGE[16]; 
    MT2_HitBlob* hit_blob;
};


class MT2_Message{
public:
    MT2_Message(void* blob):
        msg_blob((MT2_MessageBlob*)blob){}
private:
    MT2_MessageBlob* msg_blob;
};


class MT2_Configure{
public:
    MT2_Configure(void* blob):
        conf_blob((MT2_ConfigureBlob*)((uint8_t*)blob+sizeof(MT2_MessageBlob))){}
    const uint16_t getHeilth();
    const uint16_t getGunRPM(){return (250+conf_blob->fire_temp*50);}
    const uint16_t getStunTimeMS();
    const MT2_ConfigureBlob& getConfigure(){return *conf_blob; };
private:
    MT2_ConfigureBlob* conf_blob;
};


class MT2_Package{
public:
    MT2_Package(void* blob, uint16_t size): blob(blob), size(size) {}

    const bool CheckCRC();
    const MT2_PackageType getType();
    template <class T> T To () { return T(blob); }
private:
    uint16_t size;
    void* blob;
};

//*/
