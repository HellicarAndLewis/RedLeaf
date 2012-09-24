/*
 * ofxGstVideoRecorder.cpp
 *
 *  Created on: 14/01/2010
 *      Author: arturo
 */

#include "ofxGstVideoRecorder.h"
#include <gst/app/gstappsink.h>
#include <gst/app/gstappbuffer.h>
#include "ofAppRunner.h"

ofxGstVideoRecorder::ofxGstVideoRecorder() {
	bIsTcpStream = false;
	bIsUdpStream = false;
	gstSrc = NULL;
}

void ofxGstVideoRecorder::shutdown()
{
    if(gstSrc)
	    gst_app_src_end_of_stream (gstSrc);
	/*if(getPipeline()) gst_element_send_event (getPipeline(), gst_event_new_eos());*/
	gst_object_unref(gstSrc);
	gstSrc = NULL;
}

ofxGstVideoRecorder::~ofxGstVideoRecorder() {
	shutdown();
}

void ofxGstVideoRecorder::udpStreamTo(string comma_separated_ips){
	sink  = " multiudpsink clients=" + comma_separated_ips;
	bIsUdpStream = true;
}

void ofxGstVideoRecorder::tcpStreamTo(string host, int port){
	sink = " tcpserversink host="+host+" port="+ofToString(port);
	bIsTcpStream = true;
}

