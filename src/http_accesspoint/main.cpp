#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "ssid_for_accesspoint";
const char* password = "12345678";

ESP8266WebServer server(80);   //instantiate server at port 80 (http port)
String page = "";
int LEDPin = 2;

void setup(void)
{
    //the HTML of the web page
    page = "<h1>Simple NodeMCU Web Server</h1><p><a href=\"LEDOn\"><button>ON</button></a>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></p>";

    //make the LED pin output and initially turned off
    pinMode(LEDPin, OUTPUT);
    digitalWrite(LEDPin, LOW);

    delay(1000);
    Serial.begin(9600);
    WiFi.softAP(ssid, password); //begin WiFi access point
    Serial.println("");

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", []()
    {
        server.send(200, "text/html", page);
    });

    server.on("/LEDOn", []()
    {
        server.send(200, "text/html", page);
        digitalWrite(LEDPin, LOW);
        delay(1000);
    });

    server.on("/LEDOff", []()
    {
        server.send(200, "text/html", page);
        digitalWrite(LEDPin, HIGH);
        delay(1000);
    });

    server.begin();
    Serial.println("Web server started!");
}

void loop(void)
{
    server.handleClient();
}
