/**
 * SlotRundenzaehler ESP8266
 *
 * Modulare Architektur:
 * - config.h/cpp      : Konfiguration und Konstanten
 * - lane_data.h/cpp   : Bahndaten und Zeitmessung
 * - espnow_comm.h/cpp : ESP-NOW Kommunikation
 * - web_handlers.h/cpp: Webserver und REST-API
 * - html_content.h/cpp: HTML/CSS/JS für Webinterface
 */

#include <Arduino.h>
#include "config.h"
#include "lane_data.h"
#include "espnow_comm.h"
#include "web_handlers.h"

//////////////////////////////////////////////////////////////////////////////////////////

void setup() {
    Serial.begin(115200);
    Serial.println("\n\n=== SlotRundenzaehler ESP8266 ===\n");

    // LED initialisieren
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    // WiFi verbinden
    connectWiFi();

    // ESP-NOW aktivieren für Broadcast-Kommunikation
    initEspNow();

    // Webserver starten
    initWebServer();

    // Lichtschranken-Sensoren konfigurieren (D1 und D2)
    pinMode(SENSOR_1_PIN, INPUT_PULLUP);
    pinMode(SENSOR_2_PIN, INPUT_PULLUP);

    // Interrupts für Zeitmessung registrieren
    attachInterrupt(digitalPinToInterrupt(SENSOR_1_PIN), ZeitMessung_1, RISING);
    attachInterrupt(digitalPinToInterrupt(SENSOR_2_PIN), ZeitMessung_2, RISING);

    // Fahrernamen initialisieren
    lane1.driverName = "Fahrer1";
    lane2.driverName = "Fahrer2";

    Serial.println("\nSetup abgeschlossen - Bereit zum Zaehlen!\n");
}

//////////////////////////////////////////////////////////////////////////////////////////

void loop() {
    // Pending ESP-NOW Broadcasts verarbeiten (3x mit 100ms Abstand)
    processPendingBroadcast();

    // mDNS update (nur für ESP8266 notwendig)
    updateMDNS();
}