void ofxGstVideoRecorder::setup(int width, int height, int bpp, string file, Codec _codec, int fps){
	close();
	ofGstUtils::startGstMainLoop();

	file = ofToDataPath(file);
	
	codec = _codec;

	//gst_debug_set_active (true);

	if(!bIsTcpStream && !bIsUdpStream)
		sink= "filesink name=video-sink sync=false location=" + file;

	string encoder;
	string muxer = "avimux ! ";
	string colorspaceconversion = " ffmpegcolorspace ! ";

	if(ofFilePath(file).getExtension()=="avi") muxer = "avimux ! ";
	else if(ofFilePath(file).getExtension()=="mp4") muxer = "mp4mux ! ";
	else if(ofFilePath(file).getExtension()=="mov") muxer = "qtmux ! ";
	else if(ofFilePath(file).getExtension()=="mkv") muxer = "matroskamux ! ";
	else if(ofFilePath(file).getExtension()=="ogg" || ofFilePath(file).getExtension()=="ogv") muxer = " oggmux ! ";

	string pay = "";
	string videorate = " video/x-raw-yuv ! videorate force-fps=" +ofToString(fps)+"/1  ! ";

	switch(codec){
	case THEORA:
		encoder = " video/x-raw-yuv ! theoraenc quality=63 ! ";
		muxer = "oggmux ! ";
		pay = "rtptheorapay pt=96 ! ";
	break;
	case H264:
#ifdef TARGET_OSX
		encoder = " video/x-raw-yuv ! vtenc_h264 ! ";
#else
		encoder = " video/x-raw-yuv ! x264enc pass=4 ! ";
#endif
		pay = "rtph264pay pt=96 ! ";
	break;
	case MP4:
		encoder = " video/x-raw-yuv ! ffenc_mpeg4 bitrate=2000000 ! ";
		pay = "rtpmp4vpay pt=96 ! ";
	break;
	case XVID:
		encoder = " video/x-raw-yuv ! xvidenc ! ";
	break;
	case JPEG:
		encoder = " video/x-raw-yuv ! jpegenc quality=98 ! ";
		pay = "rtpjpegpay pt=96 !";
		break;
	case JPEG_SEQUENCE:
		encoder = "jpegenc quality=100 idct-method=float ! ";
		pay = "";
		sink = "multifilesink name=video-sink location=" + file.substr(0,file.rfind('.')) + "%05d" + file.substr(file.rfind('.'));
		videorate = "";
		muxer = "";
	break;
	case LOSLESS_JPEG:
		encoder = " video/x-raw-yuv ! ffenc_ljpeg ! ";
	break;
	case PDF:
		//videorate = "videorate ! video/x-raw-rgb, depth=24, bpp=32, endianness= 4321, red_mask= 65280, green_mask= 16711680, blue_mask= -16777216, framerate="+ofToString(fps)+"/1 !";
		encoder = "cairorender ! ";
		muxer = "";
	break;
	case PNG_SEQUENCE:
		encoder = "pngenc snapshot=false ! ";
		sink = "multifilesink name=video-sink location=" + file.substr(0,file.rfind('.')) + "%05d" + file.substr(file.rfind('.'));
		videorate = "";
		muxer = "";
		colorspaceconversion = "";
	break;
	case QT_ANIM:
		encoder = "ffenc_qtrle ! ";
	break;
	case FLV:
		encoder = " video/x-raw-yuv ! ffenc_flv ! ";
		muxer = "flvmux ! ";
	break;
	case FLV_H264:
		encoder = " video/x-raw-yuv ! x264enc ! ";
		muxer = "flvmux ! ";
		pay = "rtph264pay pt=96 ! ";
	break;
	case YUV:
		encoder = "";
		muxer = " video/x-raw-yuv ! avimux ! ";
	break;
	case Y4M:
		encoder = " video/x-raw-yuv ! y4menc ! ";
		muxer = "";
		break;
	case DIRAC:
		encoder = "schroenc ! ";
		//muxer = "ffmux_dirac ! ";
	break;
	}


	if(bIsUdpStream){
		muxer = "";
		if(pay!=""){
			pay = "queue ! " + pay;
			muxer = muxer + pay;
		}else{
			ofLogError() << "this format doesnt support streaming, choose one of THEORA, H264, MP4, JPEG";
			return;
		}
	}else{
		pay   = "";
		if(bIsTcpStream){
			muxer = "mpegtsmux name=mux ! ";
		}
	}

	string input_mime;
	string other_format;
	if(bpp==24 || bpp==32){
		input_mime = "video/x-raw-rgb";
		if(codec==PNG_SEQUENCE){
			other_format = ",endianness=4321,red_mask=16711680,green_mask=65280,blue_mask=255,framerate="+ofToString(fps)+"/1 ";
		}else{
			other_format = ",endianness=4321,red_mask=255,green_mask=65280,blue_mask=16711680,framerate="+ofToString(fps)+"/1 ";
		}
	}
	if(bpp==8){
		input_mime = "video/x-raw-gray";
		other_format = ",framerate="+ofToString(fps)+"/1";
	}

	if(src==""){
		src="appsrc  name=video_src is-live=true do-timestamp=true ! "
				+ input_mime
				+ ", width=" + ofToString(width)
				+ ", height=" + ofToString(height)
				+ ", depth=" + ofToString(bpp)
				+ ", bpp=" + ofToString(bpp)
				+ other_format;
	}


	string pipeline_string = src + " ! " +
									"queue ! " + colorspaceconversion +
									videorate +
									encoder + muxer +
									sink;

	ofLogVerbose() << "gstreamer pipeline: " << pipeline_string;

	setPipelineWithSink(pipeline_string,"");

	gstSrc = (GstAppSrc*)gst_bin_get_by_name(GST_BIN(getPipeline()),"video_src");
	if(gstSrc){
		gst_app_src_set_stream_type (gstSrc,GST_APP_STREAM_TYPE_STREAM);
		g_object_set (G_OBJECT(gstSrc), "format", GST_FORMAT_TIME, NULL);
	}else{
		ofLogError() << "got no gstSrc";
	}
	play();

}

void ofxGstVideoRecorder::newFrame(ofPixels & pixels){
	if(codec=PNG_SEQUENCE){
		this->pixels = pixels;
		this->pixels.swapRgb();
	}else{
		this->pixels.setFromExternalPixels(pixels.getPixels(),pixels.getWidth(),pixels.getHeight(),pixels.getNumChannels());
	}
	GstBuffer * buffer;
	buffer = gst_app_buffer_new (this->pixels.getPixels(), this->pixels.size(), NULL, this->pixels.getPixels());

	GstFlowReturn flow_return = gst_app_src_push_buffer(gstSrc, buffer);
	if (flow_return != GST_FLOW_OK) {
		ofLogError() << "error pushing buffer: flow_return was " << flow_return;
	}
	//ofGstVideoUtils::update();
}
