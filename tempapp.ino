#include <SPI.h>
#include <WiFi.h>
#include <WiFiUDP.h>

int tempPin = 0;
char ssid[] = "arta";     
char pass[] = "123456789";    
int status = WL_IDLE_STATUS;     

byte host[] = {127, 0, 0, 1};

int port = 8086;

WiFiUDP udp;

void setup() {
    Serial.begin(9600);

    Serial.println("Attempting to connect to WPA network...");
    status = WiFi.begin(ssid, pass);

    if ( status != WL_CONNECTED) { 
        Serial.println("Couldn't get a WiFi connection");
        while(true);
    } 
    else {
        Serial.println("Connected to network");
        IPAddress ip = WiFi.localIP();
        Serial.print("IP Address: ");
        Serial.println(ip);

        long rssi = WiFi.RSSI();
        Serial.print("signal strength (RSSI):");
        Serial.print(rssi);
        Serial.println(" dBm");
    }
}

float getTemperature() {
    int reading = analogRead(tempPin);

    float voltage = reading * 5.0;
    voltage /= 1024.0; 

    float temperatureC = (voltage - 0.5) * 100 ;  

    float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;

    return temperatureF;
}

void loop() {
    String line, temperature;

    delay(1000);

    temperature = String(getTemperature(), 2);

    line = String("temperature value=" + temperature);
    Serial.println(line);

    Serial.println("Sending UDP packet...");
    udp.beginPacket(host, port);
    udp.print(line);
    udp.endPacket();
}