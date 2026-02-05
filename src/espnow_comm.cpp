#include "espnow_comm.h"
#include "lane_data.h"

#ifdef ESP32
    #include <esp_wifi.h>
#endif

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

// Broadcast-Adresse
uint8_t broadcastAddressComm[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

//////////////////////////////////////////////////////////////////////////////////////////
// Callback Funktionen
//////////////////////////////////////////////////////////////////////////////////////////

#ifdef ESP32
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("ESP-NOW Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "OK" : "FEHLER");
}
#else
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
    Serial.print("ESP-NOW Send Status: ");
    Serial.println(sendStatus == 0 ? "OK" : "FEHLER");
}
#endif

#ifdef ESP32
void OnDataRecvFromSatellite(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
#else
void OnDataRecvFromSatellite(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len) {
#endif
    Serial.print("ESP-NOW Empfangen von: ");
    for (int i = 0; i < 6; i++) {
        Serial.printf("%02X", mac_addr[i]);
        if (i < 5) Serial.print(":");
    }
    Serial.println();

    // Versuche JSON zu parsen
    StaticJsonDocument<250> doc;
    DeserializationError error = deserializeJson(doc, incomingData, len);

    if (!error) {
        const char* sensor = doc["sensor"] | "";
        const char* messageType = doc["message_type"] | "";
        int lane = doc["lane"] | 0;

        Serial.print("JSON: sensor=");
        Serial.print(sensor);
        Serial.print(", message_type=");
        Serial.print(messageType);
        Serial.print(", lane=");
        Serial.println(lane);

        // Prüfe auf LapReset vom Master
        if (strcmp(sensor, "master") == 0 && strcmp(messageType, "LapReset") == 0) {
            if (lane == 1 || lane == 2) {
                setLaneTimeOffset(lane, millis());
                Serial.print(">>> LapReset für Lane ");
                Serial.println(lane);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// ESP-NOW Initialisierung
//////////////////////////////////////////////////////////////////////////////////////////

void initEspNow() {
    // WiFi sollte bereits verbunden sein - wir nutzen den gleichen Kanal
    int channel = WiFi.channel();
    Serial.print("ESP-NOW nutzt WiFi-Kanal: ");
    Serial.println(channel);

#ifdef ESP32
    // ESP32: Kanal wird automatisch vom WiFi übernommen
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW Init fehlgeschlagen!");
        return;
    }

    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecvFromSatellite);

    // Broadcast Peer hinzufügen
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, broadcastAddressComm, 6);
    peerInfo.channel = channel;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("ESP-NOW Peer hinzufügen fehlgeschlagen!");
    }
#else
    // ESP8266: Kanal wird automatisch vom WiFi übernommen
    if (esp_now_init() != 0) {
        Serial.println("ESP-NOW Init fehlgeschlagen!");
        return;
    }

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecvFromSatellite);

    // Broadcast Peer hinzufügen (Kanal wird automatisch vom WiFi-Kanal übernommen)
    if (esp_now_add_peer(broadcastAddressComm, ESP_NOW_ROLE_COMBO, channel, NULL, 0) != 0) {
        Serial.println("ESP-NOW Peer hinzufügen fehlgeschlagen!");
    }
#endif

    Serial.println("ESP-NOW initialisiert!");
}

//////////////////////////////////////////////////////////////////////////////////////////
// ESP-NOW Senden
//////////////////////////////////////////////////////////////////////////////////////////

void sendEspNowData(byte lane, unsigned long time) {
    sendData.lane_nr = lane;
    sendData.time_ms = time;
    strcpy(sendData.sensorType, "lane_sensor");

#ifdef ESP32
    esp_now_send(broadcastAddressComm, (uint8_t*)&sendData, sizeof(sendData));
#else
    esp_now_send(broadcastAddressComm, (uint8_t*)&sendData, sizeof(sendData));
#endif
}

void broadcastEspNowData(byte lane, unsigned long time) {
    messageIdCounter++;

    // time ist die Rundenzeit in Millisekunden
    unsigned long lapTime = time;

    // Formatierte Rundenzeit: 3.999s
    char lapTimeStr[16];
    unsigned long lapSecs = lapTime / 1000;
    int lapMillis = lapTime % 1000;
    snprintf(lapTimeStr, sizeof(lapTimeStr), "%lu.%03d", lapSecs, lapMillis);

    // Formatierte Zeitausgabe für Timestamp: 00:00:03.999
    unsigned long allSeconds = lapTime / 1000;
    int hours = allSeconds / 3600;
    int minutes = (allSeconds % 3600) / 60;
    int seconds = allSeconds % 60;
    int milliSecs = lapTime % 1000;

    char timestamp[16];
    snprintf(timestamp, sizeof(timestamp), "%02d:%02d:%02d.%03d", hours, minutes, seconds, milliSecs);

    // JSON-Dokument erstellen
    StaticJsonDocument<250> doc;
    doc["message_id"] = messageIdCounter;
    doc["message_type"] = "lap_time";
    doc["sensor"] = "lane_sensor";
    doc["lane"] = lane;
    doc["timestamp"] = timestamp;
    doc["time_ms"] = lapTime;
    doc["lapTime"] = lapTimeStr;
    doc["lapTime_ms"] = lapTime;

    // JSON serialisieren und in pending-Struktur speichern
    serializeJson(doc, pendingBroadcast.jsonData, sizeof(pendingBroadcast.jsonData));

    // Pending Broadcast aktivieren - wird 3x mit 100ms Abstand gesendet
    pendingBroadcast.active = true;
    pendingBroadcast.retryCount = 0;
    pendingBroadcast.nextSendTime = millis();

    Serial.print("ESP-NOW Broadcast queued: ");
    Serial.println(pendingBroadcast.jsonData);
}

void processPendingBroadcast() {
    if (!pendingBroadcast.active) return;

    if (millis() >= pendingBroadcast.nextSendTime) {
#ifdef ESP32
        esp_now_send(broadcastAddressComm, (uint8_t*)pendingBroadcast.jsonData,
                     strlen(pendingBroadcast.jsonData) + 1);
#else
        esp_now_send(broadcastAddressComm, (uint8_t*)pendingBroadcast.jsonData,
                     strlen(pendingBroadcast.jsonData) + 1);
#endif

        pendingBroadcast.retryCount++;

        Serial.print("ESP-NOW Broadcast (");
        Serial.print(pendingBroadcast.retryCount);
        Serial.print("/3): ");
        Serial.println(pendingBroadcast.jsonData);

        if (pendingBroadcast.retryCount >= 3) {
            pendingBroadcast.active = false;
        } else {
            pendingBroadcast.nextSendTime = millis() + 100;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// Hilfsfunktionen
//////////////////////////////////////////////////////////////////////////////////////////

bool compareMacAddress(uint8_t *left, uint8_t *right) {
    for (int i = 0; i < 6; i++) {
        if (left[i] != right[i]) return false;
    }
    return true;
}
