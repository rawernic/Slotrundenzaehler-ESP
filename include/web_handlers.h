#ifndef WEB_HANDLERS_H
#define WEB_HANDLERS_H

#include <ESPAsyncWebServer.h>

// Webserver Instanz
extern AsyncWebServer server;

// Initialisierung
void initWebServer();
void connectWiFi();
void startConfigPortal();
bool tryConnectWiFi(const char* ssid, const char* password, int timeout_s);

// Request Handler
void handleRoot(AsyncWebServerRequest *request);
void handleGetData(AsyncWebServerRequest *request);
void handleLed(AsyncWebServerRequest *request);
void handleResetLanes(AsyncWebServerRequest *request);
void handlePlayPause(AsyncWebServerRequest *request);
void handleZielRunden(AsyncWebServerRequest *request);
void handleZielMinuten(AsyncWebServerRequest *request);
void handleFahrer(AsyncWebServerRequest *request);
void handleSwaggerJson(AsyncWebServerRequest *request);
void handleSwaggerUI(AsyncWebServerRequest *request);
void handleWiFiConfig(AsyncWebServerRequest *request);
void handleWiFiSave(AsyncWebServerRequest *request);
void handleWiFiScan(AsyncWebServerRequest *request);
void handleWiFiStatus(AsyncWebServerRequest *request);

#endif // WEB_HANDLERS_H

