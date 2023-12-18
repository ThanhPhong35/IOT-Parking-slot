#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include "FS.h"
#include "SPIFFS.h"
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
WebServer webServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN  5  // ESP32 pin GPIO5 
#define RST_PIN 4 // ESP32 pin GPI04 
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define buzzer 16

#include "DHT.h"
#define DHTPIN 35
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

String idcard;
String namecard;
int setupMode=0;

#include "loginPage.h"
#include "setupPage.h"
#include "dataPage.h"
#include "homePage.h"

String ssid_ap,pass_ap;
String user_login,pass_login;
String ssid_sta,pass_sta;
const byte btSetup=15;
const byte ledSignal=2;
boolean btSetupState=HIGH;
unsigned long timesConfig=millis();
boolean writeFile(fs::FS &fs, const char * path, String json);
String readFile(fs::FS &fs, const char * path);
boolean wifiMode=0;
boolean parkingState[6]={0,0,0,0,0,0};
const byte sensor1=14;
const byte sensor2=27;
const byte sensor3=26;
const byte sensor4=25;
const byte sensor5=33;
const byte sensor6=32;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
unsigned long showClock=millis();
String timeNow,dateNow;
int indexNum;
boolean waitIndex=0;

#include <ESP32Servo.h>
const byte servoIn_pin = 13;
const byte servoOut_pin = 12;
Servo servoIn;
Servo servoOut;
int servoInPos = 90;
int servoOutPos = 90;
const byte sensorIn = 17;
const byte sensorOut = 34;
unsigned long timeServoClose;


#include <Wire.h>                 //Thư viện giao tiếp I2C
#include <LiquidCrystal_I2C.h>    //Thư viện giao tiếp LCD theo chuẩn I2C
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); //Khai báo địa chỉ và thông tin LCD

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  Wire.begin();  

  lcd.init();                //Khởi tạo LCD
  lcd.clear();               //Xóa màn hình
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Welcome to");
  lcd.setCursor(0,1);
  lcd.print("IOT CAR PARKING");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Configuring in!");
  lcd.setCursor(4,1);
  lcd.print("progress!");

  SPI.begin(); // init SPI bus
  mfrc522.PCD_Init(); // init MFRC522
  if(!SPIFFS.begin()){
      Serial.println("SPIFFS Mount Failed");
      return;
  }
  pinMode(btSetup,INPUT_PULLUP);
  pinMode(ledSignal,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(sensor1,INPUT_PULLUP);
  pinMode(sensor2,INPUT_PULLUP);
  pinMode(sensor3,INPUT_PULLUP);
  pinMode(sensor4,INPUT_PULLUP);
  pinMode(sensor5,INPUT_PULLUP);
  pinMode(sensor6,INPUT_PULLUP);
  pinMode(sensorIn,INPUT_PULLUP);
  pinMode(sensorOut,INPUT_PULLUP);
  digitalWrite(buzzer,LOW);
  readConfig();

  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Setting up wifi!");

  setupWifi();
  if(wifiMode==1){ //Nếu đã kết nối được wifi
    timeClient.begin();
    timeClient.setTimeOffset(25200);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Wi-Fi connected!");
    lcd.setCursor(0,1);
    lcd.print("Ip:");
    lcd.print(WiFi.localIP().toString());
  }else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Please set up wf!");
  }
  setupWebServer();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  servoIn.attach(servoIn_pin);
  servoIn.write(servoInPos);
  servoOut.attach(servoOut_pin);
  servoOut.write(servoOutPos);

  delay(1000);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Have Slot:");
  lcd.setCursor(2,1);
  lcd.print("1");
  lcd.setCursor(4,1);
  lcd.print("2");
  lcd.setCursor(6,1);
  lcd.print("3");
  lcd.setCursor(8,1);
  lcd.print("4");
  lcd.setCursor(10,1);
  lcd.print("5");
  lcd.setCursor(12,1);
  lcd.print("6:");
  updateParkState();
  //----------------------------------------
  Serial.println("----------------------Init-------------------");
}

