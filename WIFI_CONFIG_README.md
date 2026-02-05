# WiFi Konfigurations-Manager

## Übersicht

Der Rundenzähler ESP verfügt über ein integriertes WiFi-Konfigurationssystem, das es ermöglicht, die WiFi-Zugangsdaten zur Laufzeit über ein Web-Interface zu konfigurieren - direkt von einem iPad, iPhone oder Android-Gerät.

## Features

✅ **Automatische WiFi-Verbindung**: Gespeicherte Credentials werden beim Start automatisch geladen  
✅ **Captive Portal**: Bei fehlender Verbindung startet automatisch ein Konfigurations-Portal  
✅ **Netzwerk-Scanner**: Zeigt verfügbare WiFi-Netzwerke mit Signalstärke an  
✅ **Persistenter Speicher**: WiFi-Daten werden dauerhaft gespeichert (ESP32: Preferences, ESP8266: EEPROM)  
✅ **Fallback-Modus**: Bei Problemen wird auf die hart-kodierten Credentials zurückgegriffen  
✅ **Responsive Design**: Modernes, mobil-optimiertes Web-Interface  

## Erste Inbetriebnahme

### Fall 1: Keine gespeicherten WiFi-Daten

1. **ESP starten** - Der Rundenzähler bootet und sucht nach gespeicherten WiFi-Daten
2. **Kein WiFi gefunden** - Da keine Daten gespeichert sind, startet automatisch der AP-Modus
3. **Access Point erscheint**: 
   - SSID: `Rundenzaehler-Setup`
   - Passwort: `12345678`
   - IP: `192.168.4.1`

4. **Mit dem ESP verbinden**:
   - Öffne die WiFi-Einstellungen auf deinem iPad/iPhone/Android
   - Verbinde dich mit `Rundenzaehler-Setup` (Passwort: `12345678`)

5. **Browser öffnen**:
   - Gehe zu: `http://192.168.4.1/wifi/config`
   - Die WiFi-Konfigurationsseite lädt automatisch

6. **Netzwerk auswählen**:
   - Klicke auf "Netzwerke scannen"
   - Wähle dein WiFi-Netzwerk aus der Liste
   - Oder gib die SSID manuell ein

7. **Passwort eingeben**:
   - Gib das Passwort deines WiFi-Netzwerks ein
   - Klicke auf "Speichern & Verbinden"

8. **Automatischer Neustart**:
   - Bei erfolgreicher Verbindung startet der ESP neu
   - Beim nächsten Start verbindet er sich automatisch mit dem gespeicherten Netzwerk

### Fall 2: WiFi-Daten bereits gespeichert

1. **ESP starten** - Der Rundenzähler lädt die gespeicherten Credentials
2. **Automatische Verbindung** - ESP verbindet sich mit dem gespeicherten Netzwerk
3. **Web-Interface verfügbar**:
   - Via IP-Adresse (siehe Serial Monitor)
   - Via mDNS: `http://rundenzaehler.local`

## Verwendung

### WiFi-Konfiguration ändern

Auch wenn bereits ein WiFi konfiguriert ist, kannst du die Einstellungen jederzeit ändern:

1. **Verbinde dich mit dem Rundenzähler** (entweder über dein normales WiFi oder den AP-Modus)
2. **Öffne die Konfigurationsseite**:
   - `http://rundenzaehler.local/wifi/config` (wenn im normalen Netzwerk)
   - `http://192.168.4.1/wifi/config` (wenn im AP-Modus)
3. **Neue Credentials eingeben** und speichern

### API-Endpunkte

Der WiFi-Manager bietet folgende REST-API-Endpunkte:

#### `GET /wifi/config`
Zeigt die HTML-Konfigurationsseite an

#### `GET /wifi/scan`
Scannt nach verfügbaren WiFi-Netzwerken

**Response:**
```json
{
  "networks": [
    {
      "ssid": "MeinWLAN",
      "rssi": -45,
      "encryption": "secured"
    }
  ]
}
```

#### `POST /wifi/save`
Speichert neue WiFi-Credentials und verbindet sich

**Parameters:**
- `ssid`: WLAN-Name
- `password`: WLAN-Passwort

**Response:**
```json
{
  "success": true,
  "message": "Erfolgreich verbunden",
  "ip": "192.168.1.100"
}
```

#### `GET /wifi/status`
Zeigt den aktuellen WiFi-Status

**Response:**
```json
{
  "connected": true,
  "ssid": "MeinWLAN",
  "ip": "192.168.1.100",
  "rssi": -45
}
```

## Technische Details

### Speicherung der Credentials

