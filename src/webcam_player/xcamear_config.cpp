﻿#include "xcamear_config.h"
#include <fstream>
using namespace std;

void XCameraConfig::Push(const XCameraData& data)
{
	unique_lock<mutex> lock(mux_);
	cams_.push_back(data);
}

XCameraData XCameraConfig::GetCam(int index)
{
	unique_lock<mutex> lock(mux_);
	if (index<0 || index>cams_.size())
		return XCameraData();
	return cams_[index];
}

bool XCameraConfig::SetCam(int index, const XCameraData& data)
{
	unique_lock<mutex> lock(mux_);
	if (index<0 || index>cams_.size()) return false;
	cams_[index] = data;
	return true;
}

bool XCameraConfig::DelCam(int index)
{
	unique_lock<mutex> lock(mux_);
	if (index<0 || index>cams_.size()) return false;
	cams_.erase(cams_.begin() + index);
	return true;
}

int XCameraConfig::GetCamCount()
{
	unique_lock<mutex> lock(mux_);
	return cams_.size();
}

bool XCameraConfig::Save(const char* path)
{
	if (!path) return false;
	ofstream ofs(path, ios::binary);
	if (!ofs) return false;
	unique_lock<mutex> lock(mux_);
	for (XCameraData cam : cams_)
	{
		ofs.write((char*)&cam, sizeof(cam));
	}
	return true;
}

bool XCameraConfig::Load(const char* path)
{
	if (!path) return false;
	ifstream ifs(path, ios::binary);
	if (!ifs) return false;
	XCameraData data;
	unique_lock<mutex> lock(mux_);
	cams_.clear();
	while (true)
	{
		ifs.read((char*)&data, sizeof(data)); 
		if (ifs.gcount() != sizeof(data));
		{
			ifs.close();
			return true;
		}
		cams_.push_back(data);
	}
	return true;
}
