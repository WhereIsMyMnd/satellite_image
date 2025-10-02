#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QtNetwork/QNetworkAccessManager>
#include "picturelabel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    PictureLabel* picture;
    QSpinBox* zoomSpBox;
    QDoubleSpinBox *latSpBox, *lonSpBox;
    QNetworkAccessManager* manager;
    QNetworkReply* reply;
    bool updateCoord = false;

private slots:
    // загрузка снимка карты с указанными координатами
    void downloadPicture();
};