void loop() {
  if(wifiMode==1){
    getTime();
  }
 //Kiểm tra xe đã vào trong chưa, nếu đã vào thì 2 giây sau tự đóng cửa
if(servoInPos==0 && digitalRead(sensorIn)==0){//Khi xe đang đậu ngay cảm biến thì chờ
  timeServoClose = millis();
  //Serial.println("Xe quét qua cảm biến vào!");
}else if(servoInPos==0 && digitalRead(sensorIn)==1){//Xe đã rời khỏi thì đóng cổng sau 2s
  if(millis()-timeServoClose>500){
    //Serial.println("Xe rời cảm biến vào 5 giây!");
    //servoIn.attach(servoIn_pin); 
    //delay(500);
    for(int i=servoInPos;i<90;++i){ // điều chỉnh vòng lặp for
      servoIn.write(i);
      delay(10);
    }
    servoInPos=90;
    //servoIn.detach();
  }
}
//Kiểm tra xe đã ra chưa, nếu đã ra thì 2 giây sau tự đóng cửa
if(servoOutPos==0 && digitalRead(sensorOut)==0){
  timeServoClose = millis();
  //Serial.println("Xe quét qua cảm biến ra!");
}else if(servoOutPos==0 && digitalRead(sensorOut)==1){
  if(millis()-timeServoClose>500){
    //Serial.println("Xe rời cảm biến ra 5 giây!");
    //servoOut.attach(servoOut_pin); 
    //delay(500);
    for(int i=servoOutPos;i<90;++i){ // điều chỉnh vòng lặp for
      servoOut.write(i);
      delay(10);
    }
    servoOutPos=90;
    //servoOut.detach();
  }
}
  if(waitIndex==1){
    if(indexNum<0){
      for(int i=0;i<6;++i){
        if(parkingState[i]==0){
          switch(i){
              case 0:
                if(digitalRead(sensor1)==0) indexNum=0;
                break;
              case 1:
                if(digitalRead(sensor2)==0) indexNum=1;
                break;
              case 2:
                if(digitalRead(sensor3)==0) indexNum=2;
                break;
              case 3:
                if(digitalRead(sensor4)==0) indexNum=3;
                break;
              case 4:
                if(digitalRead(sensor5)==0) indexNum=4;
                break;
              case 5:
                if(digitalRead(sensor6)==0) indexNum=5;
                break;
            }
        }
      }
    }else{
      DynamicJsonDocument doc(700);
      String str = readFile(SPIFFS,"/dataParking.json");
      deserializeJson(doc, str);
      JsonArray data = doc.createNestedArray(idcard);
      data.add(namecard);
      data.add(indexNum);
      data.add(timeNow);
      data.add(dateNow);
      str="";
      serializeJson(doc, str);
      //Serial.println("Data new: " +str);
      writeFile(SPIFFS,"/dataParking.json",str);
      //Serial.println("Đã thêm dữ liệu mới!");
      //Serial.println("Data: "+str);
      updateParkState();
      String timeInt = timeNow;
      String dateInt = dateNow;
      String state = "IN";
      String dataSend = "{\"n\":\""+ namecard +"\"," +
                          "\"i\":"+ indexNum + "," +
                          "\"d\":\""+ dateInt + "\"," +
                          "\"t\":\""+ timeInt + "\"," +
                          "\"s\":\""+ state + "\"}";
      Serial.println(dataSend);
      beep(200);
      sendidcard();
      waitIndex=0;
    }  
  }
  webServer.handleClient();
  webSocket.loop();
  checkButtonReset();

  //Chờ đọc thẻ RFID
  readUID();

}
void beep(int t){
  digitalWrite(buzzer,HIGH);
  delay(200);
  digitalWrite(buzzer,LOW);
}
void checkButtonReset(){
  if(digitalRead(btSetup)==LOW){
    if(btSetupState==HIGH){
      timesConfig=millis();
      Serial.println("Ấn giữ 15 giây để reset về mặc định!");
      btSetupState=LOW;
    }else{
      digitalWrite(ledSignal,!digitalRead(ledSignal));
      delay(500);
    }
    if(millis()-timesConfig>15000){
      for(int i=0;i<20;++i){
        digitalWrite(ledSignal,!digitalRead(ledSignal));
        delay(100);
      }
      DynamicJsonDocument doc(512);
      doc["ssid_ap"]="BAI GIU XE";
      doc["pass_ap"]="12345678";
      doc["user_login"]="admin";
      doc["pass_login"]="admin";
      doc["ssid_sta"]="";
      doc["pass_sta"]="";
      String str="";
      serializeJson(doc, str);
      writeString(0,512,str);
      EEPROM.commit();
      Serial.println("Đã reset về trạng thái mặc định!");
      delay(2000);
      ESP.restart();
    }
  }else{
    btSetupState=HIGH;
    if(wifiMode==0){
      if(millis()-timesConfig>200){
        digitalWrite(ledSignal,!digitalRead(ledSignal));
        timesConfig=millis();
      }
    }else if(wifiMode==1){
      if(millis()-timesConfig>2000){
        digitalWrite(ledSignal,!digitalRead(ledSignal));
        timesConfig=millis();
      }
    }
  }
}
void readConfig(){
  Serial.println();
  Serial.println("=============================================");
  Serial.println("*          Đọc thông tin cấu hình!          *");
  Serial.println("=============================================");
  String str=readString(0,512); //max343
  DynamicJsonDocument doc(512);
  deserializeJson(doc, str);
  //serializeJsonPretty(doc, Serial);//serializeJson(doc, Serial);
  JsonObject obj = doc.as<JsonObject>();
  ssid_ap = obj["ssid_ap"].as<String>();
  pass_ap = obj["pass_ap"].as<String>();
  user_login = obj["user_login"].as<String>();
  pass_login = obj["pass_login"].as<String>();
  ssid_sta = obj["ssid_sta"].as<String>();
  pass_sta = obj["pass_sta"].as<String>();
  Serial.println("----------------Chế độ phát wifi-------------");
  Serial.println("Tên wifi: " + ssid_ap);
  Serial.println("Mật khẩu: " + pass_ap);
  Serial.println("--------------Chế độ kết nối wifi------------");
  Serial.println("Tên wifi: " + ssid_sta);
  Serial.println("Mật khẩu: " + pass_sta);
  Serial.println("--------------Thông tin webserver------------");
  Serial.println("Tên đăng nhập: " + user_login);
  Serial.println("Mật khẩu: " + pass_login);
}
void writeConfig(){
  DynamicJsonDocument doc(512);
  doc["ssid_ap"]=ssid_ap;
  doc["pass_ap"]=pass_ap;
  doc["user_login"]=user_login;
  doc["pass_login"]=pass_login;
  doc["ssid_sta"]=ssid_sta;
  doc["pass_sta"]=pass_sta;
  String str="";
  serializeJson(doc, str);
  writeString(0,512,str);
  EEPROM.commit();
  Serial.println("Đã lưu cấu hình thành công!");
}
void setupWifi(){
  Serial.println("=============================================");
  Serial.println("*          Thiết lập chế độ wifi!           *");
  Serial.println("=============================================");
  if(ssid_sta!=""){
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid_sta.c_str(),pass_sta.c_str());
    Serial.println("Đang kết nối wifi: " + ssid_sta);
    int n=0;
    while(n<20){
      if(WiFi.status()==WL_CONNECTED){
        Serial.println("\nĐã kết nối đến wifi!");
        Serial.print("Ip kết nối: ");Serial.println(WiFi.localIP());
        wifiMode=1;
        break;
      }
      n++;
      digitalWrite(ledSignal,!digitalRead(ledSignal));
      delay(500);
      Serial.print(".");
    }
  }
  if(wifiMode==0){
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid_ap.c_str(),pass_ap.c_str(), 1, false,4);
    Serial.println("\nĐã phát ra wifi mới!");
    Serial.println("Tên wifi: " + String(ssid_ap) +"\nMật khẩu:" + pass_ap);
    Serial.println("Ip webserver: " + WiFi.softAPIP().toString());
  }
}
void setupWebServer(){
  webServer.on("/",[]{
    setupMode=0;
    String s = FPSTR(homePage);
    webServer.send(200,"text/html",s);
  });
  webServer.on("/home",[]{
    setupMode=0;
    String s = FPSTR(homePage);
    webServer.send(200,"text/html",s);
  });
  webServer.on("/login",[]{
    String s = FPSTR(loginPage);
    webServer.send(200,"text/html",s);
  });
  webServer.on("/setup",[]{
    String s = FPSTR(setupPage);
    webServer.send(200,"text/html",s);
  });
  webServer.on("/data",[]{
    String s = FPSTR(dataPage);
    webServer.send(200,"text/html",s);
  });
  webServer.on("/checkLogin",checkLogin);
  webServer.on("/saveLogin",saveLogin);
  webServer.on("/getConfig",getConfig);
  webServer.on("/getWifiList",getWifiList);
  webServer.on("/saveConfig",saveConfig);
  webServer.on("/getDataStore",getDataStore);
  webServer.on("/addDataStore",addDataStore);
  webServer.on("/deleteDataStore",deleteDataStore);
  webServer.on("/getDataParking",getDataParking);
  webServer.on("/reStart",[]{ESP.restart();});
  webServer.onNotFound([]{
    String s = FPSTR(homePage);
    webServer.send(200,"text/html",s);
  });
  webServer.begin();
  Serial.println("---------------------------------------------");
  Serial.println("Đã khởi tạo Web server!");
}
void checkLogin(){
  String user_lg = webServer.arg("user_login");
  String pass_lg = webServer.arg("pass_login");
  String s;
  if(user_lg==user_login && pass_lg==pass_login){
    s="OK";
  }else{
    s="Tên đăng nhập hoặc mật khẩu không đúng!";
  }
  webServer.send(200,"text/plain",s);
}
void saveLogin(){
  user_login = webServer.arg("user_login");
  pass_login = webServer.arg("pass_login");
  writeConfig();
  String s = "Đã lưu thông tin đăng nhập!";
  webServer.send(200,"text/plain",s);
}
void getConfig(){
  String str = "";
  str = readString(0,512);
  webServer.send(200,"application/json",str);  
  //Serial.println(str);
}
void getWifiList(){
  int n = WiFi.scanNetworks();
  delay(500);
  String ssidList = "[\""; 
  for(int i=0;i<n;++i){
    ssidList+= WiFi.SSID(i)+ "\""; //{"keyword":["wifi1","wifi2",...]}
    if(i<(n-1)){
      ssidList += ",\"";
    }
  }
  ssidList += "]";
  String s = "{\"ssidList\": "+ ssidList + "}";
  webServer.send(200,"application/json",s);
  //Serial.println(s);
}
void saveConfig(){
  String str = webServer.arg("plain");
  //Serial.println(str);
  writeString(0,512, str);
  EEPROM.commit();
  String s = "Cấu hình đã được lưu thành công. Vui lòng khởi động lại thiết bị!";
  webServer.send(200,"text/html",s);
}
void reStart(){
  ESP.restart();
}
void writeString(int n, int m, String str){
  if(str.length()<=(m-n)){
    for(int i=n; i<m;++i){
      EEPROM.write(i,0);
      delay(1);
    }
    for(int i=0; i<str.length();++i){
      EEPROM.write(n+i,str[i]);
    }
  }else{
    Serial.println("Dữ liệu vược quá vùng nhớ thiết lập!");
  }
}
String readString(int n, int m){
  String str="";
  for(int i=n;i<m;++i){
    str += char(EEPROM.read(i));
    delay(1);
  }
  str = str.c_str();
  return str;
}
void getDataStore(){
  //Serial.println("Get data store!");
  setupMode=1;
  String str = readFile(SPIFFS,"/dataStore.json");
  webServer.send(200,"application/json",str);  
  //Serial.println(str);
}
void addDataStore(){
  //Serial.println("Add data store!");
  String idcard = webServer.arg("idcard");
  String namecard = webServer.arg("namecard");
  String str = readFile(SPIFFS,"/dataStore.json");
  //Serial.println("Data old: " +str);
  DynamicJsonDocument doc(2024);
  deserializeJson(doc, str);
  doc[idcard]=namecard;
  str="";
  serializeJson(doc, str);
  //Serial.println("Data new: " +str);
  writeFile(SPIFFS,"/dataStore.json",str);
  String s="Đã thêm dữ liệu mới!";
  webServer.send(200,"text/html",s);
  //Serial.println(str);
}
void deleteDataStore(){
  Serial.println("Delete data store!");
  String idcard = webServer.arg("idcard");
  String str = readFile(SPIFFS,"/dataStore.json");
  DynamicJsonDocument doc(2024);
  deserializeJson(doc, str);
  doc.remove(idcard);
  str="";
  serializeJson(doc, str);
  writeFile(SPIFFS,"/dataStore.json",str);

  str = readFile(SPIFFS,"/dataParking.json");
  deserializeJson(doc, str);
  doc.remove(idcard);
  str="";
  serializeJson(doc, str);
  writeFile(SPIFFS,"/dataParking.json",str);

  String s="Đã xóa dữ liệu!";
  webServer.send(200,"text/html",s);
  //Serial.println(str);
}
void updateParkState(){
  String str = readFile(SPIFFS,"/dataParking.json");
  //Serial.println(str);
  DynamicJsonDocument doc(700);
  deserializeJson(doc, str);
  JsonObject obj = doc.as<JsonObject>();
  //Serial.print("Parking full: ");
  for(int i=0;i<6;++i){
    parkingState[i]=0;
  }
  for (JsonPair kv : obj) {
    int index = kv.value()[1].as<int>();
    //Serial.print(String(index)+" ");
    parkingState[index]=1;
  }
  //Serial.println();
  //Serial.print("Parking state:");
  int numSlot=0;
  String temp="";
  boolean value=0;
  for(int i=0;i<6;++i){
    value = parkingState[i];
    if(value==0) temp="O";
    else temp="X";
    //Serial.print(String(value));
    if(value==0){
      numSlot = numSlot+1;
    }
    switch(i){
      case 0:
        lcd.setCursor(3,1);
        lcd.print(temp);
        break;
      case 1:
        lcd.setCursor(5,1);
        lcd.print(temp);
        break;
      case 2:
        lcd.setCursor(7,1);
        lcd.print(temp);
        break;
      case 3:
        lcd.setCursor(9,1);
        lcd.print(temp);
        break;
      case 4:
        lcd.setCursor(11,1);
        lcd.print(temp);
        break;
      case 5:
        lcd.setCursor(13,1);
        lcd.print(temp);
        break;
    }
  }
  lcd.setCursor(13,0);
  lcd.print(String(numSlot));
  Serial.println();
}
void getDataParking(){
  //Serial.println("Get data Parking!");
  String str = readFile(SPIFFS,"/dataParking.json");
  webServer.send(200,"application/json",str);  
  //Serial.println(str);
}
void webSocketEvent(uint8_t num, WStype_t type,uint8_t * payload,size_t length){
  String payloadString = (const char *)payload;
  Serial.print("payloadString= ");
  Serial.println(payloadString);
  if(payloadString=="setupStoreOn"){
    setupMode=1;
  }else if(payloadString=="setupStoreOff"){
    setupMode=0;
  }
  Serial.print("Setup Mode: ");Serial.println(setupMode);
}
void sendidcard(){
  String JSONtxt = "{\"idcard\": \""+ idcard +"\"," +
                    "\"namecard\": \""+ namecard +"\"}";
  webSocket.broadcastTXT(JSONtxt);
  //Serial.println(JSONtxt);
}
String readFile(fs::FS &fs, const char * path){
  File file = fs.open(path);
  if(!file|| file.isDirectory()){
    Serial.println("File not found!");
    return "null";
  }
  String buf;
  while(file.available()){
    buf += (char)file.read();
  }
  file.close();
  return buf;
}
boolean writeFile(fs::FS &fs, const char * path, String json){
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return false;
  }
  if (file.print(json)) {
    //Serial.println("File written");
    delay(100);
    file.close();
    return true;
  } else {
    Serial.println("Write failed");
    return false;
  }
}
void readUID(){
  unsigned long uidDec, uidDecTemp;  
  // Tim the moi
  if ( ! mfrc522.PICC_IsNewCardPresent()) return;
  // Doc the
  if ( ! mfrc522.PICC_ReadCardSerial()) return;
  uidDec = 0;
  // Hien thi so UID cua the
  for (byte i = 0; i < mfrc522.uid.size; i++){
    uidDecTemp = mfrc522.uid.uidByte[i];
    uidDec = uidDec*256+uidDecTemp;
  } 
  //Serial.println("Card UID: " + String(uidDec));
  //mfrc522.PICC_HaltA(); 
  idcard=String(uidDec);
  String str = readFile(SPIFFS,"/dataStore.json");
  DynamicJsonDocument doc(2024);
  deserializeJson(doc, str);
  JsonObject obj = doc.as<JsonObject>();
  namecard = obj[idcard].as<String>();
  if(namecard=="null") namecard="";
  //Serial.println("Name card: " + namecard);
  sendidcard();
  if(setupMode==1){
    if(namecard==""){
      Serial.println("Thẻ chưa được lưu!");
      beep(1000);
      delay(200);
      beep(1000);
    }else{
      beep(200);
    }
  }else{
    if(namecard==""){
      Serial.println("Thẻ chưa được lưu!");
      beep(1000);
      delay(200);
      beep(1000);
    }else{
      //Thẻ có trong hệ thống
      str = readFile(SPIFFS,"/dataParking.json");
      deserializeJson(doc, str);
      obj = doc.as<JsonObject>();
      bool hasIdcard = obj.containsKey(idcard);
      if(hasIdcard==0){ //Kiểm tra xem trong bãi xe có tồn tại không? nếu không
        if(waitIndex==0){
          indexNum=-1;
          waitIndex=1;
          beep(200);
          Serial.println("Wait index setup!");
          //servoIn.attach(servoIn_pin); 
          //delay(500);
          servoInPos = 90;
          for(int i=servoInPos;i>0;--i){
            servoIn.write(i);
            delay(10);
          }
          servoInPos=0;
          //servoIn.detach();
        }
      }else{
        indexNum = obj[idcard][1].as<int>();
        String timeOut = timeNow;
        String dateOut = dateNow;
        String state = "OUT";
        String dataSend = "{\"n\":\""+ namecard +"\"," +
                           "\"i\":"+ indexNum + "," +
                           "\"d\":\""+ dateOut + "\"," +
                           "\"t\":\""+ timeOut + "\"," +
                           "\"s\":\""+ state + "\"}";
        Serial.println(dataSend);
        beep(200);
        doc.remove(idcard);
        str="";
        serializeJson(doc, str);
        writeFile(SPIFFS,"/dataParking.json",str);
        Serial.println("Đã xóa dữ liệu!");
        //Serial.println("Data: "+str);
        updateParkState();
        sendidcard();

        //servoOut.attach(servoOut_pin); 
        //delay(500);
        servoOutPos=90;
        for(int i=servoOutPos;i>0;--i){
          servoOut.write(i);
          delay(10);
        }
        servoOutPos=0;
        //servoOut.detach();
      }
    }
  }
}
void getTime(){
  timeClient.update();
  if(millis()-showClock>1000){
    timeNow = timeClient.getFormattedTime();
    // Serial.print("Formatted Time: ");
    // Serial.println(timeNow);  

    time_t epochTime = timeClient.getEpochTime();
    // Serial.print("Epoch Time: ");
    //Serial.println(epochTime);
    //Get a time structure
    struct tm *ptm = gmtime ((time_t *)&epochTime); 
    int monthDay = ptm->tm_mday;
    int currentMonth = ptm->tm_mon+1;
    int currentYear = ptm->tm_year+1900;
    //Print complete date:
    dateNow = String(monthDay) + "/" + String(currentMonth) + "/" + String(currentYear);
    // Serial.print("Current date: ");
    // Serial.println(dateNow);
    showClock=millis();
  }
}
