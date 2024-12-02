#pragma once
#include <string>
#include "xtools.h"

class XCameraRecord:public XThread
{
public:
	void set_rtsp_url(std::string url) { rtsp_url_ = url; }
	void set_save_path(std::string path) { save_path_ = path; }
	void file_sec(long long s) { file_sec_ = s; }

private:
	void Main() override;
	std::string rtsp_url_;
	std::string save_path_;//存储根目录
	long long file_sec_ = 5;//每隔五秒创建新文件
};

