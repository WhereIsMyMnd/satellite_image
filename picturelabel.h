#pragma once

#include <QLabel>
#include <QMouseEvent>
#include <tuple>

class PictureLabel : public QLabel
{
    Q_OBJECT
public:
    PictureLabel();

signals:
    // передача рассчитанных координат
    void coordUpd(double lon, double lat, double eastings, double northings, int zone);

public slots:
    // обновление введенных коорднат и зума
    void valsUpdate(double centerLon, double centerLat, int zoom);

protected:
    // переопределение mouseMoveEvent для отслеживания координат по курсору мышки
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    double centerLon, centerLat;
    int zoom;
    // перевод из WGS в UTM (можно использовать библиотеку GeographicLib, метод Forward)
    std::tuple<double, double, int> latLonToUtm(double lat, double lon);
};

