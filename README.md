================================================================
 Heltec LoRa JSON Messenger — TX/RX Scripts
================================================================

OVERVIEW
--------
This pair of Arduino sketches turns two Heltec LoRa boards
(e.g. Wireless Stick / WiFi LoRa 32 V3) into a simple point-to-
point text messenger. Messages typed into the Serial Monitor on
the TX board are packaged as JSON and sent over LoRa; the RX
board receives them, parses the JSON, and prints the message
back out over Serial.

Both sketches use the Heltec LoRaWan_APP library (Radio.* API)
and ArduinoJson for encoding/decoding the payload.

FILES
-----
1. lora_tx.ino  (Transmitter)
   - Reads a line of text from Serial.
   - Wraps it in a JSON object: {"msg": "<your text>"}
   - Serializes it to a string and sends it via Radio.Send().
   - Prints a confirmation once the radio reports TxDone.

2. lora_rx.ino  (Receiver)
   - Puts the radio into continuous receive mode (Radio.Rx(0)).
   - On packet arrival (onRxDone), null-terminates the raw bytes,
     prints them, and deserializes the JSON.
   - Extracts the "msg" field (other fields like sender/count
     are present in the code but currently commented out).
   - Immediately re-arms the receiver for the next packet.

RADIO CONFIGURATION
--------------------
Both boards must match on these settings to communicate:
   Frequency     : 915 MHz  (change SetChannel() if using a
                   different regional band, e.g. 868 MHz in EU)
   Modem         : LoRa
   Spreading F.  : SF7
   Bandwidth     : 125 kHz (index 0)
   Coding Rate   : 4/5 (index 1)
   Sync Word     : 0x12 (private network sync word)
   TX Power      : 14 dBm (transmitter only)

REQUIREMENTS
------------
- Two Heltec ESP32 LoRa boards (e.g. Wireless Stick, WiFi LoRa 32 V3)
- Heltec ESP32 board support package installed in Arduino IDE
- Libraries:
    * LoRaWan_APP (bundled with Heltec board package)
    * ArduinoJson (install via Library Manager)

USAGE
-----
1. Flash lora_rx.ino to one board and open its Serial Monitor
   at 115200 baud. It will print "LoRa JSON Receiver Online."
2. Flash lora_tx.ino to the other board and open its Serial
   Monitor at 115200 baud.
3. On the TX board's Serial Monitor, type a message and press
   Enter. It transmits as JSON, e.g. {"msg":"hello"}.
4. The RX board's Serial Monitor will print the raw JSON packet
   it received.

NOTES / KNOWN LIMITATIONS
--------------------------
- Several useful debug lines (sender name, packet count, RSSI/SNR
  link budget) are present in the code but commented out. Uncomment
  them in both sketches if you want richer diagnostics — just make
  sure the corresponding "sender" and "count" fields are also
  uncommented on the TX side, since the RX side expects them.
- No acknowledgment/retry logic: this is a simple fire-and-forget
  link. Packets can be lost with no notification to the sender.
- No encryption or authentication — anyone with a compatible LoRa
  radio and the same sync word can receive or spoof messages.
- TX blocks new sends until tx_idle is true again (i.e. until the
  previous transmission's TxDone callback fires).

================================================================