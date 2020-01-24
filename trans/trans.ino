#include <SPI.h>
#include <Ethernet.h>

//MAC Address for the ardiuno shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 0, 1);

String a;
float lat,lon;
int c[5];
//Initializing ethernet client 
EthernetClient client;

String incomingString;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("Initialize Ethernet with DHCP:"));
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Failed to configure Ethernet using DHCP"));
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println(F("Ethernet shield was not found.  Sorry, can't run without hardware. :("));
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println(F("Ethernet cable is not connected."));
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.println(F("  DHCP assigned IP "));
    Serial.println(Ethernet.localIP());
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    incomingString = Serial.readString();
  
    if(incomingString.indexOf("A") > 0){
      Serial.println("Recived "+incomingString);
      int count = 0;
     for (int i = 0; i < incomingString.length(); i++) {
          if (incomingString.substring(i, i+1) == ",") {
            c[count] = i+1;
            count++;
          }
}     
  a = incomingString.substring(c[2], (c[3] -1));


if (client.connect("sadns.000webhostapp.com", 80)) {
    Serial.println("-> Connected");
    // Make a HTTP request:
    client.print("GET /data.php?"); //GET request to write data to the database.
      client.print("accidentStatus=");
      client.print(a);
      client.print("&productID=");
      client.print("123456");
      client.println(" HTTP/1.1"); 
      client.println("Host: sadns.000webhostapp.com"); 
      client.println("Connection: close"); 
      client.println(); 
      client.println(); 
      client.stop();
   
    } else {
    // if you didn't get a connection to the server:
    Serial.println(F("connection failed"));
    }
  }


    if(incomingString.indexOf("L") > 0){
      Serial.println("Recived "+incomingString);
      int count = 0;
     for (int i = 0; i < incomingString.length(); i++) {
          if (incomingString.substring(i, i+1) == ",") {
            c[count] = i+1;
            count++;
          }
}     
  lat = incomingString.substring(c[2], (c[3] -1)).toFloat();
  lon = incomingString.substring(c[3], (c[4] -1)).toFloat();


if (client.connect("sadns.000webhostapp.com", 80)) {
    Serial.println("-> Connected");
    // Make a HTTP request:
    client.print("GET /data.php?"); //GET request to write data to the database.
      client.print("&long=");
      client.print(lon);
      client.print("&lat=");
      client.print(lat);
      client.print("&productID=");
      client.print("123456");
      client.println(" HTTP/1.1"); 
      client.println("Host: sadns.000webhostapp.com"); 
      client.println("Connection: close"); 
      client.println(); 
      client.println(); 
      client.stop();
   
    } else {
    // if you didn't get a connection to the server:
    Serial.println(F("connection failed"));
    }
  }
}
}
