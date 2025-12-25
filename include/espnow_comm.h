
#define ESPNOW_COMM_H

#include <Arduino.h>
#include <espnow.h>

// Max size of this struct is 250 bytes - ESP NOW buffer limit
struct DataPackage {
    int lane_nr;
    unsigned long time_ms;
};

struct SendPackage {
    int lane_nr;
    unsigned long time_ms;
};

// Globale Datenpakete
extern DataPackage receivedData;
extern SendPackage sendData;

// Funktionen
void initEspNow();
void sendEspNowData(byte lane, unsigned long time);

// Callback Funktionen
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);
void OnDataRecvFromSatellite(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len);

// Hilfsfunktionen
bool compareMacAddress(uint8_t *left, uint8_t *right);

#endif // ESPNOW_COMM_H

