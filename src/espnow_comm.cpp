#include "espnow_comm.h"
#include "config.h"
#include "lane_data.h"

// Globale Datenpakete
DataPackage receivedData;
SendPackage sendData;
JsonPackage jsonPackage;

// Broadcast Queue
struct BroadcastQueue {
    byte lane;
    unsigned long time;
    bool pending;
    int attempts;
    unsigned long lastAttempt;
} broadcastQueue;

//////////////////////////////////////////////////////////////////////////////////////////

void initEspNow() {
    Serial.println("Initialisiere ESP-NOW...");

#ifdef ESP32
    // ESP32 ESP-NOW Init
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW Init fehlgeschlagen!");
        return;
    }

    // Register callbacks
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecvFromSatellite);

    // Peer hinzufügen (Broadcast)
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Fehler beim Hinzufügen des Broadcast-Peers");
        return;
    }

#else
    // ESP8266 ESP-NOW Init
    if (esp_now_init() != 0) {
        Serial.println("ESP-NOW Init fehlgeschlagen!");
        return;
    }

    // Set role
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

    // Register callbacks
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecvFromSatellite);

    // Peer hinzufügen (Broadcast)
    esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
#endif

    Serial.println("ESP-NOW erfolgreich initialisiert");

    // Broadcast Queue initialisieren
    broadcastQueue.pending = false;
    broadcastQueue.attempts = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////

void sendEspNowData(byte lane, unsigned long time) {
    strcpy(sendData.sensorType, "master");
    sendData.lane_nr = lane;
    sendData.time_ms = time;

#ifdef ESP32
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&sendData, sizeof(sendData));
#else
    int result = esp_now_send(broadcastAddress, (uint8_t *)&sendData, sizeof(sendData));
#endif

    if (result == 0) {
        Serial.printf("ESP-NOW Daten gesendet: Lane %d, Zeit %lu ms\n", lane, time);
    } else {
        Serial.printf("ESP-NOW Senden fehlgeschlagen: %d\n", result);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

void broadcastEspNowData(byte lane, unsigned long time) {
    // In Queue einreihen für 3-faches Senden
    broadcastQueue.lane = lane;
    broadcastQueue.time = time;
    broadcastQueue.pending = true;
    broadcastQueue.attempts = 0;
    broadcastQueue.lastAttempt = 0;

    Serial.printf("Broadcast in Queue: Lane %d, Zeit %lu ms\n", lane, time);
}

//////////////////////////////////////////////////////////////////////////////////////////

void processPendingBroadcast() {
    if (!broadcastQueue.pending) return;

    unsigned long now = millis();

    // Warte 100ms zwischen Versuchen
    if (broadcastQueue.attempts > 0 && (now - broadcastQueue.lastAttempt) < 100) {
        return;
    }

    if (broadcastQueue.attempts < 3) {
        sendEspNowData(broadcastQueue.lane, broadcastQueue.time);
        broadcastQueue.attempts++;
        broadcastQueue.lastAttempt = now;

        Serial.printf("Broadcast Versuch %d/3\n", broadcastQueue.attempts);
    } else {
        // Nach 3 Versuchen ist der Broadcast abgeschlossen
        broadcastQueue.pending = false;
        Serial.println("Broadcast abgeschlossen");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// Callback Funktionen
//////////////////////////////////////////////////////////////////////////////////////////

#ifdef ESP32
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("ESP-NOW Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void OnDataRecvFromSatellite(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
    memcpy(&receivedData, incomingData, sizeof(receivedData));
    memcpy(receivedFrom, mac_addr, 6);

    Serial.printf("ESP-NOW Daten empfangen von Satellite: Lane %d, Zeit %lu ms\n",
                  receivedData.lane_nr, receivedData.time_ms);

    // Daten an entsprechende Lane weiterleiten
    if (receivedData.lane_nr == 1) {
        lane1.recordLapPart(receivedData.time_ms);
    } else if (receivedData.lane_nr == 2) {
        lane2.recordLapPart(receivedData.time_ms);
    }
}

#else
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
    Serial.print("ESP-NOW Send Status: ");
    Serial.println(sendStatus == 0 ? "Success" : "Fail");
}

void OnDataRecvFromSatellite(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len) {
    memcpy(&receivedData, incomingData, sizeof(receivedData));
    memcpy(receivedFrom, mac_addr, 6);

    Serial.printf("ESP-NOW Daten empfangen von Satellite: Lane %d, Zeit %lu ms\n",
                  receivedData.lane_nr, receivedData.time_ms);

    // Daten an entsprechende Lane weiterleiten
    if (receivedData.lane_nr == 1) {
        lane1.recordLapPart(receivedData.time_ms);
    } else if (receivedData.lane_nr == 2) {
        lane2.recordLapPart(receivedData.time_ms);
    }
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////
// Hilfsfunktionen
//////////////////////////////////////////////////////////////////////////////////////////

bool compareMacAddress(uint8_t *left, uint8_t *right) {
    for (int i = 0; i < 6; i++) {
        if (left[i] != right[i]) return false;
    }
    return true;
}
