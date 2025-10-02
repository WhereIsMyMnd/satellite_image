#pragma once

#include <QLabel>
#include <QMouseEvent>

class PictureLabel : public QLabel
{
    Q_OBJECT
public:
    PictureLabel();

signals:
    void coordUpd(double lon, double lat);

public slots:
    void valsUpdate(double centerLon, double centerLat, int zoom);

protected:
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    double centerLon, centerLat;
    int zoom;
};
