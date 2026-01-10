#include "espnow_comm.h"
#include "lane_data.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

// Globale Datenpakete
DataPackage receivedData;
SendPackage sendData;
JsonPackage jsonPackage;

// Message-ID Counter
static uint32_t messageIdCounter = 0;

// Retry-Struktur für verzögertes Senden
struct PendingBroadcast {
    bool active = false;
    uint8_t retryCount = 0;
    unsigned long nextSendTime = 0;
    char jsonData[250];
};

static PendingBroadcast pendingBroadcast;

//////////////////////////////////////////////////////////////////////////////////////////

void processPendingBroadcast() {
    if (!pendingBroadcast.active) return;

    if (millis() >= pendingBroadcast.nextSendTime) {
        esp_now_send(broadcastAddress, (uint8_t*)pendingBroadcast.jsonData,
                     strlen(pendingBroadcast.jsonData) + 1);

        pendingBroadcast.retryCount++;

        Serial.print("ESP-NOW Broadcast (");
        Serial.print(pendingBroadcast.retryCount);
        Serial.print("/3): ");
        Serial.println(pendingBroadcast.jsonData);

        if (pendingBroadcast.retryCount >= 3) {
            pendingBroadcast.active = false;
        } else {
            pendingBroadcast.nextSendTime = millis() + 100; // 100ms = 1/10 Sekunde
        }
    }
}

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

void broadcastEspNowData(byte lane, unsigned long time) {
    // Message-ID erhöhen
    messageIdCounter++;

    // Formatierte Zeitausgabe: 00:00:03.999
    unsigned long allSeconds = time / 1000;
    int hours = allSeconds / 3600;
    int minutes = (allSeconds % 3600) / 60;
    int seconds = allSeconds % 60;
    int milliSecs = time % 1000;

    char timestamp[16];
    snprintf(timestamp, sizeof(timestamp), "%02d:%02d:%02d.%03d", hours, minutes, seconds, milliSecs);

    // Rundenzeit aus der entsprechenden Lane holen
    unsigned long lapTime = 0;
    if (lane == 1) {
        lapTime = lane1.laps[0];  // Letzte Rundenzeit
    } else if (lane == 2) {
        lapTime = lane2.laps[0];  // Letzte Rundenzeit
    }

    // Formatierte Rundenzeit: 3.999s
    char lapTimeStr[16];
    unsigned long lapSecs = lapTime / 1000;
    int lapMillis = lapTime % 1000;
    snprintf(lapTimeStr, sizeof(lapTimeStr), "%lu.%03d", lapSecs, lapMillis);

    // JSON-Dokument erstellen
    StaticJsonDocument<250> doc;
    doc["message_id"] = messageIdCounter;
    doc["sensor"] = "master";
    doc["lane"] = lane;
    doc["timestamp"] = timestamp;
    doc["time_ms"] = time;
    doc["lapTime"] = lapTimeStr;
    doc["lapTime_ms"] = lapTime;

    // JSON serialisieren und in pending-Struktur speichern
    serializeJson(doc, pendingBroadcast.jsonData, sizeof(pendingBroadcast.jsonData));

    // Pending Broadcast aktivieren - wird 3x mit 100ms Abstand gesendet
    pendingBroadcast.active = true;
    pendingBroadcast.retryCount = 0;
    pendingBroadcast.nextSendTime = millis(); // Sofort erstes Senden

    Serial.print("ESP-NOW Broadcast queued: ");
    Serial.println(pendingBroadcast.jsonData);
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

    // Register Broadcast Peer (für alle Geräte)
    if (esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0) != 0) {
        Serial.println("Failed to add broadcast peer");
        return;
    }

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
