# MQTT-RFID_MRC522
##Publish the UID of an RFID Card, Sticker or KeyChip to an MQTT Server.

#Hardware
- ESP8266 (ESP-01 is not supported)
- MRC522
- RFID Tags

#Software
- Arduino IDE (1.6.9)
- ESP8266 Boards (2.3.0)
- MQTT Lib (1.10.1)


#FHEM integration 
```sh
define rfid_reader MQTT_DEVICE
attr rfid_reader IODev MQTT (your mqtt device)
attr rfid_reader stateFormat transmission-state
attr rfid_reader subscribeReading_uid /hardware/rfid_reader/uid
```
