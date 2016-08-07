 /*
        -UID Transmitter over MQTT-
 */
     
    #include <ESP8266WiFi.h>
    #include <WiFiClient.h>
    #include <MQTTClient.h>
    #include <SPI.h>
    #include <MFRC522.h>
     
    #define SS_PIN D8
    #define RST_PIN D3
     
    const char* host = "rfid_reader";
    const char* ssid = "WIFISSID";
    const char* password = "WIFIPASS";
    const char* broker = "YOUR_MQTT_SERVER";
     
    WiFiClient wifi;
    MQTTClient mqtt;
     
    MFRC522 rfid(SS_PIN, RST_PIN);
    MFRC522::MIFARE_Key key;
      
    void connect();
     
    void setup(void){
      SPI.begin();
      rfid.PCD_Init();
     
      Serial.begin(115200);
      Serial.println();
      Serial.println("Connecting to Wifi...");
      WiFi.mode(WIFI_AP_STA);
      WiFi.begin(ssid, password);
      mqtt.begin(broker, wifi);
     
      connect();
    
      Serial.println("Wifi connected!");
    }
     
    void loop(void){
      if(!mqtt.connected()) {
        connect();
      }
      mqtt.loop();
      delay(10);
     
      handleRFID();
    }

    
     
    void connect() {
      while(WiFi.waitForConnectResult() != WL_CONNECTED){
        WiFi.begin(ssid, password);
        Serial.println("Wifi failed, retrying.");
      }
     
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
     
      while (!mqtt.connect(host)) {
        Serial.print(".");
      }
      Serial.println("\nMQTT connection ready!");
    }
    
     
    void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
      Serial.print("incoming: ");
      Serial.print(topic);
      Serial.print(" - ");
      Serial.print(payload);
      Serial.println();
    }
     
    void handleRFID() {
      if (!rfid.PICC_IsNewCardPresent()) return;
      if (!rfid.PICC_ReadCardSerial()) return;
      mqtt.publish("/hardware/rfid_reader/uid", printHex(rfid.uid.uidByte, rfid.uid.size));
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
     
     
    String printHex(byte *buffer, byte bufferSize) {
      String id = "";
      for (byte i = 0; i < bufferSize; i++) {
        id += buffer[i] < 0x10 ? "0" : "";
        id += String(buffer[i], HEX);
      }
      return id;
    }
