 /**
  The Pozyx ready to range tutorial (c) Pozyx Labs
  Please read the tutorial that accompanies this sketch: https://www.pozyx.io/Documentation/Tutorials/ready_to_range/Arduino

  This demo requires two Pozyx devices and one Arduino. It demonstrates the ranging capabilities and the functionality to
  to remotely control a Pozyx device. Place one of the Pozyx shields on the Arduino and upload this sketch. Move around
  with the other Pozyx device.

  This demo measures the range between the two devices. The closer the devices are to each other, the more LEDs will
  light up on both devices.
*/

#include <Pozyx.h>
#include <Pozyx_definitions.h>
#include <Wire.h>

////////////////////////////////////////////////
////////////////// PARAMETERS //////////////////
////////////////////////////////////////////////

uint16_t anchors[2] = {0x6862, 0x6834};
float scale = 8410.0;
int anchorNum = sizeof(anchors);
int stat[2];
float distances[2];
float RSS[2];
//Anchor* first = new Anchor(0x6862, POZYX_RANGE_PROTOCOL_PRECISION, 1000, 0);
//Anchor* second = new Anchor(0x6834, POZYX_RANGE_PROTOCOL_PRECISION, 1000, 1335);

//XBee xbee = XBee(13);

//uint16_t destination_id = 0x6834;     // the network id of the other pozyx device: fill in the network id of the other device
signed int range_step_mm = 1000;      // every 1000mm in range, one LED less will be giving light.

uint8_t ranging_protocol = POZYX_RANGE_PROTOCOL_PRECISION; // ranging protocol of the Pozyx.

uint16_t remote_id = 0x6028;          // the network ID of the remote device
bool remote = false;                  // whether to use the given remote device for ranging

////////////////////////////////////////////////

void setup(){
  
  Serial.begin(115200);
  if(Pozyx.begin() == POZYX_FAILURE){
    Serial.println("ERROR: Unable to connect to POZYX shield");
    Serial.println("Reset required");
    delay(100);
    abort();
  }
  // setting the remote_id back to NULL will use the local Pozyx
  if (!remote){
    remote_id = NULL;
  }
  Serial.println("------------POZYX RANGING V1.1------------");
  Serial.println("NOTES:");
  Serial.println("- Change the parameters:");
  Serial.println("\tdestination_id (target device)");
  Serial.println("\trange_step (mm)");
  Serial.println();
  Serial.println("- Approach target device to see range and");
  Serial.println("led control");
  Serial.println("------------POZYX RANGING V1.1------------");
  Serial.println();
  Serial.println("START Ranging:");
/*
  // make sure the pozyx system has no control over the LEDs, we're the boss
  uint8_t led_config = 0x0;
  Pozyx.setLedConfig(led_config, remote_id);
  // do the same with the remote device
  Pozyx.setLedConfig(led_config, destination_id);
  // set the ranging protocol
  */
  Pozyx.setRangingProtocol(ranging_protocol, remote_id);

}

void loop(){
  device_range_t range1;
  device_range_t range2;
  device_range_t ranges[2] = {range1,range2};
  //float distance_to_first = first->GetDistance();
  //float distance_to_second = second->GetDistance();
  boolean statusAll = true;
  stat[0] = Pozyx.doRanging(anchors[0], &range1);
  stat[1] = Pozyx.doRanging(anchors[1], &range2);
  /*for (int i = 0; i < anchorNum; i++){
    stat[i] = Pozyx.doRanging(anchors[i], &ranges[i]);
  }

  for (int i = 0; i < anchorNum; i++){
    if (stat[i] == false) {
      statusAll = false;
      break; 
    }
  }
*/
  if (statusAll){
    float RSStemp = 0.0;
    int temp = 0;
    /*for (int i = 0; i < anchorNum; i++){
      distances[i] = ranges[i].distance;
      RSS[i] = ranges[i].RSS;
      if (RSS[i] > RSStemp) {
        RSStemp = RSS[i];
        temp = i;
      }
    }*/
    Serial.print("POS,0x6028,");
    float minus11 = 0, minus12 = 0;         //previous distances between anchor1 and tag
    float minus21 = 0, minus22 = 0;         //previous distances between anchor2 and tag
    float res1 = 0, res2 = 0;

    res1 = (minus11 + minus12 + range1.distance) / 3;
    res2 = (minus21 + minus22 + range2.distance) / 3;
    minus22 = minus21;
    minus12 = minus11;
    minus21 = range2.distance;
    minus11 = range1.distance;
    
    Serial.println(adjust(res2, res1, scale), DEC);
    //Serial.println(res, DEC);
    //Serial.write(range1.distance);
    Serial.println(adjust(range2.distance, range1.distance, scale), DEC);
    //Serial.println(range1.distance);
    //Serial.println(range2.distance);
    //Serial.println(first->GetScale() + distance_to_first);
    //Serial.print("second result: ");
    //Serial.println(second->GetScale() - distance_to_second);  
  }else{
    Serial.println("ERROR: ranging");
  }
  delay(200);
}

float adjust(float distance1, float distance2, float scale){
  //distance1 = sqrt(pow(distance1, 2) - pow(50, 2));
  //distance2 = sqrt(pow(distance2, 2) - pow(50, 2));
  float solution = distance1 * (scale - distance1 - distance2) / (distance2 + 1 + distance1);
  return distance1 + solution;
}


