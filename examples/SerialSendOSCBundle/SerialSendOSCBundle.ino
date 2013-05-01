
/*
Make an OSC bundle and send it over SLIP serial

OSCBundles allow OSCMessages to be grouped together
to  preserve the order and completeness of related messages.
They also allow for timetags to be carried to represent the presentation time
of the messages.
 */
#include <OSCBundle.h>

//Teensy and Leonardo variants have special USB serial
#if defined(CORE_TEENSY)|| defined(__AVR_ATmega32U4__)
#include <SLIPEncodedUSBSerial.h>
SLIPEncodedUSBSerial SLIPSerial(Serial);
#else
// any hardware serial port
#include <SLIPEncodedSerial.h>
SLIPEncodedSerial SLIPSerial(Serial);
#endif


void setup() {
  //begin SLIPSerial just like Serial
    SLIPSerial.begin(9600);   // set this as high as you can reliably run on your platform
    while(!Serial)
      ; //Leonardo "feature"
}

void loop(){
  //declare the bundle
  OSCBundle bndl;
  //BOSCBundle's add' returns the OSCMessage so the message's 'add' can be composed together
  bndl.add("/analog/0").add(analogRead(0));
  bndl.add("/analog/1").add(analogRead(1));
  bndl.add("/digital/5").add((digitalRead(5)==HIGH)?"HIGH":"LOW");
  bndl.send(SLIPSerial); // send the bytes to the SLIP stream
  SLIPSerial.endPacket(); // mark the end of the OSC Packet
  
  bndl.empty(); // empty the bundle to free room for a new one
  bndl.add("/mouse/step").add(analogRead(0)).add(analogRead(1));
  bndl.add("/units").add("pixels");
  bndl.send(SLIPSerial); // send the bytes to the SLIP stream
  SLIPSerial.endPacket(); // mark the end of the OSC Packet
  
  delay(1000);
}
