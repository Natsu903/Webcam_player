#pragma once

#include <QtWidgets/QWidget>
#include <QMenu>
#include "ui_webcam_player.h"

class Webcam_player : public QWidget
{
    Q_OBJECT

public:
    Webcam_player(QWidget *parent = nullptr);
    ~Webcam_player();

    //重载事件 用于拖动窗口 
    void mouseMoveEvent(QMouseEvent* ev) override;
    void mousePressEvent(QMouseEvent* ev) override;
    void mouseReleaseEvent(QMouseEvent* ev) override;

    //窗口大小发生变化
    void resizeEvent(QResizeEvent* ev) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

    void View(int count);

public slots:
    void MaxWindow();
    void NormalWindow();

    void View1();
    void View4();
    void View9();
    void View16();

private:
    Ui::Webcam_playerClass ui;
    QMenu left_menu_;
};
