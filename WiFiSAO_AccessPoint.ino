#include <ESP8266WiFi.h>
#include <FastLED.h>
#include <ESP8266WebServer.h>


// ESP8266wifi DOCS: https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html
// FastLED docs: http://fastled.io/

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

/* verification message on connect.  Go to http://192.168.4.1 in a web browser 
 *  connected to this access point to see it.
 *  
  */
void handleRoot() {
  server.send(200, "text/html", "<h1>Hello world, this is my smart badge!</h1>"); 
  Serial.println("Client request handled");
}

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
//  WiFiClient client = server.available();
//
//  if (client){
//    Serial.println("NewClient.");
//    
//  // Display the HTML web page
//  client.println("<!DOCTYPE html><html>");
//  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
//  client.println("<link rel=\"icon\" href=\"data:,\">");
//  client.println("</head>");
//            
//  // Web Page Heading
//  client.println("<body><h1>WiFi SAO Web Server</h1>");
//            
//  // Display current state 
//  client.println("<p>Hello World</p>");
//  }

}
