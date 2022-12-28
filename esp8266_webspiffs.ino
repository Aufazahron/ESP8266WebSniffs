#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>

//SSID PASSWORD
const char* ssid = "Muthi Shop"; 
const char* password = "ditfamut123"; 

// PORT WEBSITE 
AsyncWebServer server(80); 

// INI STATIC IP
IPAddress IP(192,168,100,100); 
IPAddress SUBNET(255,255,255,0); 
IPAddress GATEWAY(192,168,100,1); 


String ledState;
String ledState2;

String processor(const String& var){
  if(var == "STATE"){
    if(digitalRead(2)){
      ledState = "";
    }
    else{
      ledState = "checked";
    }
    return ledState;
  }
  if(var == "STATE2"){
    if(digitalRead(4)){
      ledState2 = "";
    }
    else{
      ledState2 = "checked";
    }
    return ledState2;
  }
  return String();
}

// VOID VOID AN //
void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  // INITIALIZE SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  //Wifi
  wifi();

  //server
  iniserver();
  
}

void loop() {
  // put your main code here, to run repeatedly:

}

//---------------------------------------------//
void wifi(){
// Mengatur WiFi 
  WiFi.begin(ssid, password); 
  WiFi.config(IP,GATEWAY,SUBNET);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

// Status Connect
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void iniserver(){
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
     request->send(SPIFFS, "/index.html", String(), false, processor);
  });
   server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String parameter;
    String parameter2;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam("state")) {
      parameter = request->getParam("state")->value();
      digitalWrite(2, parameter.toInt());
    }
    if (request->hasParam("state2")) {
      parameter2 = request->getParam("state2")->value();
      digitalWrite(4, parameter2.toInt());
    }
    
    request->send(200, "text/plain", "OK");
  });
  server.begin();
}
