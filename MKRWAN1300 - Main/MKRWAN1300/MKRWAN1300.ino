//define libraries
#include <MKRWAN.h>
#include "DHT.h"
#include <Wire.h>
//define secrets file
#include "mkrwan-1300-02-secrets.h"

//Create LoRa Modem
LoRaModem modem;

// Set AppEUI and AppKey from secrets file
String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

//define datarate (5=SF7)
#define DATARATE 5
// Set ADR
#define ADR true

//create var for determing last packet sent
unsigned long lastData = 0;

//Setup DHT11
#define DHTPIN 7   
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

//set data rates
byte DR=DATARATE;  //0=SF12, 5=SF7
float ftemp=0;
byte downlink = 0;

void setup() {
  // change this to your regional band (eg. US915, AS923, ...)
  if (!modem.begin(US915)) {    
    while (1) {
      
    }
  };

  //connect to TTN over OTAA
  int connected = modem.joinOTAA(appEui, appKey);
  //Print error if fails
  if (!connected) {    
    while (1) {     
    }
  }

  //Start DHT11
  dht.begin();

  modem.minPollInterval(60);
  modem.setADR(ADR);
  modem.dataRate(DR);
}

void loop() {  
  // Read sensor values and multiply by 100 to effectively keep 2 decimals
  // Signed 16 bits integer, -32767 up to +32767
  int16_t t = dht.readTemperature() * 100;
  // Unsigned 16 bits integer, 0 up to 65535
  uint16_t h = dht.readHumidity() * 100;  

  //create the byte of data to be sent
  byte buffer[4];
  buffer[0] = t >> 8;
  buffer[1] = t;
  buffer[2] = h >> 8;
  buffer[3] = h;

  //Set counter to current EPOCH
  unsigned long now = millis();

  //check if 60s has elasped since it was set
  if (now - lastData > 1000*60)
  {
    //set last data to counter
    lastData = now;
    //create packet
    modem.beginPacket();
    //send packet
    modem.write(buffer, sizeof(buffer));  
 
    //print error if packet fails to send
    int err = modem.endPacket(true);
      if (err > 0)
      {

      }
      else
      {

      }
  }
 
}
