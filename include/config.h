#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Debug-Modus
#define _DEBUG_

// ESP-NOW Konfiguration
#define ESP_WIFI_CHANNEL 1  // WiFi-Kanal für ESP-NOW

// WiFi Konfiguration
extern const char* WIFI_SSID;
extern const char* WIFI_PASSWORD;

// MAC Adressen für ESP-NOW
extern uint8_t masterAddress1[];
extern uint8_t sensorAddress1[];
extern uint8_t sensorAddress2[];
extern uint8_t receivedFrom[];

// Konstanten
#define MAX_LAPS 20
#define MAX_LAP_PARTS 5
#define MIN_RUNDEN_ZEIT 1000  // Minimale Rundenzeit in ms

// GPIO Pins (boardabhängig)
#if defined(ESP32)
    #if defined(CONFIG_IDF_TARGET_ESP32C3) || defined(ARDUINO_ESP32C3_DEV)
        // ESP32-C3 Supermini (GPIO 18/19 = USB, GPIO 20/21 = UART)
        #define LED_PIN 8           // ESP32-C3 eingebaute LED (GPIO8)
        #define SENSOR_1_PIN 5      // GPIO5
        #define SENSOR_2_PIN 6      // GPIO6
    #else
        // ESP32 Standard (D1 Mini etc.)
        #define LED_PIN 2           // ESP32 eingebaute LED
        #define SENSOR_1_PIN 16     // GPIO16
        #define SENSOR_2_PIN 17     // GPIO17
    #endif
#else
    // ESP8266
    #define LED_PIN LED_BUILTIN
    #define SENSOR_1_PIN D1     // GPIO5
    #define SENSOR_2_PIN D2     // GPIO4
#endif

#endif // CONFIG_H
