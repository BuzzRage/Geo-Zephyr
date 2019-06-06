# Geo-Zephyr
Contrôle de qualité de l'air Géo-localisée.

**Matériel utilisé**:
- *Sparkfun ESP32 Thing*: équipé d'un dual-core 32bits Xtensa (Tensilica)
- *Adafruit BME680*: Capteur de température, d'humidité, de pression et de gaz
- *Ultimate GPS Breakout v3*: Capteur GPS PA6H1F1627
<!-- - *GP-735T*: module GPS -->

**Bibliothèques nécessaires**:
- `Adafruit_Sensor.h` [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor "Lien GitHub pour la Unified Sensor library")
- `Adafruit_BME680.h` [Adafruit_BME680](https://github.com/adafruit/Adafruit_BME680 "Lien GitHub pour le BME680")
- `Adafruit_GPS.h` [Adafruit_GPS](https://github.com/adafruit/Adafruit_GPS/ "Lien GitHub pour la bibliothèque GPS d'Adafruit")
- `TinyGPS++.h` [TinyGPS++](https://github.com/mikalhart/TinyGPSPlus "Lien GitHub pour la classe TinyGPS++")

**TODO**:
  - Régler le capteur *BME680*
  - Faire structure de donnée pour Qualité de l'Air / coordonnées (/ autres data ?)
  - Faire Interface Web pour avoir les informations (+ requète geocoding ?)
  - Exportation les données en .csv (et .gpx ?)
  - passer sous esp-idf (et assigner un core au WiFi et un core aux périphériques)
  - Faire interface AccessPoint pour première connexion
