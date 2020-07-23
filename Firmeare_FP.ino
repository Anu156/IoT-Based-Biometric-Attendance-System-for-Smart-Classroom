/*//#include <SerialESP8266wifi.h>//https://forum.arduino.cc/ says am I a Joke to you
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <Adafruit_Fingerprint.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <ESP8266HTTPClient.h>
SoftwareSerial mySerial(13, 15,false,256);//rx(blue)-D7,tx(yellow)-D8
StaticJsonDocument<200> doc;
uint8_t id;
char dev_id='1';
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
// Replace these with your WiFi network settings
const char* ssid = "archu"; //replace this with your WiFi network name
const char* password = "tryagain"; //replace this with your WiFi network password
//uint8_t getFingerprintEnroll(int id);
void deleteRecords();
uint8_t getFingerprintID();
void deleteRecoeds()
{
    finger.emptyDatabase();
    
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}

void setup() {
  delay(1000);
   EEPROM.begin(512);
    Serial.begin(115200);
  Serial1.begin(9600);
  finger.begin(57600);
    if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
 
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("success!");
  Serial.print("IP Address is: ");
  Serial.println(WiFi.localIP());
  // put your setup code here, to run once:

}
int GetIdOnSerialSingle()
{
    while(true){
      int id=getFingerprintIDez();
      if(id>0){
      //Serial.println(id);
      return id;
      id=0;
      break;
      }
    } 
  }
  uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}
uint8_t getFingerprintEnroll() {
int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}
int getTotalNumberofFP()
{
 return finger.templateCount ;
  }
int Enroll()
{
    Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
 id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
     return 0;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);
  
  while (!  getFingerprintEnroll() );
}
void StartAttandance()
{
  HTTPClient http;
  http.begin("http://192.168.43.119/recieve.php");
  while(true)
  {
    String lec_id=read_String(1);
    int id=getFingerprintIDez();
    if(id!=-1){
//    doc["Lecture_Id"]=lec_id;
//    doc["fp_id"]=id;
    //serializeJson(doc, Serial); 
//deserializeJson(doc, http);
http.addHeader("Content-Type", "application/json");  
 String postData = "{\"Lecture_Id\":\""+lec_id+"\",\"fp_id\":\""+String(id)+"\"}";
    //Serial.println(postData);

  int httpCode = http.POST(postData);

    String payload = http.getString();
    Serial.println(httpCode);
    //Serial.println(payload);
    }
  }
  }
  String read_String(char add)
{
  int i;
  char data[100]; //Max 100 Bytes
  int len=0;
  unsigned char k;
  k=EEPROM.read(add);
  while(k!='\0' && len<500)   //Read until null character
  {    
    k=EEPROM.read(add+len);
    
    data[len]=k;
    
    len++;
    
  }
  data[len]='\0';
  return String(data);
}
    void writeString(char add,String data)
{
  int _size = data.length();
  int i;
  for(i=0;i<_size;i++)
  {
    EEPROM.write(add+i,data[i]);
  }
  EEPROM.write(add+_size,'\0');   //Add termination null character for String Data
  EEPROM.commit();
}
void loop() {
if(Serial.available()>0)
  {
    int incomingByte = Serial.read();
int choice = incomingByte;
    //Serial.println(choice);
String a;
    switch(choice)
   { 
    case '1':
           Serial.println('1');
           break;

        case '2':
            while(!Serial.available()>0);
            a=Serial.readString();
            writeString(1,a); //Store data to EEPROM
           StartAttandance();
           break;

        case '3':
           // handle '3'
           break;

        default:
           // handle unwanted input here
           break;
   }
    }
  
}
//  int a;
//  if(Serial.available())
//  {
//    a=Serial.read();
//    }
//    else
//    {
//      return;
//      }
//      if(a=='1'){
//GetIdOnSerialSingle();
//  Serial1.println("Back to main");
//      }else if (a=='2'){
//Enroll();
//      }else if (a=='3'){
//deleteRecoeds();
//      }else if (a=='4'){
//int t=getTotalNumberofFP();
//Serial.print("Number of Templates: ");Serial1.println(t);
//      }
//  delay(50); 



*/

#include <ESP8266WiFi.h>
#include <Adafruit_Fingerprint.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <ESP8266HTTPClient.h>

