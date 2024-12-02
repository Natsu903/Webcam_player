#include "webcam_player.h"
#include "xcamear_config.h"
#include <QtWidgets/QApplication>
#include <qDebug>
#include <QDir>
#include "xcamera_record.h"

//#define TEST_CAM_PATH "test.db"
#define TEST_URL "rtsp://127.0.0.1:8554/test"
#define RTSP_URL "rtsp://admin:GZH&password@192.168.31.234:554/Streaming/Channels/101"

int main(int argc, char *argv[])
{
	/*auto* xc = XCameraConfig::Instance();
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
	xc->DelCam(0);*/

	const char* save_path = "./video/0/";
	QDir dir;
	dir.mkpath(save_path);

	XCameraRecord xr;
	xr.set_rtsp_url(RTSP_URL);
	xr.set_save_path(save_path);
	xr.Start();

    QApplication a(argc, argv);
    Webcam_player w;
    w.show();
    auto re = a.exec();
	xr.Stop();
	return re;
}
