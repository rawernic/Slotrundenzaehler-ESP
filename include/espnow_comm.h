#ifndef ESPNOW_COMM_H
#define ESPNOW_COMM_H

#include <Arduino.h>

#ifdef ESP32
    #include <esp_now.h>
    #include <WiFi.h>
#else
    #include <espnow.h>
    #include <ESP8266WiFi.h>
#endif

#include <ArduinoJson.h>

// Max size of this struct is 250 bytes - ESP NOW buffer limit
struct DataPackage {
    char sensorType[16];  // z.B. "master", "satellite1"
    int lane_nr;
    unsigned long time_ms;
};

struct SendPackage {
    char sensorType[16];  // z.B. "master", "satellite1"
    int lane_nr;
    unsigned long time_ms;
};

// JSON-Paket für ESP-NOW (max 250 Bytes)
struct JsonPackage {
    char jsonData[250];
};

// Globale Datenpakete
extern DataPackage receivedData;
extern SendPackage sendData;
extern JsonPackage jsonPackage;

// Funktionen
void initEspNow();
void sendEspNowData(byte lane, unsigned long time);
void broadcastEspNowData(byte lane, unsigned long time);
void processPendingBroadcast();

// Callback Funktionen (Signaturen unterscheiden sich zwischen ESP8266 und ESP32)
#ifdef ESP32
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecvFromSatellite(const uint8_t *mac_addr, const uint8_t *incomingData, int len);
#else
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);
void OnDataRecvFromSatellite(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len);
#endif

// Hilfsfunktionen
bool compareMacAddress(uint8_t *left, uint8_t *right);

#endif // ESPNOW_COMM_H

