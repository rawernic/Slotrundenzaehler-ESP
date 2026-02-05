#include "config.h"

#ifdef ESP32
    #include <Preferences.h>
    Preferences preferences;
#else
    #include <EEPROM.h>
    #define EEPROM_SIZE 512
    #define SSID_ADDR 0
    #define PASS_ADDR 100
#endif

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
// WiFi Konfiguration (Fallback)

String savedSSID = "";
String savedPassword = "";

// WiFi-Konfiguration laden
bool loadWiFiConfig() {
#ifdef ESP32
    preferences.begin("wifi", true); // read-only
    savedSSID = preferences.getString("ssid", "");
    savedPassword = preferences.getString("password", "");
    preferences.end();
#else
    EEPROM.begin(EEPROM_SIZE);

    // SSID lesen
    char ssid[64] = {0};
    for (int i = 0; i < 63; i++) {
        ssid[i] = EEPROM.read(SSID_ADDR + i);
        if (ssid[i] == 0) break;
    }

    // Password lesen
    char pass[64] = {0};
    for (int i = 0; i < 63; i++) {
        pass[i] = EEPROM.read(PASS_ADDR + i);
        if (pass[i] == 0) break;
    }

    savedSSID = String(ssid);
    savedPassword = String(pass);
    EEPROM.end();
#endif

    return (savedSSID.length() > 0);
}

// WiFi-Konfiguration speichern
void saveWiFiConfig(String ssid, String password) {
#ifdef ESP32
    preferences.begin("wifi", false); // read-write
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.end();
#else
    EEPROM.begin(EEPROM_SIZE);

    // SSID schreiben
    for (int i = 0; i < ssid.length() && i < 63; i++) {
        EEPROM.write(SSID_ADDR + i, ssid[i]);
    }
    EEPROM.write(SSID_ADDR + ssid.length(), 0); // Null-Terminator

    // Password schreiben
    for (int i = 0; i < password.length() && i < 63; i++) {
        EEPROM.write(PASS_ADDR + i, password[i]);
    }
    EEPROM.write(PASS_ADDR + password.length(), 0); // Null-Terminator

    EEPROM.commit();
    EEPROM.end();
#endif

    savedSSID = ssid;
    savedPassword = password;

    Serial.println("WiFi-Konfiguration gespeichert:");
    Serial.print("SSID: ");
    Serial.println(ssid);
}

// WiFi-Konfiguration löschen
void clearWiFiConfig() {
#ifdef ESP32
    preferences.begin("wifi", false);
    preferences.clear();
    preferences.end();
#else
    EEPROM.begin(EEPROM_SIZE);
    for (int i = 0; i < 200; i++) {
        EEPROM.write(i, 0);
    }
    EEPROM.commit();
    EEPROM.end();
#endif

    savedSSID = "";
    savedPassword = "";
    Serial.println("WiFi-Konfiguration gelöscht");
}


