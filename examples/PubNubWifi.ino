/*
PubNub sample client

This sample client will use just the minimal-footprint raw PubNub
interface where it is your responsibility to deal with the JSON encoding.

It will just send a hello world message and retrieve one back, reporting
its deeds on serial console.

https://github.com/pubnub/LinkItONE

This code is in the public domain.
*/

#include "SPI.h"
 
#include "PubNub.h"
#include <LWiFi.h>
 

//#include "LWiFi\LWiFiClient.h"
#include <LWiFiClient.h>


#define WIFI_AP "RP95" // provide your WIFI_AP name
#define WIFI_PASSWORD "355972054522595" //provide your WIFI password
#define WIFI_AUTH LWIFI_WPA

const int subLedPin = 10;
const int pubLedPin = 13;

char pubkey[] = "demo";
char subkey[] = "demo";
char channel[] = "hello_world";

void setup()
{

    pinMode(subLedPin, OUTPUT);
    pinMode(pubLedPin, OUTPUT);
    digitalWrite(subLedPin, LOW);
    digitalWrite(pubLedPin, LOW);

    Serial.begin(9600);
    Serial.println("Serial set up");

     
    Serial.println("Connecting to AP");
    while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD)))
    {
        Serial.println(" . ");
        delay(1000);
    }
    Serial.println("Connected to AP");     

    PubNub.begin(pubkey, subkey);
    Serial.println("PubNub set up");
}

void flash(int ledPin)
{
    /* Flash LED three times. */
    for (int i = 0; i < 3; i++) {
        digitalWrite(ledPin, HIGH);
        delay(100);
        digitalWrite(ledPin, LOW);
        delay(100);
    }
}

void loop()
{
     

    LWiFiClient *client;

    Serial.println("publishing a message");

    client = PubNub.publish(channel, "\"\\\"Hello world!\\\" she said.\"");
     

    if (!client) {
        Serial.println("publishing error");
        delay(1000);
        return;
    }
    while (client->connected()) {
        while (client->connected() && !client->available()); // wait
        char c = client->read();
        Serial.print(c);
    }
    client->stop();
    Serial.println();
    flash(pubLedPin);

    Serial.println("waiting for a message (subscribe)");
    PubSubClient *pclient = PubNub.subscribe(channel);
    if (!pclient) {
        Serial.println("subscription error");
        delay(1000);
        return;
    }
    while (pclient->wait_for_data()) {
        char c = pclient->read();
        Serial.print(c);
    }
    pclient->stop();
    Serial.println();
    flash(subLedPin);

    Serial.println("retrieving message history");
    client = PubNub.history(channel);
    if (!client) {
        Serial.println("history error");
        delay(1000);
        return;
    }
    while (client->connected()) {
        while (client->connected() && !client->available()); // wait
        char c = client->read();
        Serial.print(c);
    }
    client->stop();
    Serial.println();

    delay(10000);
}
