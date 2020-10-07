
#include <Arduino.h>
#include <ESP8266WiFi.h>

WiFiClient client;
const char* ssid = "ssid_for_connection";
const char* password = "password_for_ssid";
const char* host = "192.168.2.6";
const int httpPort = 80;

int LEDPin = 2;

void setup()
{
    //make the LED pin output and initially turned off
    pinMode(LEDPin, OUTPUT);
    digitalWrite(LEDPin, LOW);

    Serial.begin(9600);
    delay(10);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("WiFi connected");
    Serial.println("IP address: "); Serial.println(WiFi.localIP());
}

int counter = 0;

void loop()
{
    counter++;
    delay(3000);

    Serial.print("connecting to "); Serial.println(host);

    if (!client.connect(host, httpPort))
    {
        Serial.println("connection failed");
        return;
    }

    String url;
    if(counter % 2 == 0)
    {
        digitalWrite(LEDPin, HIGH);
        url = "/LEDOff";
    }
    else
    {
        digitalWrite(LEDPin, LOW);
        url = "/LEDOn";
    }

    Serial.print("Requesting URL: "); Serial.println(url);

    // This will send the request to the server
    client.print(
        String("GET ") + url + " HTTP/1.1\r\n" +
        "Host: " + host + "\r\n" +
        "Connection: close\r\n\r\n"
    );

    unsigned long timeout = millis();
    while(client.available() == 0)
    {
        if (millis() - timeout > 5000)
        {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available())
    {
        String line = client.readStringUntil('\r');
        Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");
}