#define I2C_ADDRESS 0x3C
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

SSD1306AsciiWire oled;

SoftwareSerial mySerial(13, 15,false,256);
StaticJsonDocument<200> doc;
uint8_t id;
char dev_id='1';
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
// Replace these with your WiFi network settings
const char* ssid = "archu"; //replace this with your WiFi network name
const char* password = "tryagain"; //replace this with your WiFi network password
//uint8_t getFingerprintEnroll(int id);
void deleteRecords();
uint8_t getFingerprintID();
void deleteRecoeds()
{
    finger.emptyDatabase();                                                                       
    
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}

void setup() {
  delay(1000);
   EEPROM.begin(512);
    Serial.begin(115200);
  Serial1.begin(9600);
  finger.begin(57600);

  Wire.begin();
oled.begin(&Adafruit128x32, I2C_ADDRESS);
oled.set400kHz();
oled.setFont(Iain5x7);

    if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
 
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Connecting");
  oled.clear();
  oled.set2X();
  oled.println(" Connecting..");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
   oled.setCursor(2,2);
  oled.set2X();
  oled.println(" ......");
    Serial.print(".");
  }

  Serial.println("success!");
  oled.clear();
  oled.set2X();
  oled.println(" success!");
  Serial.print("IP Address is: ");
  Serial.println(WiFi.localIP());


}
int GetIdOnSerialSingle()
{
    while(true){
      int id=getFingerprintIDez();
      if(id>0){
      //Serial.println(id);
      return id;
      id=0;
      break;
      }
    } 
  }
  uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}
uint8_t getFingerprintEnroll() {
int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}
int getTotalNumberofFP()
{
 return finger.templateCount ;
  }
int Enroll()
{
    Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
 id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
     return 0;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);
  
  while (!  getFingerprintEnroll() );
}
void StartAttandance()
{
  HTTPClient http;
  http.begin("http://192.168.43.199/recieve.php");//api id
  while(true)
  {
    String lec_id=read_String(1);
    int id=getFingerprintIDez();
    if(id!=-1){
//    doc["Lecture_Id"]=lec_id;
//    doc["fp_id"]=id;
    //serializeJson(doc, Serial); 
//deserializeJson(doc, http);
http.addHeader("Content-Type", "application/json");  
 String postData = "{\"Lecture_Id\":\""+lec_id+"\",\"fp_id\":\""+String(id)+"\"}";
    //Serial.println(postData);

  int httpCode = http.POST(postData);

    String payload = http.getString();
    Serial.println(httpCode);
    //Serial.println(payload);
    }
  }
  }
  String read_String(char add)
{
  int i;
  char data[100]; //Max 100 Bytes
  int len=0;
  unsigned char k;
  k=EEPROM.read(add);
  while(k!='\0' && len<500)   //Read until null character
  {    
    k=EEPROM.read(add+len);
    
    data[len]=k;
    
    len++;
    
  }
  data[len]='\0';
  return String(data);
}
    void writeString(char add,String data)
{
  int _size = data.length();
  int i;
  for(i=0;i<_size;i++)
  {
    EEPROM.write(add+i,data[i]);
  }
  EEPROM.write(add+_size,'\0');   //Add termination null character for String Data
  EEPROM.commit();
}
void loop() {
if(Serial.available()>0)
  {
    int incomingByte = Serial.read();
int choice = incomingByte;
    //Serial.println(choice);
String a;
    switch(choice)
   { 
    case '1':
           Serial.println('1');
           break;

        case '2':
            while(!Serial.available()>0);
            a=Serial.readString();
            writeString(1,a); //Store data to EEPROM
           StartAttandance();
           break;

        case '3':
           // handle '3'
           break;

        default:
           // handle unwanted input here
           break;
   }
    }
  
}
//  int a; h
//  if(Serial.available())
//  {
//    a=Serial.read();
//    }
//    else
//    {
//      return;
//      }
//      if(a=='1'){
//GetIdOnSerialSingle();
//  Serial1.println("Back to main");
//      }else if (a=='2'){
//Enroll();
//      }else if (a=='3'){
//deleteRecoeds();
//      }else if (a=='4'){
//int t=getTotalNumberofFP();
//Serial.print("Number of Templates: ");Serial1.println(t);
//      }
//  delay(50); 
