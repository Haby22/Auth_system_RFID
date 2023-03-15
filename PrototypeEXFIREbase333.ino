
//*******************************libraries********************************
//ESP32----------------------------
#include <NTPClient.h>
#include <HttpClient.h>
#include <WiFi.h>
#include "time.h"

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
//RFID-----------------------------
#include <SPI.h>
#include <MFRC522.h>
//************************************************************************
#define RST_PIN         22           // Configurable, see typical pin layout above
#define SS_PIN          5                // Configurable, see typical pin layout above
//Connection au WIFI-----------------------------
#define CONNECTION_TIMEOUT 10
#define WIFI_SSID "Abiba"
#define WIFI_PASSWORD "Itisme2000"
#include <SD.h>

//************************************************************************
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
//************************************************************************

//FIREBASE DATABASE-----------------------------
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
// Insert Firebase project API Key
#define API_KEY "AIzaSyDCmJjTBwnh9R9420tCgcOmR7TYBenvZpQ" //A MODIFIER
// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://esp32-d96d7-default-rtdb.firebaseio.com/" //A MODIFIER
// Insert Authorized Email and password
#define USER_EMAIL "yayehaby2000@gmail.com" //A MODIFIER
#define USER_PASSWORD "Abiba2000" //A MODIFIER

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;
String uidPath= "/";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;
//**************************FONCTIONS********************************
//*******************************************************************
//Connection au WIFI-----------------------------
void connect(){
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  int timeout_counter = 0;
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  timeout_counter++;
  if(timeout_counter >= CONNECTION_TIMEOUT*5){
  ESP.restart();
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}
//*******************************************************************
void rfidscan(){
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Merci D'approcher votre carte sur le lecteur...");
  Serial.println();
}
//*******************************************************************
//FIREBASE DATABASE-----------------------------
void loaddataonfirebase(){
  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /*Assign the user sign in credentials*/
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("Connection établie avec la firebase...");
    Serial.println();
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}
//*******************************************************************
void setup() {
  connect();
  rfidscan();
  loaddataonfirebase();
  getDatafromfirestore();
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
}
//*******************************************************************
void array_to_string(byte array[], unsigned int len, char buffer[])
{
   for (unsigned int i = 0; i < len; i++)
   {
      byte nib1 = (array[i] >> 4) & 0x0F;
      byte nib2 = (array[i] >> 0) & 0x0F;
      buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
      buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
   }
   buffer[len*2] = '\0';
}
//*******************************************************************
void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  //Serial.println(&timeinfo, "%d/%B/%Y,%H:%M:%S");
  //Serial.println(output_time);
  //Serial.println(output_Date);
  char output_time [14];
  char output_Date [14];
  String output_TIME=output_time;
  String output_DATE=output_Date;
  strftime(output_time, 14, "%H:%M:%Ss" ,&timeinfo);
  strftime(output_Date, 14, "%d/%B/%Y" ,&timeinfo);
  Serial.println(output_time);
  Serial.println(output_Date);
  
  Serial.println(); 
}
void getDatafromfirestore(){
// [END get_collection]
}
//*******************************************************************
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void loop(){
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
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  content.toUpperCase();
  delay(1);
  String s="salifou";
  char strUID[32] = "";
  String p="mohamed";
  array_to_string(mfrc522.uid.uidByte, 4, strUID); //Insert (byte array, length, char array for output)
  Serial.println(strUID);//Print the output uid string
  Serial.println("OKAY SUIVANT");
  mfrc522.PICC_HaltA();
  String strUIDD=strUID;
  char output_time [14];
  String output_TIME=output_time;
  char output_Date [14];
  String output_DATE=output_Date;

  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
  sendDataPrevMillis = millis();
  // Write an Float number on the database path test/float
  // LA FONCTION QUI ECRIS DANS LA FIREBASE LE NOM
  //************************************************11111111111********************************************
  if (Firebase.RTDB.setString(&fbdo, ("USERS/"+ strUIDD+"/NOM/"),p)){
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
  //***************************************************2222222222************************
  if (Firebase.RTDB.setString(&fbdo, ("USERS/"+ strUIDD+"/DATE/"),output_DATE)){
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
  //******************************************************3333333333******************
  if (Firebase.RTDB.setString(&fbdo, ("USERS/"+ strUIDD+"/HEURE/"),output_TIME)){
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
  }
  //*********************************************
  printLocalTime();
}
