#include "MT2Protocol.h"

//*

const bool CheckCRC(void* blob, uint16_t size){
    if(size == MT2_HIT_LEN || size==MT2_MSG_LEN){
        return true;
    }else if(size%8<0){
        const uint8_t data_size = size/8 - sizeof(MT2_MessageBlob)-1;
        uint8_t* data_ptr = ((uint8_t*)blob)+sizeof(MT2_MessageBlob);
        uint8_t crc = data_ptr[data_size]; // нет -1, т.к последний элемент - CRC
        int sum=0;
        for(int i=0; i<data_size; i++){
            sum += data_ptr[i];
        }
        return (crc == sum%0x100)? true : false;
    }else{
        return false;
    }
}


const uint8_t MT2_DecodeDamage(uint8_t damage_id){
    static const uint8_t _MT2_DAMAGE[] = {
        1,
        2,
        4,
        5,
        7,
        10,
        15,
        17,
        20,
        25,
        30,
        35,
        40,
        50,
        75,
        100
    };
    return _MT2_DAMAGE[damage_id];
}

const uint16_t MT2_DecodeHealth(uint8_t byte){
    if(byte<0x14)
        return byte;
    else if(byte<0x39)
        return 20+(byte-0x14)*5;
    else if(byte<0x48)
        return 250+(byte-0x39)*50;
    else
        return 999;
}

const uint16_t MT2_DecodeGunRPM(uint8_t byte){
    return (250+byte*50);
}

const uint16_t MT2_DecodeStunTime(uint8_t byte){
    if(byte<0x04)
        return byte*250;
    else
        return 1+(byte-0x04);
}
//*/




/*
constexpr uint8_t MT2_Hit::_MT2_DAMAGE[16] = {
    1,
    2,
    4,
    5,
    7,
    10,
    15,
    17,
    20,
    25,
    30,
    35,
    40,
    50,
    75,
    100
};

const uint16_t MT2_Configure::getHeilth(){
    uint8_t byte = conf_blob->respawn_health;
    if(byte<0x14)
        return byte;
    else if(byte<0x39)
        return 20+(byte-0x14)*5;
    else if(byte<0x48)
        return 250+(byte-0x39)*50;
    else
        return 999;
}
const uint16_t MT2_Configure::getStunTimeMS(){
    uint16_t byte = conf_blob->stun_time;
    if(byte<0x04)
        return byte*250;
    else
        return 1+(byte-0x04);
}


const bool MT2_Package::CheckCRC(){
    if(size == MT2_HIT_LEN || size==MT2_MSG_LEN){
        return true;
    }else if(size%8<0){
        const uint8_t data_size = size/8 - sizeof(MT2_MessageBlob)-1;
        uint8_t* data_ptr = ((uint8_t*)blob)+sizeof(MT2_MessageBlob);
        uint8_t crc = data_ptr[data_size]; // нет -1, т.к последний элемент - CRC
        int sum=0;
        for(int i=0; i<data_size; i++){
            sum += data_ptr[i];
        }
        return (crc == sum%0x100)? true : false;
    }else{
        return false;
    }
}
const MT2_PackageType MT2_Package::getType(){
    return (((uint8_t*)blob)[0] < 0x80)?
        MT2_PackageType::HIT : MT2_PackageType(((uint8_t*)blob)[0]);
}
//*/
