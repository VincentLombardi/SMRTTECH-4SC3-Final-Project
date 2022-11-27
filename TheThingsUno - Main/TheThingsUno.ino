//define libraries
#include <TheThingsNetwork.h>
#include "DHT.h"
//define secrets file
#include "ttn-un-01-secrets.h"

// Set AppEUI and AppKey from secrets file
const char *appEui = SECRET_APP_EUI;
const char *appKey = SECRET_APP_KEY;

//define lora and debug serial
#define loraSerial Serial1
#define debugSerial Serial
 
//define region
#define freqPlan TTN_FP_US915

//Setup DHT11
#define DHTPIN 2   
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
 
//setup TTN
TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);
 
void setup()
{
  loraSerial.begin(57600);

  //Join TTN
  ttn.join(appEui, appKey);

  //print join status
  ttn.showStatus();

  //Start DHT11
  dht.begin();

}
 
void loop()
{
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
    
  // Send it off
  ttn.sendBytes(buffer, sizeof(buffer));

  //wait 1 minute  
  delay(60000);

  
}