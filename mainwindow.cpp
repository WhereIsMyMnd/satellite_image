#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QtNetwork/QNetworkReply>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    picture = new QLabel;
    picture->setFixedSize(600, 400);
    picture->setContentsMargins(0, 0, 0, 0);

    latSpBox = new QDoubleSpinBox;
    latSpBox->setDecimals(4);
    latSpBox->setRange(-90.0000, 90.0000);
    latSpBox->setValue(55.7558);

    lonSpBox = new QDoubleSpinBox;
    lonSpBox->setDecimals(4);
    lonSpBox->setRange(-180.0000, 180.0000);
    lonSpBox->setValue(37.6173);

    zoomSpBox = new QSpinBox;
    zoomSpBox->setRange(0, 21);
    zoomSpBox->setValue(11);

    QPushButton* downloadBtn = new QPushButton("Загрузить");
    connect(downloadBtn, &QPushButton::clicked, this, &MainWindow::downloadPicture);

    QHBoxLayout* ctrlLayout = new QHBoxLayout;
    ctrlLayout->addStretch();
    ctrlLayout->addWidget(new QLabel("Широта:"));
    ctrlLayout->addWidget(latSpBox);
    ctrlLayout->addWidget(new QLabel("Долгота:"));
    ctrlLayout->addWidget(lonSpBox);
    ctrlLayout->addWidget(new QLabel("Зум:"));
    ctrlLayout->addWidget(zoomSpBox);
    ctrlLayout->addWidget(downloadBtn);
    ctrlLayout->addStretch();

    manager = new QNetworkAccessManager;

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(picture);
    mainLayout->addLayout(ctrlLayout);

    ui->centralwidget->setLayout(mainLayout);
    ui->centralwidget->setFocus();
    ui->centralwidget->adjustSize();
    adjustSize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::downloadPicture()
{
    QString url = QString("https://static-maps.yandex.ru/1.x/?ll=%1,%2&z=%3&l=sat,skl&size=600,400").arg(lonSpBox->value()).arg(latSpBox->value()).arg(zoomSpBox->value());

    QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(url)));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QPixmap map;
        map.loadFromData(reply->readAll());
        picture->setPixmap(map);
        reply->deleteLater();
    });
}