**ESP32:**
- Verwendet die `Preferences` Library
- Namespace: `wifi`
- Keys: `ssid`, `password`
- Persistent über Neustarts und Firmware-Updates

**ESP8266:**
- Verwendet `EEPROM` Library (512 Bytes)
- SSID: Adresse 0-99
- Password: Adresse 100-199
- Persistent über Neustarts

### Verbindungs-Logik beim Boot

```
1. Lade gespeicherte WiFi-Daten aus Speicher
   ├─ Daten vorhanden? 
   │  ├─ JA: Versuche Verbindung (10s Timeout)
   │  │     ├─ Verbindung erfolgreich → Normaler Betrieb
   │  │     └─ Verbindung fehlgeschlagen → Weiter zu 2.
   │  └─ NEIN: Weiter zu 2.
   │
2. Versuche Verbindung mit Fallback-Credentials (config.cpp)
   ├─ Verbindung erfolgreich → Normaler Betrieb
   └─ Verbindung fehlgeschlagen → Starte Config Portal
   
3. Config Portal (AP-Modus)
   └─ Warte auf Benutzer-Konfiguration
```

### Sicherheitshinweise

⚠️ **Wichtig:**
- Die WiFi-Passwörter werden **unverschlüsselt** im Speicher abgelegt
- Der Config Portal AP hat ein Standard-Passwort (`12345678`)
- Für produktive Umgebungen solltest du:
  - Das AP-Passwort in `web_handlers.cpp` ändern
  - Verschlüsselung für die Credentials implementieren
  - Einen Admin-Zugang für die Config-Seite hinzufügen

### Debugging

Über den Serial Monitor (115200 Baud) erhältst du detaillierte Informationen:

```
=== WiFi Verbindung starten ===
Gespeicherte WiFi-Daten gefunden: MeinWLAN
Verbinde mit WiFi: MeinWLAN
..........
✓ WiFi erfolgreich verbunden
IP Adresse: 192.168.1.100
Wi-Fi Channel: 6
MAC Address: XX:XX:XX:XX:XX:XX
mDNS gestartet: http://rundenzaehler.local
```

## Fehlerbehebung

### Problem: ESP verbindet sich nicht

**Lösung:**
1. Prüfe, ob die SSID und das Passwort korrekt sind
2. Stelle sicher, dass das 2.4 GHz Band aktiv ist (ESP unterstützt kein 5 GHz)
3. Prüfe die Signalstärke - bewege den ESP näher zum Router
4. Lösche die gespeicherten Credentials und versuche es erneut

### Problem: Config Portal erscheint nicht

**Lösung:**
1. Warte 20-30 Sekunden nach dem Boot
2. Prüfe den Serial Monitor für Fehlermeldungen
3. Der AP startet nur, wenn keine Verbindung möglich ist

### Problem: Gespeicherte Daten löschen

**Option 1 - Über Code:**
Füge in `setup()` ein:
```cpp
clearWiFiConfig();
```

**Option 2 - Flash löschen:**
```bash
pio run -t erase
```

## Beispiel-Nutzung

### Szenario: Slotcar-Event

1. **Zuhause:** 
   - ESP konfigurieren mit dem Heim-WLAN
   - Alles testen unter `http://rundenzaehler.local`

2. **Event-Location:**
   - ESP startet, findet das Heim-WLAN nicht
   - Config Portal startet automatisch
   - Mit iPad verbinden und vor Ort WLAN konfigurieren
   - ESP verbindet sich mit dem Event-WLAN

3. **Zurück zuhause:**
   - Optional: WiFi-Config zurücksetzen oder einfach das Heim-WLAN erneut konfigurieren

## Erweiterungen

Mögliche zukünftige Erweiterungen:

- [ ] Multi-WLAN Support (mehrere Netzwerke speichern)
- [ ] Verschlüsselte Passwort-Speicherung
- [ ] Web-basiertes Factory Reset
- [ ] Automatisches Fallback auf andere gespeicherte Netzwerke
- [ ] WPS-Support für einfache Konfiguration
- [ ] QR-Code für schnelle Config-Portal-Verbindung

## Code-Dateien

Die WiFi-Manager-Implementierung verteilt sich auf folgende Dateien:

- `include/config.h` - Funktionsdeklarationen
- `src/config.cpp` - Speicher-Funktionen (load/save/clear)
- `include/web_handlers.h` - Handler-Deklarationen
- `src/web_handlers.cpp` - WiFi-Verbindungslogik und API-Endpunkte
- `src/html_content.cpp` - HTML-Interface für WiFi-Konfiguration

## Lizenz

Siehe Hauptprojekt-Lizenz
