/*
 * ofxGstVideoRecorder.h
 *
 *  Created on: 14/01/2010
 *      Author: arturo
 */

#ifndef OFXGSTVIDEORECORDER_H_
#define OFXGSTVIDEORECORDER_H_

//#include "ofMain.h"
#include <gst/app/gstappsrc.h>
#include "ofGstUtils.h"

typedef struct
{
  GstElement *pipeline;
  GstAppSrc *source;
  unsigned char * pixels;
  int width, height, bpp;
  bool new_frame;
  int fps;
} AppData;

class ofxGstVideoRecorder: public ofGstVideoUtils {
public:
	ofxGstVideoRecorder();
	virtual ~ofxGstVideoRecorder();

	enum Codec{
		THEORA,
		H264,
		MP4,
		XVID,
		JPEG,
		LOSLESS_JPEG,
		QT_ANIM,
		PDF,
		PNG_SEQUENCE,
		FLV,
		FLV_H264,
		YUV,
		DIRAC
	};

	void udpStreamTo(string comma_separated_ips);
	void tcpStreamTo(string host, int port);
	void pipeTo(string command);

	void setup(int width, int height, int bpp, string file, Codec codec, int fps=30);

	void shutdown();

	void newFrame(ofPixels & pixels);


protected:

	GstAppSrc * gstSrc;
	string src;
	string sink;
	bool bIsUdpStream, bIsTcpStream;


};

#endif /* OFXGSTVIDEORECORDER_H_ */
