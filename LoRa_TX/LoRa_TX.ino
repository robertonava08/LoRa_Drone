#include "LoRaWan_APP.h"
#include <ArduinoJson.h>

RadioEvents_t RadioEvents;
bool tx_idle = true;
uint32_t packet_counter = 0;

void onTxDone() {
    Serial.println(">>> LoRa Packet Broadcast Complete!");
    tx_idle = true; 
}

void setup() {
    Serial.begin(115200);
    Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);
    
    RadioEvents.TxDone = onTxDone;
    Radio.Init(&RadioEvents);
    
    Radio.SetChannel(915000000); // 915 MHz
    Radio.SetTxConfig(MODEM_LORA, 14, 0, 0, 7, 1, 8, false, true, 0, 0, false, 3000);
    Radio.SetSyncWord(0x12); 

    Serial.println("Type a message in the bar above and hit Enter to transmit JSON...");
}

void loop() {
    Radio.IrqProcess();

    if (Serial.available() && tx_idle) {
        String userInput = Serial.readStringUntil('\n');
        userInput.trim(); 

        if (userInput.length() > 0) {
            tx_idle = false; 
            packet_counter++;

            // Create JSON document
             JsonDocument doc;
          //  doc["sender"] = "Heltec_V3_TX";
            doc["msg"] = userInput;
         //   doc["count"] = packet_counter;

            // Serialize JSON into a string buffer
            String jsonString;
            serializeJson(doc, jsonString);

            Serial.print("Serializing & Sending JSON: ");
            Serial.println(jsonString);
            
            // Transmit over the air
            Radio.Send((uint8_t*)jsonString.c_str(), jsonString.length());
        }
    }
}