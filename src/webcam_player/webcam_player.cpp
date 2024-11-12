#include "webcam_player.h"
#include <QmouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QDebug>
#include <QContextMenuEvent>
#include <QGridLayout>

static QWidget* cam_wids[16] = { 0 };

Webcam_player::Webcam_player(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    //去除原窗口边框
    setWindowFlags(Qt::FramelessWindowHint);

	//布局head和body
	auto vlay = new QVBoxLayout();
	//边框设置为0
	vlay->setContentsMargins(0, 0, 0, 0);
	//元素间距
	vlay->setSpacing(0);
	vlay->addWidget(ui.head);
	vlay->addWidget(ui.body);
	this->setLayout(vlay);

	//相机列表和相机预览
	//水平布局器
	auto hlay = new QHBoxLayout();
	ui.body->setLayout(hlay);
	hlay->setContentsMargins(0, 0, 0, 0);
	hlay->addWidget(ui.left);
	hlay->addWidget(ui.cams);

	/*初始化右键菜单*/

	auto m = left_menu_.addMenu(QString("视图"));
	auto a = m->addAction(QString("1窗口"));
	connect(a, SIGNAL(triggered()), this, SLOT(View1()));
	a = m->addAction(QString("4窗口"));
	connect(a, SIGNAL(triggered()), this, SLOT(View4()));
	a = m->addAction(QString("9窗口"));
	connect(a, SIGNAL(triggered()), this, SLOT(View9()));
	a = m->addAction(QString("16窗口"));
	connect(a, SIGNAL(triggered()), this, SLOT(View16()));

	//默认9窗口
	View(9);
}

Webcam_player::~Webcam_player()
{}

static bool mouse_press = false;
static QPoint mouse_point;

/**
 * 鼠标拖动窗口.
 * 
 * \param ev
 */ 
void Webcam_player::mouseMoveEvent(QMouseEvent * ev)
{
	if (!mouse_press)
	{
		QWidget::mouseMoveEvent(ev);
		return;
	}
	this->move(ev->globalPosition().toPoint() - mouse_point);
}

void Webcam_player::mousePressEvent(QMouseEvent* ev)
{
	if (ev->button() == Qt::LeftButton)
	{
		mouse_press = true;
		mouse_point = ev->pos();
	}
}

void Webcam_player::mouseReleaseEvent(QMouseEvent* ev)
{
	mouse_press = false;
}

void Webcam_player::resizeEvent(QResizeEvent* ev)
{
	int x = width() - ui.btns_title->width();
	int y = ui.btns_title->y();
	ui.btns_title->move(x, y);
}

void Webcam_player::contextMenuEvent(QContextMenuEvent* event)
{
	left_menu_.exec(QCursor::pos());
	event->accept();
}

void Webcam_player::View(int count)
{
	qDebug() << "View("<<count<<")";
	//2*2、3*3、4*4
	//确定行列
	int cols = sqrt(count);
	//总窗口数量
	int wid_size = sizeof(cam_wids)/sizeof(QWidget*);
	//初始化布局器
	auto lay =(QGridLayout*)ui.cams->layout();
	if (!lay)
	{
		lay = new QGridLayout();
		lay->setContentsMargins(0, 0, 0, 0);
		lay->setSpacing(2);//元素间距
		ui.cams->setLayout(lay);
	}
	//初始化窗口
	for (int i = 0; i < count; i++)
	{
		if (!cam_wids[i])
		{
			cam_wids[i] = new QWidget();
			cam_wids[i]->setStyleSheet("background-color:rgb(51,51,51);");
		}
		lay->addWidget(cam_wids[i],i/cols,i%cols);
	}
	//清理多余窗体
	for (int i = count; i < wid_size; i++)
	{
		if (cam_wids[i])
		{
			delete cam_wids[i];
			cam_wids[i] = nullptr;
		}
	}
}

void Webcam_player::MaxWindow()
{
	ui.max->setVisible(false);
	ui.normal->setVisible(true);
	showMaximized();
}

void Webcam_player::NormalWindow()
{
	ui.max->setVisible(true);
	ui.normal->setVisible(false);
	showNormal();
}

void Webcam_player::View1()
{
	View(1);
}

void Webcam_player::View4()
{
	View(4);
}

void Webcam_player::View9()
{
	View(9);
}

void Webcam_player::View16()
{
	View(16);
}
