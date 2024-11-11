#pragma once

#include <QtWidgets/QWidget>
#include "ui_webcam_player.h"

class Webcam_player : public QWidget
{
    Q_OBJECT

public:
    Webcam_player(QWidget *parent = nullptr);
    ~Webcam_player();

private:
    Ui::Webcam_playerClass ui;
};
