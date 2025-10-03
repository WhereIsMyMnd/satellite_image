#include "picturelabel.h"
#include <cmath>

#if GEO_LIB
#include <GeographicLib/UTMUPS.hpp>
#endif

PictureLabel::PictureLabel()
{
    setMouseTracking(true);
    setFixedSize(600, 400);
    setContentsMargins(0, 0, 0, 0);
}

void PictureLabel::mouseMoveEvent(QMouseEvent *event)
{
    int mouseX = event->pos().x();    // X позиция мышки
    int mouseY = event->pos().y();    // Y позиция мышки
    int pictureHeight = height();     // высота картинки
    int pictureWidth = width();       // ширина картинки

    int mapSize = 256 * std::pow(2, zoom);                                                 // размер отобржаемого куска карты на данном зуме
    double sinLat = sin(centerLat * M_PI / 180.0);                                         // синус широты центра
    double centerX = (centerLon + 180.0) / 360.0 * mapSize;                                // центр долготы
    double centerY = (0.5 - log((1 + sinLat) / (1 - sinLat)) / (4 * M_PI)) * mapSize;      // центр широты

    double mouseXpic = centerX + mouseX - pictureWidth / 2.0;      // X позиция мышки в глобальных пикселях карты
    double mouseYpic = centerY + mouseY - pictureHeight / 2.0;     // Y позиция мышки в глобальных пикселях карты

    double lon = mouseXpic / mapSize * 360.0 - 180.0;              // долгота на Х позиции мышки
    while (lon > 180.0) lon -= 360.0;
    while (lon < -180.0) lon += 360.0;

    double n = M_PI - 2.0 * M_PI * mouseYpic / mapSize;
    double lat = 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));    // широта на Y позиции мышки
    
    double eastings, northings;
    int zone;
    
#if GEO_LIB
    bool northp;
    GeographicLib::UTMUPS::Forward(lat, lon, zone, northp, eastings, northings);
        
    if (!northp) {
        northings += 10000000.0;
    }
#else
    std::tie(eastings, northings, zone) = latLonToUtm(lat, lon);
#endif

    emit coordUpd(lon, lat, eastings, northings, zone);
}

void PictureLabel::valsUpdate(double centerLon, double centerLat, int zoom)
{
    this->centerLon = centerLon;
    this->centerLat = centerLat;
    this->zoom = zoom;
}

std::tuple<double, double, int> PictureLabel::latLonToUtm(double lat, double lon)
{
    const double a = 6378137.0;                  // большая полуось
    const double f = 1 / 298.257223563;          // сжатие
    const double k0 = 0.9996;

    double latRad = lat * M_PI / 180.0;
    double lonRad = lon * M_PI / 180.0;

    int zone = int((lon + 180) / 6) + 1;
    double lon0 = (zone - 1) * 6 - 180 + 3;      // центральный меридиан зоны
    double lon0Rad = lon0 * M_PI / 180.0;

    double e = sqrt(f * (2 - f));
    double N = a / sqrt(1 - e * e * sin(latRad) * sin(latRad));
    double T = tan(latRad) * tan(latRad);
    double C = (e * e) / (1 - e * e) * cos(latRad) * cos(latRad);
    double A = cos(latRad) * (lonRad - lon0Rad);

    double M = a*((1 - e * e / 4 - 3 * e * e * e * e / 64 - 5 * e * e * e * e * e * e / 256) * latRad
           - (3 * e * e / 8 + 3 * e * e * e * e / 32 + 45 * e * e * e * e * e * e / 1024) * sin(2 * latRad)
           + (15 * e * e * e * e / 256 + 45 * e * e * e * e * e * e / 1024)* sin(4 * latRad)
           - (35 * e * e * e * e * e * e / 3072) * sin(6 * latRad));

    double E = k0 * N * (A + (1 - T + C) * A * A * A / 6 + (5 - 18 * T + T * T + 72 * C - 58 * (e * e) / (1 - e * e)) * A * A * A * A * A / 120) + 500000.0;
    double N_utm = k0 * (M + N * tan(latRad) * (A * A / 2 + (5 - T + 9 * C + 4 * C * C) * A * A * A * A / 24 + (61 - 58 * T + T * T + 600 * C - 330 * (e * e)/(1 - e * e)) * A * A * A * A * A * A / 720));

    if (lat < 0) N_utm += 10000000;         // для южного полушария

    return {E, N_utm, zone};
}
