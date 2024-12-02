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

    //显示预览相机窗口
    void View(int count);

    //刷新左侧相机列表
    void RefreshCams();

	//编辑摄像机配置
	void SetCam(int index);

    void timerEvent(QTimerEvent* ev) override;

public slots:
    void MaxWindow();
    void NormalWindow();

    void View1();
    void View4();
    void View9();
    void View16();

    //新增摄像机配置
    void AddCam();
    //修改摄像机配置
    void SetCam();
    //删除摄像机配置
    void DelCam();

private:
    Ui::Webcam_playerClass ui;
    QMenu left_menu_;
};
