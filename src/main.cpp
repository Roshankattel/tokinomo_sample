/*
  Read Tokinomo system Prototype

  PIR and SD card Module Interfacing with Arduino

   Typical pin layout used:
   ----------------------------------
                            Arduino Uno
              PIR/SD card   MCU
   Signal      Pin          Pin
   ----------------------------------
   PIR DATA    DATA         A0
   LED         LED          8
   SPEAKER                  9
   SPI MOSI    MOSI         D7 (GPIO13)
   SPI MISO    MISO         D6 (GPIO12)
   SPI SCK     SCK          D5 (GPIO14)
   SPI SS      SDA(SS)      D4 (GPIO2)
   RST/Reset   RST          D3 (GPIO0)
   IRQ         IRQ          N/C
   3.3V        3.3V         3.3V
   GND         GND          GND

*/

#include <Arduino.h>
#include <SD.h>     // need to include the SD library
#include <TMRpcm.h> //Arduino library for asynchronous playback of PCM/WAV files
#include <SPI.h>    //  need to include the SPI library

TMRpcm tmrpcm; // create an object for use in this sketch

#define SD_ChipSelectPin 4 // connect pin 4 of arduino to cs pin of sd card
#define PIR_SENEOR A0
#define LED_PIN 8
#define RE_TRIGGER_INTERVAL 30 // In seconds

unsigned long time;
bool play = false;
uint16_t pirSersorValue = 0;

void setup()
{
  pinMode(PIR_SENEOR, INPUT);
  tmrpcm.speakerPin = 9; // Speaker Pin
  Serial.begin(112500);
  if (!SD.begin(SD_ChipSelectPin)) // returns 1 if the card is present
  {
    Serial.println("SD fail");
    return;
  }
  tmrpcm.setVolume(4); //
  // tmrpcm.play("b.wav"); // the sound file "song" will play each time the arduino powers up, or is reset
  // try to provide the file name with extension
}

void loop()
{
  if (pirSersorValue > 500)
  {
    if (!play)
    {
      Serial.print("Detected at: ");
      Serial.println(time);
      digitalWrite(LED_PIN, HIGH);
      tmrpcm.play("b.wav"); // the sound file "song" will play each time the arduino powers up, or is reset
                            // try to provide the file name with extension
      play = true;
      time = millis();
    }
  }

  if (play)
  {
    if (millis() - time >= RE_TRIGGER_INTERVAL * 1000) // Retigger in 30 sec
    {
      Serial.print("Timeout at: ");
      Serial.println(millis());
      play = false;
      digitalWrite(LED_PIN, LOW);
      pirSersorValue = 0;
    }
  }
  else
  {
    pirSersorValue = analogRead(PIR_SENEOR);
  }
}