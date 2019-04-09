# Geo-Zephyr
Contrôle de qualité de l'air Géo-localisée.

**Matériel utilisé**:
- *Sparkfun ESP32 Thing*: équipé d'un dual-core 32bits Xtensa (Tensilica)
- *Adafruit BME680*: Capteur de température, d'humidité, de pression et de gaz
- *GP-735T*: module GPS

**Bibliothèques nécessaires**:
`Adafruit_Sensor.h` [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor "Lien GitHub pour la Unified Sensor library")
`Adafruit_BME680.h` [Adafruit_BME680](https://github.com/adafruit/Adafruit_BME680 "Lien GitHub pour le BME680")

**TODO**:
  - Tester/tuner *BME680*
  - Tester module GPS
  - Faire structure de donnée pour Qualité de l'Air / autres data
  - Interface Web
  - Objectify
  - passer sous esp-idf (et assigner un core au WiFi et un core aux sensors)
  - Faire interface AccessPoint pour première connexion
