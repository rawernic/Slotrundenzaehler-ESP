#include "web_handlers.h"
#include "html_content.h"
#include "lane_data.h"
#include <ArduinoJson.h>

#ifdef ESP32
    #include <WiFi.h>
    #include <AsyncTCP.h>
    #include <ESPmDNS.h>
#else
    #include <ESP8266WiFi.h>
    #include <ESPAsyncTCP.h>
    #include <ESP8266mDNS.h>
#endif

// Webserver Instanz
AsyncWebServer server(80);

// JSON Buffer
static StaticJsonDocument<2000> jsonDocument;
static char buffer[2000];

//////////////////////////////////////////////////////////////////////////////////////////

void connectWiFi() {
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Waiting to connect...");
    }

    Serial.println("\nWiFi connected\n");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Wi-Fi Channel: ");
    Serial.println(WiFi.channel());
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());

    // mDNS starten - Webseite erreichbar unter http://rundenzaehler.local
    if (MDNS.begin("rundenzaehler")) {
        Serial.println("mDNS gestartet: http://rundenzaehler.local");
        MDNS.addService("http", "tcp", 80);
    } else {
        Serial.println("Fehler beim Starten von mDNS");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

void updateMDNS() {
#ifndef ESP32
    // mDNS update nur für ESP8266 notwendig
    MDNS.update();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////

void handleRoot(AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", MAIN_PAGE);
}

//////////////////////////////////////////////////////////////////////////////////////////

void handleResetLanes(AsyncWebServerRequest *request) {
    lane1.reset();
    lane2.reset();
    raceStatus.reset();

    request->send(200, "text/plane", "play_circle");
}

//////////////////////////////////////////////////////////////////////////////////////////

void handleZielRunden(AsyncWebServerRequest *request) {
    Serial.println("Get handleZielRunden");

    if (raceStatus.targetTime > 0) {
        request->send(400, "text/plane", "laeuft");
        return;
    }
    if (raceStatus.targetLaps > 0) {
        request->send(400, "text/plane", "laeuft");
        return;
    }

    raceStatus.targetLaps = 20;
    raceStatus.targetTime = 0;
    raceStatus.startTime = 0;
    raceStatus.runTime = 0;

    request->send(200, "text/plane", "play_circle");
}

//////////////////////////////////////////////////////////////////////////////////////////

void handleZielMinuten(AsyncWebServerRequest *request) {
    Serial.println("Get handleZielMinuten");

    if (raceStatus.targetTime > 0) {
        request->send(400, "text/plane", "laeuft");
        return;
    }
    if (raceStatus.targetLaps > 0) {
        request->send(400, "text/plane", "laeuft");
        return;
    }

    raceStatus.targetLaps = 0;
    raceStatus.targetTime = 5 * 60 * 1000; // 5 Minuten
    raceStatus.startTime = 0;
    raceStatus.runTime = 0;

    request->send(200, "text/plane", "play_circle");
}

//////////////////////////////////////////////////////////////////////////////////////////

void handlePlayPause(AsyncWebServerRequest *request) {
    Serial.println("Get handlePlayPause");

    if (!raceStatus.paused) {
        raceStatus.paused = true;
        request->send(200, "text/plane", "play_circle");
    } else {
        raceStatus.startTime = millis();
        raceStatus.paused = false;
        request->send(200, "text/plane", "pause_circle");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

void handleGetData(AsyncWebServerRequest *request) {
    Serial.println("Get data");
    jsonDocument.clear();

    String message = getTimeString(raceStatus.runTime);
    String modus = "Training";

    if (!raceStatus.paused) {
        raceStatus.runTime += (millis() - raceStatus.startTime);
        raceStatus.startTime = millis();
        message = getTimeString(raceStatus.runTime);
    }

    // Zeitrennen-Logik
    if (raceStatus.targetTime > 0) {
        message = getTimeString(raceStatus.runTime);
        modus = getTimeString(raceStatus.targetTime);

        if (raceStatus.runTime >= raceStatus.targetTime) {
            raceStatus.paused = true;
            raceStatus.raceFinished = true;
            if (lane1.lapCount > lane2.lapCount) {
                message = "((: the winner is " + lane1.driverName + " :))";
            } else {
                message = "((: the winner is " + lane2.driverName + " :))";
            }
            Serial.println("Zielzeit erreicht!");
        }
    }

    // Rundenrennen-Logik
    if (raceStatus.targetLaps > 0) {
        message = getTimeString(raceStatus.runTime);
        modus = String(raceStatus.targetLaps) + " Runden";

        // Prüfe ob eine Lane die Zielrunden erreicht hat (Gewinner)
        if ((unsigned long)lane1.lapCount >= raceStatus.targetLaps) {
            raceStatus.paused = true;
            raceStatus.raceFinished = true;
            message = "((: the winner is " + lane1.driverName + " :))";
            Serial.println("Lane 1 hat die Zielrunden erreicht!");
        }
        else if ((unsigned long)lane2.lapCount >= raceStatus.targetLaps) {
            raceStatus.paused = true;
            raceStatus.raceFinished = true;
            message = "((: the winner is " + lane2.driverName + " :))";
            Serial.println("Lane 2 hat die Zielrunden erreicht!");
        }
    }

    // Lane 1 Daten
    JsonObject jsonLane1 = jsonDocument.createNestedObject("Lane_1");
    jsonLane1["modus"] = modus;
    jsonLane1["message"] = message;
    jsonLane1["raceFinished"] = raceStatus.raceFinished;
    jsonLane1["newFastestLap"] = lane1.newFastestLap;
    jsonLane1["fahrer"] = lane1.driverName;
    jsonLane1["lap_count"] = lane1.lapCount;
    jsonLane1["total_time"] = getTimeString(lane1.totalTime);
    jsonLane1["fast_lap"] = getSecString(lane1.fastestLap);
    jsonLane1["last_lap"] = getSecString(lane1.laps[0]);
    jsonLane1["average_lap"] = getSecString(lane1.averageLap);

    jsonLane1["lap_part_0"] = getSecString(lane1.lapParts[0]);
    jsonLane1["lap_part_1"] = getSecString(lane1.lapParts[1]);
    jsonLane1["lap_part_2"] = getSecString(lane1.lapParts[2]);

    for (int i = 0; i < 15; i++) {
        jsonLane1["lap" + String(i)] = getSecString(lane1.laps[i]);
    }

    // Lane 2 Daten
    JsonObject jsonLane2 = jsonDocument.createNestedObject("Lane_2");
    jsonLane2["newFastestLap"] = lane2.newFastestLap;
    jsonLane2["fahrer"] = lane2.driverName;
    jsonLane2["lap_count"] = lane2.lapCount;
    jsonLane2["total_time"] = getTimeString(lane2.totalTime);
    jsonLane2["fast_lap"] = getSecString(lane2.fastestLap);
    jsonLane2["last_lap"] = getSecString(lane2.laps[0]);
    jsonLane2["average_lap"] = getSecString(lane2.averageLap);

    jsonLane2["lap_part_0"] = getSecString(lane2.lapParts[0]);
    jsonLane2["lap_part_1"] = getSecString(lane2.lapParts[1]);
    jsonLane2["lap_part_2"] = getSecString(lane2.lapParts[2]);

    for (int i = 0; i < 15; i++) {
        jsonLane2["lap" + String(i)] = getSecString(lane2.laps[i]);
    }

    serializeJsonPretty(jsonDocument, buffer);
    request->send(200, "application/json", buffer);

    // Flags zurücksetzen nach dem Senden
    lane1.newFastestLap = false;
    lane2.newFastestLap = false;
}

//////////////////////////////////////////////////////////////////////////////////////////

void handleLed(AsyncWebServerRequest *request) {
    Serial.println("handleLed");
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    request->send(200, "text/html", "LED toggled");
}

//////////////////////////////////////////////////////////////////////////////////////////

void handleFahrer(AsyncWebServerRequest *request) {
    Serial.println("handleFahrer");
    Serial.println(request->arg("fahrer1"));
    Serial.println(request->arg("fahrer2"));

    lane1.driverName = request->arg("fahrer1");
    lane2.driverName = request->arg("fahrer2");

    jsonDocument.clear();
    JsonObject fahrer = jsonDocument.createNestedObject("Fahrer");
    fahrer["name1"] = lane1.driverName;
    fahrer["name2"] = lane2.driverName;

    serializeJsonPretty(jsonDocument, buffer);
    request->send(200, "application/json", buffer);
}

//////////////////////////////////////////////////////////////////////////////////////////

void handleSwaggerJson(AsyncWebServerRequest *request) {
    request->send(200, "application/json", SWAGGER_JSON);
}

//////////////////////////////////////////////////////////////////////////////////////////

void handleSwaggerUI(AsyncWebServerRequest *request) {
    request->send(200, "text/html", SWAGGER_UI);
}

//////////////////////////////////////////////////////////////////////////////////////////

void handleFavicon(AsyncWebServerRequest *request) {
    request->send_P(200, "image/svg+xml", FAVICON_SVG);
}

//////////////////////////////////////////////////////////////////////////////////////////

void initWebServer() {
    server.on("/", handleRoot);
    server.on("/favicon.svg", handleFavicon);
    server.on("/favicon.ico", handleFavicon);  // Fallback für Browser, die .ico erwarten
    server.on("/data", handleGetData);
    server.on("/led", handleLed);
    server.on("/reset", handleResetLanes);
    server.on("/playpause", handlePlayPause);
    server.on("/zielrunden", handleZielRunden);
    server.on("/zielminuten", handleZielMinuten);
    server.on("/fahrer", handleFahrer);
    server.on("/swagger.json", handleSwaggerJson);
    server.on("/swaggerUI", handleSwaggerUI);

    server.begin();
    Serial.println("Server listening");
}
