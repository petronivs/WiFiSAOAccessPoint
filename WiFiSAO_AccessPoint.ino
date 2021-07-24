#include <ESP8266WiFi.h>
#include <FastLED.h>
#include <ESP8266WebServer.h>
#include <FS.h>


// ESP8266wifi DOCS: https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html
// FastLED docs: http://fastled.io/
// SPIFFS filesystem install docs (use newest version): https://randomnerdtutorials.com/install-esp8266-filesystem-uploader-arduino-ide/

#define NUM_LEDS 2
#define DATA_PIN 14

CRGB leds[NUM_LEDS];

// Set web server port number
//WiFiServer server(80);
ESP8266WebServer server(80);
//Variable to store teh HTTP request
String header;
String ssid = "SmartBadge";
int numClients;
String webPage = "<html><body><h1>Hello world, this is my smart badge!</h1></body></html>";



void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();

  // Initialize LEDs
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(  20 );
  leds[0] = CRGB::Black;
  leds[1] = CRGB::Black;
  FastLED.show();
  FastLED.show();
  
  
  Serial.print("Setting up access point");
  Serial.println(WiFi.softAP(ssid) ? "Success" : "Fail");

  Serial.print("Access Point AP Address");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/feedback.html", handleFeedback);
  server.begin();
  Serial.println("HTTP server started");

}

void loop() {
  // how many clients connected?
  if(numClients != WiFi.softAPgetStationNum()){
    numClients = WiFi.softAPgetStationNum();
    Serial.printf("Stations connected = %d\n", numClients);
    //delay(3000);
  }

  server.handleClient();

}

/* verification message on connect.  Go to http://192.168.4.1 in a web browser 
 *  connected to this access point to see it.
 *  (not currently working on phone, but works on laptop)
  */
void handleRoot() {
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    server.send(200, "text/html", "<html><body><h1>Hello world, this is my smart badge!<BR>Unfortunately, the file system is not working today.</h1></body></html>");     
  }
  else{
    File file = SPIFFS.open("/index.html", "r");
    if(!file){
      Serial.println("Failed to open file for reading");      
    }
    else {
      Serial.println();
      Serial.println("File Content:");
      while(file.available()){
        webPage = file.readString();
        //Serial.write(file.read());
        Serial.println(webPage);
      }
      file.close();
    }
  }
  server.send(200, "text/html", webPage); 
  Serial.println("Client request handled");
}

void handleFeedback(){
  Serial.println("Feedback handling");
  Serial.println(server.arg("contactbox"));
  
  server.send(200, "text/html", "<html><body><h1>Thank you for your feedback, " + server.arg("contactbox") + ". Unfortunately, I've already forgotten it!  Good talk.");
  //server.sendHeader("Location", "/");
  //server.send(303);
}
