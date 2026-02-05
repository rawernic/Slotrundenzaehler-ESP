# 📱 WiFi-Konfiguration - Schnellstart

## ⚡ Schnelle Einrichtung

### 1️⃣ ESP zum ersten Mal starten

Der Rundenzähler startet im **Konfigurations-Portal-Modus**, wenn keine WiFi-Daten gespeichert sind.

### 2️⃣ Mit dem ESP verbinden

📱 **Auf deinem iPad/iPhone/Android:**

1. Öffne die **WiFi-Einstellungen**
2. Suche nach dem Netzwerk: **`Rundenzaehler-Setup`**
3. Verbinde dich mit dem Passwort: **`12345678`**

### 3️⃣ Konfigurationsseite öffnen

Öffne deinen Browser und gehe zu:
```
http://192.168.4.1/wifi/config
```

Die Seite lädt automatisch und scannt nach verfügbaren Netzwerken.

### 4️⃣ Dein WLAN auswählen

1. **Netzwerke scannen**: Die Seite zeigt alle verfügbaren WLANs
2. **Netzwerk auswählen**: Klicke auf dein WLAN
3. **Passwort eingeben**: Gib dein WLAN-Passwort ein
4. **Speichern**: Klicke auf "Speichern & Verbinden"

### 5️⃣ Fertig! 🎉

- Der ESP startet automatisch neu
- Er verbindet sich mit deinem WLAN
- Ab jetzt erreichbar unter: **`http://rundenzaehler.local`**

---

## 🔄 WiFi nachträglich ändern

Wenn der ESP bereits mit einem WLAN verbunden ist:

1. Gehe auf die Hauptseite: `http://rundenzaehler.local`
2. Klicke auf das **WiFi-Symbol** (oben rechts)
3. Gib neue WiFi-Daten ein
4. Speichern und fertig!

---

## 🆘 Problemlösung

### ❓ Config Portal erscheint nicht?

**Warte 20-30 Sekunden** nach dem Start. Der ESP versucht zuerst, sich mit gespeicherten Netzwerken zu verbinden.

### ❓ ESP verbindet sich nicht?

- ✅ Prüfe SSID und Passwort
- ✅ Stelle sicher, dass 2.4 GHz aktiv ist (kein 5 GHz!)
- ✅ Näher zum Router bewegen
- ✅ Config löschen und neu starten

### ❓ Gespeicherte Daten löschen?

Füge temporär in `src/main.cpp` in die `setup()` Funktion ein:
```cpp
clearWiFiConfig();
delay(1000);
```

Dann den ESP flashen und neu starten.

---

## 📍 Wichtige URLs

| Zweck | URL |
|-------|-----|
| Hauptseite (normal) | `http://rundenzaehler.local` |
| Hauptseite (IP) | `http://[IP-ADRESSE]` |
| WiFi-Config (AP-Modus) | `http://192.168.4.1/wifi/config` |
| WiFi-Config (normal) | `http://rundenzaehler.local/wifi/config` |
| WiFi-Status (API) | `http://rundenzaehler.local/wifi/status` |

---

## 🎯 Anwendungsfälle

### 🏠 Zuhause
```
ESP startet → Verbindet sich automatisch mit Heim-WLAN → Fertig!
```

### 🏁 Event/Rennstrecke
```
ESP startet → Kein bekanntes WLAN → Config Portal öffnet sich
→ Mit iPad/Phone verbinden → Event-WLAN konfigurieren → Fertig!
```

### 🔧 WLAN wechseln
```
WiFi-Button klicken → Neues WLAN auswählen → Speichern → Fertig!
```

---

## 💡 Tipps

- **mDNS funktioniert nicht?** Verwende die IP-Adresse (steht im Serial Monitor)
- **Mehrere Geräte?** Ändere den mDNS-Namen in `web_handlers.cpp` Zeile 49
- **Sicherer machen?** Ändere das AP-Passwort in `web_handlers.cpp` Zeile 427

---

Viel Erfolg! 🏎️💨
