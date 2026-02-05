#include "lane_data.h"
#include "espnow_comm.h"

// Globale Instanzen
LaneData lane1;
LaneData lane2;
RaceStatus raceStatus;

//////////////////////////////////////////////////////////////////////////////////////////
// LaneData Implementation
//////////////////////////////////////////////////////////////////////////////////////////

void LaneData::reset() {
    for (int k = 0; k < MAX_LAPS; k++) {
        laps[k] = 0;
    }
    for (int k = 0; k < MAX_LAP_PARTS; k++) {
        lapParts[k] = 0;
    }
    totalTime = 0;
    lapCount = 0;
    fastestLap = 0;
    averageLap = 0;
    previousTime = 0;
}

void LaneData::recordLap(unsigned long currentTime) {
    unsigned long lapTime = currentTime - previousTime;

    if (previousTime == 0) {
        previousTime = currentTime;
        return;
    }
    if (lapTime < MIN_RUNDEN_ZEIT) return;

    previousTime = currentTime;

    // Rundenzeiten verschieben
    for (int k = MAX_LAPS - 1; k >= 0; k--) {
        laps[k + 1] = laps[k];
    }
    laps[0] = lapTime;

    // Lap-Parts zurücksetzen
    for (int k = 0; k < MAX_LAP_PARTS; k++) {
        lapParts[k] = 0;
    }

    totalTime += lapTime;
    lapCount++;
    averageLap = totalTime / lapCount;

    if (fastestLap == 0 || fastestLap > lapTime) {
        fastestLap = lapTime;
    }

    if ((unsigned long)lapCount == raceStatus.targetLaps) {
        raceStatus.paused = true;
    }
}

void LaneData::recordLapPart(unsigned long time) {
    // Zwischenzeiten verschieben
    for (int k = MAX_LAP_PARTS - 1; k > 0; k--) {
        lapParts[k] = lapParts[k - 1];
    }
    lapParts[0] = time;
}

//////////////////////////////////////////////////////////////////////////////////////////
// RaceStatus Implementation
//////////////////////////////////////////////////////////////////////////////////////////

void RaceStatus::reset() {
    paused = true;
    targetLaps = 0;
    targetTime = 0;
    startTime = 0;
    runTime = 0;
}

void RaceStatus::togglePlayPause() {
    if (!paused) {
        paused = true;
    } else {
        startTime = millis();
        paused = false;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// Hilfsfunktionen
//////////////////////////////////////////////////////////////////////////////////////////

String getTimeString(unsigned long millis_) {
    unsigned long allSeconds = millis_ / 1000;
    int runHours = allSeconds / 3600;
    int secsRemaining = allSeconds % 3600;
    int runMinutes = secsRemaining / 60;
    int runSeconds = secsRemaining % 60;

    char buf[30];
    sprintf(buf, "%02d:%02d:%02ds", runHours, runMinutes, runSeconds);
    return buf;
}

String getSecString(unsigned long millis_) {
    unsigned long allSeconds = millis_ / 1000;
    int milliSecs = millis_ % 1000;

    char buf[30];
    sprintf(buf, "%lu.%03ds", allSeconds, milliSecs);
    return buf;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Lane Time Offset
//////////////////////////////////////////////////////////////////////////////////////////

void setLaneTimeOffset(int lane, unsigned long offset) {
    if (lane == 1) {
        lane1.previousTime = offset;
    } else if (lane == 2) {
        lane2.previousTime = offset;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// Interrupt Service Routines
//////////////////////////////////////////////////////////////////////////////////////////

// Entprellung: Zeitpunkt des letzten gültigen Impulses pro Lane
static volatile unsigned long lastTriggerLane1 = 0;
static volatile unsigned long lastTriggerLane2 = 0;
static const unsigned long DEBOUNCE_TIME_MS = 1000;  // 1 Sekunde Entprellzeit

IRAM_ATTR void ZeitMessung_1() {
    if (raceStatus.paused) return;

    unsigned long currentTime = millis();

    // Entprellung: Ignoriere Impulse innerhalb von 1 Sekunde
    if (currentTime - lastTriggerLane1 < DEBOUNCE_TIME_MS) return;
    lastTriggerLane1 = currentTime;

    lane1.recordLap(currentTime);

    // Impuls per ESP-NOW an andere Controller broadcasten - sende die Rundenzeit
    if (lane1.laps[0] > 0) {  // Nur senden wenn eine Rundenzeit vorhanden ist
        broadcastEspNowData(1, lane1.laps[0]);
    }
}

IRAM_ATTR void ZeitMessung_2() {
    if (raceStatus.paused) return;

    unsigned long currentTime = millis();

    // Entprellung: Ignoriere Impulse innerhalb von 1 Sekunde
    if (currentTime - lastTriggerLane2 < DEBOUNCE_TIME_MS) return;
    lastTriggerLane2 = currentTime;

    lane2.recordLap(currentTime);

    // Impuls per ESP-NOW an andere Controller broadcasten - sende die Rundenzeit
    if (lane2.laps[0] > 0) {  // Nur senden wenn eine Rundenzeit vorhanden ist
        broadcastEspNowData(2, lane2.laps[0]);
    }
}
