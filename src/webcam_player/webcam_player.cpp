#include "webcam_player.h"
#include "xcamear_config.h"
#include "xcamera_widget.h"

#include <QmouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QDebug>
#include <QDialog>
#include <QFormLayout>
#include <QContextMenuEvent>
#include <QGridLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <sstream> 

#define CAM_CONF_PATH "cam.db"

static XCameraWidget* cam_wids[16] = { 0 };


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

void Webcam_player::View(int count)
{
	qDebug() << "View(" << count << ")";
	//2*2、3*3、4*4
	//确定行列
	int cols = sqrt(count);
	//总窗口数量
	int wid_size = sizeof(cam_wids) / sizeof(QWidget*);
	//初始化布局器
	auto lay = (QGridLayout*)ui.cams->layout();
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
			cam_wids[i] = new XCameraWidget();
			cam_wids[i]->setStyleSheet("background-color:rgb(51,51,51);");
		}
		lay->addWidget(cam_wids[i], i / cols, i % cols);
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

void Webcam_player::contextMenuEvent(QContextMenuEvent* event)
{
	left_menu_.exec(QCursor::pos());
	event->accept();
}

void Webcam_player::SetCam(int index)
{
	auto c = XCameraConfig::Instance();
	QDialog dlg(this);
	dlg.resize(800, 200);
	QFormLayout* lay = new QFormLayout;
	dlg.setLayout(lay);

	QLineEdit* name_edit = new QLineEdit;
	lay->addRow(QString("名称"), name_edit);

	QLineEdit* url_edit = new QLineEdit;
	lay->addRow(QString("主码流"), url_edit);

	QLineEdit* sub_url_edit = new QLineEdit;
	lay->addRow(QString("辅码流"), sub_url_edit);

	QLineEdit* save_path_edit = new QLineEdit;
	lay->addRow(QString("保存目录"), save_path_edit);

	QPushButton* btn_save = new QPushButton;
	btn_save->setText(QString("保存"));
	connect(btn_save, &QPushButton::clicked, &dlg, &QDialog::accept);

	lay->addRow("", btn_save);

	//编辑,读入原数据显示
	if (index >= 0)
	{
		auto cam = c->GetCam(index);
		name_edit->setText(cam.name);
		url_edit->setText(cam.url);
		sub_url_edit->setText(cam.sub_url);
		save_path_edit->setText(cam.save_path);
	}

	while (true)
	{
		//点击保存按钮退出
		if (dlg.exec() == QDialog::Accepted)
		{
			if (name_edit->text().isEmpty())
			{
				QMessageBox::information(0, "error", QString("请输入名称"));
				continue;
			}
			if (url_edit->text().isEmpty())
			{
				QMessageBox::information(0, "error", QString("请输入主码流地址"));
				continue;
			}
			if (sub_url_edit->text().isEmpty())
			{
				QMessageBox::information(0, "error", QString("请输入辅码流地址"));
				continue;
			}
			if (save_path_edit->text().isEmpty())
			{
				QMessageBox::information(0, "error", QString("请输入保存位置"));
				continue;
			}
			break;
		}
		return;
	}
	XCameraData data;
	strcpy_s(data.name, name_edit->text().toUtf8());
	strcpy_s(data.url, url_edit->text().toUtf8());
	strcpy_s(data.sub_url, sub_url_edit->text().toUtf8());
	strcpy_s(data.save_path, save_path_edit->text().toUtf8());
	if (index >= 0)//修改
	{
		c->SetCam(index, data);
	}
	else//新增
	{
		c->Push(data);
	}
	c->Save(CAM_CONF_PATH);
	RefreshCams();
	
	delete lay;
	delete name_edit;
	delete url_edit;
	delete sub_url_edit;
	delete save_path_edit;
	delete btn_save;
}

void Webcam_player::timerEvent(QTimerEvent* ev)
{
	//总窗口数量
	int wid_size = sizeof(cam_wids) / sizeof(QWidget*);
	for (int i = 0; i < wid_size; i++)
	{
		if (cam_wids[i])
		{
			//渲染多窗口视频
			cam_wids[i]->Draw();
		}
	}
}

void Webcam_player::SetCam()
{
	int row = ui.cam_list->currentIndex().row();
	if (row < 0)
	{
		QMessageBox::information(this, "error", QString("请选择摄像机"));
		return;
	}
	SetCam(row);
}

void Webcam_player::DelCam()
{
	int row = ui.cam_list->currentIndex().row();
	if (row < 0)
	{
		QMessageBox::information(this, "error", QString("请选择摄像机"));
		return;
	}
	std::stringstream ss;
	ss << "确认要删除摄像机" << ui.cam_list->currentItem()->text().toUtf8().constData();
	ss << "吗";
	if (QMessageBox::information(this,
		"confirm", QString(ss.str().c_str()),
		QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
	{
		return;
	}
	XCameraConfig::Instance()->DelCam(row);
	RefreshCams();
}

void Webcam_player::AddCam()
{
	SetCam(-1);
}

void Webcam_player::RefreshCams()
{
	auto c = XCameraConfig::Instance();
	ui.cam_list->clear();
	int count = c->GetCamCount();
	for (int i = 0; i < count; i++)
	{
		auto cam = c->GetCam(i);
		auto item = new QListWidgetItem(QIcon(":/Webcam_player/img/cam.png"), cam.name);
		ui.cam_list->addItem(item);
	}
}

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
	XCameraConfig::Instance()->Load(CAM_CONF_PATH);

	/*{
		XCameraData cd;
		strcpy_s(cd.name, "camera1");
		strcpy_s(cd.save_path, ".\\camera1\\");
		strcpy_s(cd.url, "rtsp://127.0.0.1:8554/test");
		strcpy_s(cd.sub_url, "rtsp://127.0.0.1:8554/test");
		XCameraConfig::Instance()->Push(cd);
	}*/

	//刷新左侧摄像机
	RefreshCams();

	//启动定时器渲染视频
	startTimer(1);
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

void Webcam_player::resizeEvent(QResizeEvent* ev)
{
	int x = width() - ui.btns_title->width();
	int y = ui.btns_title->y();
	ui.btns_title->move(x, y);
}

/**
 * 鼠标拖动窗口.
 *
 * \param ev
 */
static bool mouse_press = false;
static QPoint mouse_point;
void Webcam_player::mouseMoveEvent(QMouseEvent* ev)
{
	if (!mouse_press)
	{
		QWidget::mouseMoveEvent(ev);
		return;
	}
	this->move(ev->globalPosition().toPoint() - mouse_point);
}

Webcam_player::~Webcam_player()
{}

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