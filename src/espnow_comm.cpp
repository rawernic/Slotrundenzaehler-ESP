#include "espnow_comm.h"
#include "lane_data.h"
#include <ESP8266WiFi.h>

// Globale Datenpakete
DataPackage receivedData;
SendPackage sendData;

//////////////////////////////////////////////////////////////////////////////////////////

bool compareMacAddress(uint8_t *left, uint8_t *right) {
    for (int i = 0; i < 6; i++) {
        if (left[i] != right[i]) return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////

void sendEspNowData(byte lane, unsigned long time) {
    sendData.lane_nr = lane;
    sendData.time_ms = time;

    int result = esp_now_send(sensorAddress1, (uint8_t*)&sendData, sizeof(SendPackage));
    result = esp_now_send(sensorAddress2, (uint8_t*)&sendData, sizeof(SendPackage));

    if (result == 0) {
        Serial.println("Sent with success");
    } else {
        Serial.println("Error sending the data");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
    Serial.print("Last Packet Send Status: ");
    if (sendStatus == 0) {
        Serial.println("Delivery success");
    } else {
        Serial.println("Delivery fail");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

void OnDataRecvFromSatellite(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len) {
    memcpy(&receivedData, incomingData, sizeof(receivedData));
    memcpy(&receivedFrom, mac_addr, sizeof(uint8_t) * 6);

#ifdef _DEBUG_
    char messageStr[100];
    Serial.print("Packet received from: ");
    snprintf(messageStr, sizeof(messageStr), "%02x:%02x:%02x:%02x:%02x:%02x Lane: %d Time: %lu ms",
             mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5],
             receivedData.lane_nr, receivedData.time_ms);
    Serial.println(messageStr);
#endif

    // Zähler macht PAUSE
    if (raceStatus.paused) return;
    if (receivedData.lane_nr == 99) return;

    if (receivedData.lane_nr == 1) {
        lane1.recordLapPart(receivedData.time_ms);
    }

    if (receivedData.lane_nr == 2) {
        lane2.recordLapPart(receivedData.time_ms);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

void initEspNow() {
    Serial.println("Initializing ESP-NOW");

    if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Set ESP-NOW Role
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

    // Register peer SENSOR 1
    if (esp_now_add_peer(sensorAddress1, ESP_NOW_ROLE_COMBO, 1, NULL, 0) != 0) {
        Serial.println("Failed to add peer SENSOR 1");
        return;
    }

    // Register peer SENSOR 2
    if (esp_now_add_peer(sensorAddress2, ESP_NOW_ROLE_COMBO, 1, NULL, 0) != 0) {
        Serial.println("Failed to add peer SENSOR 2");
        return;
    }

    // Register receive callback
    esp_now_register_recv_cb(OnDataRecvFromSatellite);
    Serial.println("ESP-NOW initialized successfully");
}
