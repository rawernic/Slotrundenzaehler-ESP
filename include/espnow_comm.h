#ifndef ESPNOW_COMM_H
#define ESPNOW_COMM_H

#include <Arduino.h>
#include <espnow.h>
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

// Callback Funktionen
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);
void OnDataRecvFromSatellite(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len);

// Hilfsfunktionen
bool compareMacAddress(uint8_t *left, uint8_t *right);

#endif // ESPNOW_COMM_H

