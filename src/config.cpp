#include "config.h"
uint8_t receivedFrom[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};     // received from
uint8_t sensorAddress2[] = {0x4A, 0x55, 0x19, 0x12, 0xFF, 0x86};   // sensor 2
uint8_t sensorAddress1[] = {0x4E, 0x75, 0x25, 0x1A, 0x2A, 0x0A};   // sensor 1
uint8_t masterAddress1[] = {0x48, 0x55, 0x19, 0x12, 0xFF, 0x86};   // master
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // alle Sensoren
// Satellit 1: 4C:75:25:1A:2A:0A
// Rundenzaehler: 48:55:19:12:19:EB
// MAC Adressen für ESP-NOW

const char* WIFI_PASSWORD = "MGQzODI3YTBmODlj";
const char* WIFI_SSID = "DW-Network II 2,4GHZ";
// WiFi Konfiguration


