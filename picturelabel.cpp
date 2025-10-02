#include "picturelabel.h"
#include <cmath>

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

    emit coordUpd(lon, lat);
}

void PictureLabel::valsUpdate(double centerLon, double centerLat, int zoom)
{
    this->centerLon = centerLon;
    this->centerLat = centerLat;
    this->zoom = zoom;
}
