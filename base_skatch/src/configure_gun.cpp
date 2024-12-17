#include "configure_gun.h"

bool JsonToConfig(String json, MT2_Configure& blob){
    JsonDocument doc;
    Serial.println(json);
    auto error = deserializeJson(doc, json);
    if(error){
        return true;
    }else{
        blob.damage =           doc[F("gst_gun_damage")];
        blob.respawn_health =   doc[F("gst_player_live_count")];
        blob.stun_time =        doc[F("gst_stun_time")];
        blob.game_settings1 &=  bool(doc[F("gst_friendlyfire")])<<3;
        blob.burst_size =       doc[F("gst_gun_burst_size")];
        blob.fire_temp =        doc[F("gst_gun_fire_temp")];
        blob.reload_type =      doc[F("gst_gun_reload_type")];
        blob.reload_time =      doc[F("gst_gun_reload_time")];
        blob.clip_size =        doc[F("gst_gun_clip_size")];
        blob.clip_count =       doc[F("gst_gun_clip_count")];
        
        uint8_t *blob_bytes = ((uint8_t*)(&blob));
        uint32_t sum = 0;
        for(int i=0; i<36; i++){
            sum += blob_bytes[i];
        }
        blob._CRC = sum%0x100;
        return 0;
    }
}
