#include "lane_data.h"

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
// Interrupt Service Routines
//////////////////////////////////////////////////////////////////////////////////////////

IRAM_ATTR void ZeitMessung_1() {
    if (raceStatus.paused) return;

    unsigned long currentTime = millis();
    lane1.recordLap(currentTime);
}

IRAM_ATTR void ZeitMessung_2() {
    if (raceStatus.paused) return;

    unsigned long currentTime = millis();
    lane2.recordLap(currentTime);
}
