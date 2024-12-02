#include "xcamera_record.h"
#include "xdemux_task.h";
#include "xmuxtask.h"
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace chrono;

/**
 * 生成存储的视频文件名.
 * 
 * \return 
 */
static string getFileName(string path)
{
	stringstream ss;
	//获取当前时间
	auto now = system_clock::now(); 
	auto t = system_clock::to_time_t(now); 
	tm local_time; 
	localtime_s(&local_time, &t); 
	ss << path << "/cam_" << put_time(&local_time, "%Y_%m_%d_%H_%M_%S") << ".mp4"; 
	return ss.str();
}

void XCameraRecord::Main()
{
	XDemuxTask demux;
	XMuxTask mux;
	if (rtsp_url_.empty())
	{
		LOGERROR("open rtsp url failed!");
		return;
	}
	//自动重连
	while (!is_exit_)
	{
		if (demux.Open(rtsp_url_))
		{
			break;
		}
		MSleep(10);
		continue;
	}
	//音视频参数
	auto vpara = demux.CopyVideoPara();
	if (!vpara)
	{
		LOGERROR("demux.CopyVideoPara() failed!");
		demux.Stop();
		return;
	}
	//启动解封装线程
	demux.Start();
	auto apara = demux.CopyAudioPara();

	AVCodecParameters* para = nullptr;//音频参数
	AVRational* timebase = nullptr;//音频时间基数
	if (apara)
	{
		para = apara->para;
		timebase = apara->time_base;
	}

	if (!mux.Open(getFileName(save_path_).c_str(), vpara->para, vpara->time_base, para, timebase))
	{
		LOGERROR("mux.Open failed!");
		demux.Stop();
		mux.Stop();
		return;
	}
	demux.set_next(&mux);
	mux.Start();
	auto cur = NowMs();
	while (!is_exit_)
	{
		//定时创建新的文件
		if (NowMs() - cur > file_sec_ * 1000)
		{
			cur = NowMs();
			mux.Stop();//停止存储写入索引
			if (!mux.Open(getFileName(save_path_).c_str(), vpara->para, vpara->time_base, para, timebase))
			{
				LOGERROR("mux.Open failed!");
				demux.Stop();
				mux.Stop();
				return;
			}
			mux.Start();
		}
		MSleep(10);
	}

	mux.Stop();
	demux.Stop();
}
