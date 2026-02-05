#ifndef LANE_DATA_H
#define LANE_DATA_H

#include <Arduino.h>
#include "config.h"

// Forward declaration
struct RaceStatus;

// Datenstruktur für eine Fahrbahn
struct LaneData {
    volatile unsigned long laps[MAX_LAPS] = {0};
    volatile unsigned long lapParts[MAX_LAP_PARTS] = {0};
    volatile unsigned long fastestLap = 0;
    volatile unsigned long averageLap = 0;
    volatile unsigned long totalTime = 0;
    volatile int lapCount = 0;
    unsigned long previousTime = 0;
    String driverName = "Fahrer";

    void reset();
    void recordLap(unsigned long currentTime);
    void recordLapPart(unsigned long time);
};

// Rennstatus
struct RaceStatus {
    volatile bool paused = true;
    unsigned long startTime = 0;
    unsigned long runTime = 0;
    unsigned long targetTime = 0;
    unsigned long targetLaps = 0;

    void reset();
    void togglePlayPause();
    bool isRunning() const { return !paused; }
};

// Globale Instanzen
extern LaneData lane1;
extern LaneData lane2;
extern RaceStatus raceStatus;

// Hilfsfunktionen
String getTimeString(unsigned long millis_);
String getSecString(unsigned long millis_);

// Lane Time Offset setzen (für Synchronisation über ESP-NOW)
void setLaneTimeOffset(int lane, unsigned long offset);

// Interrupt Service Routines
void IRAM_ATTR ZeitMessung_1();
void IRAM_ATTR ZeitMessung_2();

#endif // LANE_DATA_H
