// Test code for Adafruit GPS modules using MTK3329/MTK3339 driver
//
// This code shows how to listen to the GPS module in an interrupt
// which allows the program to have more 'freedom' - just parse
// when a new NMEA sentence is available! Then access data when
// desired.
//
// Tested and works great with the Adafruit Ultimate GPS module
// using MTK33x9 chipset
//    ------> http://www.adafruit.com/products/746
// Pick one up today at the Adafruit electronics shop
// and help support open source hardware & software! -ada

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <SPI.h>

String signLat,signLon;
SoftwareSerial mySerial(8, 7);
Adafruit_GPS GPS(&mySerial);

const int buttonPin = 3; 
String location = "Dhiraj";
//Variables
int buttonState = 0;
int flag=0;
int payload=0;
unsigned long previousMillis = 0; 
 const unsigned long interval = 10000;

#define GPSECHO  true

void setup()
{

  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);
  delay(5000);
  Serial.println("Adafruit GPS library basic test!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);
  // Ask for firmware version
  mySerial.println(PMTK_Q_RELEASE);

 
 
}

uint32_t timer = millis();
void loop()                     // run over and over again
{
  buttonState = digitalRead(buttonPin);
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
 

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }

  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis())  timer = millis();

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
    if (GPS.fix) {
     sendGPS(); 
    }
  }

  
  if(buttonState == HIGH){
    if(flag==0){ 
      Serial.println("AT+SEND=8,3,A,1");
      Serial.println("\n");
      delay(3000);
      flag=1;
  }else{
    flag=0;
  }
}
}

void sendGPS(){

     unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval){
      previousMillis += interval;
      if(GPS.lat == 'N' && GPS.lon == 'E'){
        signLat = "+";
        signLon = "+";
      }
       if(GPS.lat == 'N' && GPS.lon == 'W'){
        signLat = "+";
        signLon = "-";
      }
       if(GPS.lat == 'S' && GPS.lon == 'W'){
        signLat = "-";
        signLon = "-";
      }
       if(GPS.lat == 'S' && GPS.lon == 'E'){
        signLat = "-";
        signLon = "+";
      }
      location = signLat + String(GPS.latitude) + "," +signLon+String(GPS.longitude);
      payload = location.length()+2;
      Serial.println("AT+SEND=8,"+String(payload)+",L,"+location+"");
      Serial.println("\n");
    }
}
