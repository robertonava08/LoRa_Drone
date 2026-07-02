#include "LoRaWan_APP.h"
#include <ArduinoJson.h>

RadioEvents_t RadioEvents;

void onRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);

void setup() {
    Serial.begin(115200);
    Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);

    RadioEvents.RxDone = onRxDone;
    Radio.Init(&RadioEvents);

    Radio.SetChannel(915000000); // 915 MHz
    Radio.SetRxConfig(MODEM_LORA, 0, 7, 1, 0, 8, 0, false, 0, true, 0, 0, false, true);
    Radio.SetSyncWord(0x12); 

    Serial.println("LoRa JSON Receiver Online. Awaiting incoming packets...");
    Radio.Rx(0); 
}

void loop() {
    Radio.IrqProcess(); 
}

void onRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) {
    // Null-terminate the raw payload string safely
    char rxBuffer[size + 1];
    memcpy(rxBuffer, payload, size);
    rxBuffer[size] = '\0';

   // Serial.println("\n-------------------------------------------");
   // Serial.print("Raw Link Packet: ");
    Serial.println(rxBuffer);

    // Allocate memory and deserialize the JSON string
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, rxBuffer);

    if (error) {
        //Serial.print("JSON Parsing Failed: ");
       // Serial.println(error.f_str());
    } else {
        // Extract and print specific fields cleanly
        //const char* sender = doc["sender"];
        const char* msg = doc["msg"];
       // int count = doc["count"];

       // Serial.println(">>> Parsed JSON Object successfully:");
       // Serial.printf("  Origin Device: %s\n", sender);
        //Serial.printf("  Packet Index : %d\n", count);
       // Serial.printf("  Payload Text : \"%s\"\n", msg);
       // Serial.printf("Link Budget    -> RSSI: %d dBm | SNR: %d dB\n", rssi, snr);
    }

    // Re-initialize continuous listening mode instantly
    Radio.Rx(0); 
}