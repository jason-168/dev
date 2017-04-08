/*
 * sl_protocol.h
 *
 *  Created on: Jun 26, 2016
 *      Author: root
 */

#ifndef SL_PROTOCOL_H_
#define SL_PROTOCOL_H_

#include <stdint.h>

#define SL_ID_LEN		32
#define SL_IP_LENGTH	46


#define INTERNAL_PUNCHF_SPEC_FAILED	50

#define SL_CHANNEL_BASE_ERRNO		10000

#define SL_CHANNEL_RELAY_FAILED		11

#define SL_CHANNEL_RELAY_REQTIMOUT	12


//-----------------------------------------------------------------
#define SL_LOGIN_OK	1

#define SL_DEVICE_OFFLINK			(SL_CHANNEL_BASE_ERRNO + 1)

#define SL_PROT_PARSE_FAILED		(SL_CHANNEL_BASE_ERRNO + 10)

#define SL_UNAME_OR_PWD_ERROR		(SL_CHANNEL_BASE_ERRNO + 20)

#define SL_USER_UNLOGIN				(SL_CHANNEL_BASE_ERRNO + 700)





typedef enum SLReqType{
	SL_REQ_NOP	= 0,
	SL_REQ_STREAM		= 1,
	SL_REQ_VOICECALL	= 2,
	SL_REQ_VIDEOCALL	= 3,

	SL_REQ_PLAYBACK		= 4,
	SL_REQ_UPLOAD		= 5,
	SL_REQ_DOWNLOAD		= 6,

	SL_REQ_MAX = SL_REQ_DOWNLOAD,
}SLReqType;

typedef enum SLStreamMask{
	SL_STREAM_NOP = 0,
	SL_STREAM_AUDIO = 1,
	SL_STREAM_VIDEO = 1 << 1,
}SLStreamMask;

typedef enum SLACodecID{
	SL_ACODEC_ID_NONE = 0,
	SL_ACODEC_ID_AAC = 1,
	SL_ACODEC_ID_G711A = 2,
	SL_ACODEC_ID_G711U = 3,
	SL_ACODEC_ID_OPUS = 4,
}SLACodecID;

typedef enum SLVCodecID{
	SL_VCODEC_ID_NONE = 0,
	SL_VCODEC_ID_H264 = 1,
	SL_VCODEC_ID_HEVC = 2,
}SLVCodecID;

#define SL_FRAME_TYPE_I 1
#define SL_FRAME_TYPE_P 2

typedef enum SLFrameType{
	SL_I_FRAME	= SL_FRAME_TYPE_I,
	SL_P_FRAME	= SL_FRAME_TYPE_P,
}SLFrameType;

typedef struct SLAudioFormat{
	int16_t haveAudio;

	uint16_t acodecID;		//编码方式
	uint16_t samplerate;	//采样率
	uint16_t channels;		//通道数
}SLAudioFormat;

typedef struct SLVideoFormat{
	int16_t haveVideo;

	uint16_t vcodecID;		//编码方式
	uint16_t width;			//图像宽度
	uint16_t height;		//图像高度
	uint16_t framerate;		//帧率, fps
	uint16_t ifiv;			//I Frames Interval I帧间隔
}SLVideoFormat;

typedef struct SLAVFormat{
	SLAudioFormat audiofmt;
	SLVideoFormat videofmt;
}SLAVFormat;

typedef struct SLStreamReq{
	int chno; //channel number
	int stream_mask; //0: data stream, 1: audio stream, 2: video stream, or 1|2
	int stream_type; //0: main stream, 1: sub stream
}SLStreamReq;

typedef SLStreamReq SLVideocallReq;

int StreamRes_pack(char* buffer, int len, int restype, int16_t rc, SLAVFormat* avfmt);
int StreamRes_parse(int16_t* rc, SLAVFormat* avfmt, const char* data, int datalen);

typedef enum SLPlaybackCtrl{
	SL_PLAYBACK_START		= 1,
	SL_PLAYBACK_PAUSE		= 2,
	SL_PLAYBACK_RESUME		= 3,
	SL_PLAYBACK_SEEK		= 4,
	SL_PLAYBACK_SPEEDUP		= 5,
	SL_PLAYBACK_SPEEDDOWN	= 6,
	SL_PLAYBACK_STOP		= 7,
}SLPlaybackCtrl;

typedef struct SLPlaybackReq{
	char file[256]; // media file
	int chno;  //channel number
	int start_ts; //start time
}SLPlaybackReq;

typedef struct SLPlaybackRes{
	int rc;

	int start_ts; //start time
	int duration;//sec

	SLAVFormat avfmt;
}SLPlaybackRes;



/*
录像，图像等搜索请求，参数位json格式
{
	"chno":%d,			//int，通道号chno >= ０时，搜索特定的通道。-１则位搜索全部的通道。
	"mediatype":%d,		//int，媒体类型，1:为录像，２为图像。
	"subtype":%d,		//int，录像子类型，
	"startIndex":%d,	//int，文件索引
	"startDate":%lld,	//int64_t，开始日期，单位ms
	"endDate":%lld,		//int64_t，结束日期，单位ms
}

录像，图像等搜索回复，json格式
{
	"rc":%d,
	"num":%d,
	"files":[
	{
		"chno":%d,
		"mediatype":%d,
		"subtype":%d,
		"file":"%s",
		"date":%lld,
		"start:"%lld,	//当文件位录像，表示视频开始时间
		"duration":%lld, //当文件位录像，表示视频播放总时间
	}
	]
}
*/

#endif /* SL_PROTOCOL_H_ */
