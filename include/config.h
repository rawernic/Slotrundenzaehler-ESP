#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Debug-Modus
#define _DEBUG_

// WiFi Konfiguration
extern const char* WIFI_SSID;
extern const char* WIFI_PASSWORD;

// MAC Adressen für ESP-NOW
extern uint8_t broadcastAddress[];
extern uint8_t masterAddress1[];
extern uint8_t sensorAddress1[];
extern uint8_t sensorAddress2[];
extern uint8_t receivedFrom[];

// Konstanten
#define MAX_LAPS 20
#define MAX_LAP_PARTS 5
#define MIN_RUNDEN_ZEIT 1000  // Minimale Rundenzeit in ms

// GPIO Pins
#define LED_PIN LED_BUILTIN
#define SENSOR_1_PIN D1
#define SENSOR_2_PIN D2

#endif // CONFIG_H
