#include "webcam_player.h"
#include "xcamear_config.h"
#include <QtWidgets/QApplication>
#include <qDebug>

#define TEST_CAM_PATH "test.db"

int main(int argc, char *argv[])
{
	auto* xc = XCameraConfig::Instance();
	xc->Load(TEST_CAM_PATH);
    {
		XCameraData cd;
		strcpy_s(cd.name, "camera1");
		strcpy_s(cd.save_path, ".\\camera1\\");
		strcpy_s(cd.url, "rtsp://127.0.0.1:8554/test");
		strcpy_s(cd.sub_url, "rtsp://127.0.0.1:8554/test");
		xc->Push(cd);
    }
    {
		XCameraData cd;
		strcpy_s(cd.name, "camera2");
		strcpy_s(cd.save_path, ".\\camera2\\");
		strcpy_s(cd.url, "rtsp://127.0.0.1:8554/test");
		strcpy_s(cd.sub_url, "rtsp://127.0.0.1:8554/test");
		xc->Push(cd);
    }
	int cam_count = xc->GetCamCount();
	for (int i = 0; i < cam_count; i++)
	{
		auto cam = xc->GetCam(i);
		qDebug() << cam.name;
	}
	qDebug() << "=============Set===============";
	auto d1 = xc->GetCam(0);
	strcpy_s(d1.name, "camera_001");
	xc->SetCam(0, d1);

	cam_count = xc->GetCamCount();
	for (int i = 0; i < cam_count; i++)
	{
		auto cam = xc->GetCam(i);
		qDebug() << cam.name;
	}

	xc->Save(TEST_CAM_PATH);

	qDebug() << "=============Del===============";
	xc->DelCam(1);
	cam_count = xc->GetCamCount();
	for (int i = 0; i < cam_count; i++)
	{
		auto cam = xc->GetCam(i);
		qDebug() << cam.name;
	}

	cam_count = xc->GetCamCount();
	for (int i = 0; i < cam_count; i++)
	{
		auto cam = xc->GetCam(i);
		qDebug() << cam.name;
	}
    QApplication a(argc, argv);
    Webcam_player w;
    w.show();
    return a.exec();
}
