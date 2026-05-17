#include "GPSManager.h"
#include "pins/PinMap.h"

void GPSManager::begin() {
    gpsSerial.begin(9600, SERIAL_8N1, PIN_GPS_RX, PIN_GPS_TX);
    Serial.println("[GPS] GPS serial started");
}

void GPSManager::update() {
    while (gpsSerial.available()) {
        gps.encode(gpsSerial.read());
    }
}

void GPSManager::readGPS(TelemetryData &data) {
    data.gpsValid = gps.location.isValid();

    if (data.gpsValid) {
        data.latitude = gps.location.lat();
        data.longitude = gps.location.lng();
    }

    if (gps.speed.isValid()) {
        data.gpsSpeedKmph = gps.speed.kmph();
    }

    if (gps.satellites.isValid()) {
        data.gpsSatellites = gps.satellites.value();
    }
}