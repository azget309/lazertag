import cstruct

class GunConfig(cstruct.MemCStruct):
    __def__ = """
    struct ConfigurePackageBlob{
        unsigned char _reserv_1;
        unsigned char _reserv_2;
        unsigned char _reserv_3;
        unsigned char team_id;
        unsigned char _reserv_4;
        unsigned char ammo_in_box;
        unsigned char health_in_pack;
        unsigned char _reserv_5;
        unsigned char hit_led_timeout;
        unsigned char sound_set_id;
        unsigned char overheat_limit_in_round;
        unsigned char reload_type;
        unsigned char _reserv_7;
        unsigned char damage;
        unsigned char clip_size;
        unsigned char clip_count;
        unsigned char fire_type;
        unsigned char burst_size;
        unsigned char fire_temp;
        unsigned char reload_time;
        unsigned char ir_power;
        unsigned char ir_range;
        unsigned char tager_settings;
        unsigned char respawn_health;
        unsigned char _reserv_8;
        unsigned char respawn_delay;
        unsigned char armour;
        unsigned char game_settings1;
        unsigned char game_settings2;
        unsigned char stun_time;
        unsigned char time_to_start;
        unsigned char time_to_respawn;
        unsigned char game_timeout;
        unsigned char max_respawn_count;
        unsigned char _reserv_9;
        unsigned char _CRC;
    }
    """


