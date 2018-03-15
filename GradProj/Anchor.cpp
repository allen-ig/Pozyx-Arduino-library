#include "Anchor.h"
#include "Pozyx.h"
#include "Pozyx_definitions.h"
#include "Wire.h"
#include "math.h"

Anchor::Anchor(uint16_t anchor_id, uint8_t ranging_protocol, signed int range_step_mm, float scale){
  this->anchor_id = anchor_id;
  this->ranging_protocol = ranging_protocol;
  this->range_step_mm = range_step_mm;
  this->scalePos = scale;
  device_range_t range;
}

uint16_t Anchor::GetID(){
  return anchor_id;
}

float Anchor::GetDistance(){
  return range.distance;
}

float Anchor::GetTimeStamp(){
  return range.timestamp;
}

float Anchor::GetRSS(){
  return range.RSS;
}

float Anchor::GetScale(){
  return scalePos;
}
int Anchor::Status(){
  return Pozyx.doRanging(this->GetID(), &range);
}

float Anchor::Adjust(Anchor *other){
  float first = this->GetDistance();
  float second = other->GetDistance();
  float scale = other->GetScale() - this->GetScale();
  return sqrt(abs(pow(first, 2) - pow((scale / 2 + (pow(first, 2) - pow(second, 2)) / (2 * scale)), 2)));
}


