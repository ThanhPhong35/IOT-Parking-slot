#include "HTTPSRedirect.h"
#include <WiFiClientSecure.h>

#include <ArduinoJson.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <WebServer.h>
WebServer webServer(80);
#include "loginPage.h"
#include "setupPage.h"
String ssid_ap,pass_ap;
String user_login,pass_login;
String ssid_sta,pass_sta;
const byte btSetup=0;
const byte ledSignal=2;
boolean btSetupState=HIGH;
unsigned long timesConfig=millis();

boolean wifiMode=0;

const char* host = "script.google.com";
// Replace with your own script id to make server side changes
const char *GScriptId = "AKfycbzCJDxtlFvvnLMoWPDAjQq4V-Xz_E5iXqzkuiqogMkXEkzBNo-pOnxMQJzWkX4ALDYP"; // Receiving data from google script address
String payload_base =  "{\"command\": \"insert_row\", \"sheet_name\": \"Sheet1\", \"values\": ";
const int httpsPort = 443;
String payload = "";
HTTPSRedirect* client = nullptr;
String namecard, date_ , time_ , state;
int indexNum;
boolean sendState = 0;

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  
  pinMode(btSetup,INPUT_PULLUP);
  pinMode(ledSignal,OUTPUT);
  
  readConfig();

  delay(1000);

  setupWifi();
  if(wifiMode==1){ //Nếu đã kết nối được wifi
    //----------------------------------------------------------
    // Use HTTPSRedirect class to create a new TLS connection
    client = new HTTPSRedirect(httpsPort);
    client->setInsecure();
    client->setPrintResponseBody(true);
    client->setContentTypeHeader("application/json");
    //----------------------------------------------------------
    
    delay(5000);
    //----------------------------------------------------------
    Serial.print("Connecting to ");
    Serial.println(host);
    //----------------------------------------------------------
    // Try to connect for a maximum of 5 times
    bool flag = false;
    for(int i=0; i<5; i++){ 
      int retval = client->connect(host, httpsPort);
      //*************************************************
      if (retval == 1){
        flag = true;
        String msg = "Connected. OK";
        Serial.println(msg);
      
        delay(2000);
        break;
      }
      //*************************************************
      else
        Serial.println("Connection failed. Retrying...");
      //*************************************************
    }
    //----------------------------------------------------------
    if (!flag){
      //____________________________________________
      
      //____________________________________________
      Serial.print("Could not connect to server: ");
      Serial.println(host);
      delay(5000);
      return;
      //____________________________________________
    }
    //----------------------------------------------------------
    delete client;    // delete HTTPSRedirect object
    client = nullptr; // delete HTTPSRedirect object
    //----------------------------------------------------------
  }else{
    Serial.println("Vui lòng kết nối wifi!");
  }
  setupWebServer();
  //----------------------------------------
  Serial.println("----------------------Init-------------------");
}

void loop() {
  if(Serial.available()){
    String buff="";
    while(Serial.available()>0){
      buff = Serial.readStringUntil('\n');
    }
    int checkChar1 = -1;
    int checkChar2 = -1;
    checkChar1 = buff.indexOf('{');
    checkChar2 = buff.indexOf('}');
    if((checkChar1>=0) && (checkChar2>=0)){
      Serial.println("Data rec:"+buff);
      DynamicJsonDocument doc(200);
      deserializeJson(doc, buff);
      JsonObject obj = doc.as<JsonObject>();
      namecard = obj["n"].as<String>();
      indexNum = obj["i"].as<int>();
      date_ = obj["d"].as<String>();
      time_ = obj["t"].as<String>();
      state = obj["s"].as<String>();
      sendState = 1;
    }
  }
  if(wifiMode==1){
    //----------------------------------------------------------------
    static bool flag = false;
    if (!flag){
      client = new HTTPSRedirect(httpsPort);
      client->setInsecure();
      flag = true;
      client->setPrintResponseBody(true);
      client->setContentTypeHeader("application/json");
    }
    if (client != nullptr){
      //when below if condition is TRUE then it takes more time then usual, It means the device 
      //is disconnected from the google sheet server and it takes time to connect again
      //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
      if (!client->connected()){
        int retval = client->connect(host, httpsPort);
        if (retval != 1){
          Serial.println("Disconnected. Retrying...");
          
          return; //Reset the loop
        }
      }
      //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
    }
    else{Serial.println("Error creating client object!"); Serial.println("else");}
    //----------------------------------------------------------------

    if(sendState==1){
      sendData();
      sendState = 0;
    }
  }
  webServer.handleClient();
  checkButtonReset();

}

// Subroutine for sending data to Google Sheets
void sendData() {
  String url = String("/macros/s/") + GScriptId+ "/exec";
  payload = payload_base + "\"" + date_ + "," + time_ + "," + namecard + "," + indexNum + "," + state + "\"}";
  // Publish data to Google Sheets
  Serial.println("Publishing data...");
  Serial.println(payload);
  if(client->POST(url, host, payload)){ 
    // do stuff here if publish was successful
    Serial.println("[OK] Data published.");
  }
  else{
    // do stuff here if publish was not successful
    Serial.println("Error while connecting");
  }
  //----------------------------------------Processing data and sending data
  
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
      doc["ssid_ap"]="BAI GIU XE2";
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
    String s = FPSTR(setupPage);
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
  webServer.on("/checkLogin",checkLogin);
  webServer.on("/saveLogin",saveLogin);
  webServer.on("/getConfig",getConfig);
  webServer.on("/getWifiList",getWifiList);
  webServer.on("/saveConfig",saveConfig);
  webServer.on("/reStart",[]{ESP.restart();});
  webServer.onNotFound([]{
    String s = FPSTR(setupPage);
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
