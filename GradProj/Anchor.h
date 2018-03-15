#ifndef Anchor_H
#define Anchor_H

#include "Pozyx.h"
#include "Pozyx_definitions.h"
#include "Wire.h"

class Anchor{
  public:
    float GetDistance();
    float GetTimeStamp();
    float GetRSS();
    float GetScale();
    float Adjust(Anchor *other);
    uint16_t GetID();
    int Status();
    Anchor(uint16_t anchor_id, uint8_t ranging_protocol, signed int range_step_mm, float scale);
  private:
    signed int range_step_mm;
    uint16_t anchor_id;
    uint8_t ranging_protocol;
    float scalePos;
    device_range_t range;
};

#endif

