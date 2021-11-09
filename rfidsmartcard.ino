#include <ESP8266WiFi.h>                 
#include <FirebaseArduino.h> 
#include <ArduinoJson.h> 
#include <ESP8266HTTPClient.h>   
#include <SPI.h>
#include <MFRC522.h>         
 
#define FIREBASE_HOST "smart-card-for-bus-service-default-rtdb.firebaseio.com"      
#define FIREBASE_AUTH "WS9CCCznvHZt8naO2YvHXqKEvCVoBaEwc9Byp2FF"            
#define WIFI_SSID "IoT"                                  
#define WIFI_PASSWORD "12345678"
 
#define SS_PIN D2
#define RST_PIN D3
#define buzzer D8
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

void setup() 
{
  Serial.begin(115200);
   SPI.begin(); // Initiate  SPI bus                                                
               
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   Serial.println();                                  
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    pinMode(buzzer,OUTPUT);
  }
 
  Serial.println();
  Serial.print("Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());                               //prints local IP address
     
  mfrc522.PCD_Init();   // Initiate MFRC522
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                 // connect to the firebase

  Serial.println("Put your card to the reader...");
  Serial.println();

}

void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  String content= "";
  Serial.print("Tag UID : ");
  Serial.println(content);

  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : "");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : ""));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Tag UID : ");
  Serial.println(content);
  content.toUpperCase();
  Firebase.setString("CardID/value",content); //setup path to send content readings
  if (Firebase.failed()) 
   {
     Serial.print("pushing/logs failed:");
     Serial.print(Firebase.error()); 
      return;
  }
  else
  {
    digitalWrite(buzzer, HIGH);
    delay(100);
    Serial.print("RFID's Data has been Received...");
    digitalWrite(buzzer,LOW);
  }
   delay(2000);
}
