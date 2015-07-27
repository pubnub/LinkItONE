/*
PubNub sample client

This sample client will use just the minimal-footprint raw PubNub
interface where it is your responsibility to deal with the JSON encoding.

It will just send a hello world message and retrieve one back, reporting
its deeds on serial console.

https://github.com/pubnub/LinkItONE

This code is in the public domain.
*/


#include <SPI.h>
#include "PubNub.h"

//import all the necessary files for GPRS connectivity
#include "LGPRS.h"
#include "LGPRSClient.h"

const int subLedPin = 8;
const int pubLedPin = 9;

//define the required keys for using PubNub
char pubkey[] = "demo";
char subkey[] = "demo";
char channel[] = "hello_world";

void setup()
{
    

    Serial.begin(9600);
    Serial.println("Serial setup");

    //Connect to the GRPS network in order to send/receive PubNub messages
    Serial.println("Attach to GPRS network with correct APN settings from your mobile network provider");
    
    //attachGPRS(const char *apn, const char *username, const char *password);
    while (!LGPRS.attachGPRS("wap.cingular","wap@cingulargprs.com","cingular1"))
    {
        Serial.println(" . ");
        delay(1000);
    }


    Serial.println("LGPRS setup");

    PubNub.begin(pubkey, subkey);
    Serial.println("PubNub setup");
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
    

    LGPRSClient *client;

    Serial.println("publishing a message");
  //  client = PubNub.publish(channel, "[{\"latlng\":[32.6271,-123.685],\"data\":\"hello1\"}]", 60);
  client = PubNub.publish(channel, "\"\\\"Hello world!\\\".\"");
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

