/*
 * ofxAxisGrabber.h
 *
 *  Created on: Sep 23, 2012
 *      Author: arturo
 */

#ifndef OFXAXISGRABBER_H_
#define OFXAXISGRABBER_H_

#include "ofBaseTypes.h"
#include "ofVideoPlayer.h"
#include "ofParameter.h"
#include "ofParameterGroup.h"
#include "ofxHttpUtils.h"

class ofxAxisGrabber: public ofBaseVideoGrabber {
public:
	ofxAxisGrabber();

	//overwritten from ofBaseVideoGrabber
	void	listDevices();
	bool	initGrabber(int w, int h);
	void	update();
	bool	isFrameNew();

	unsigned char 	* getPixels();
	ofPixels_<unsigned char> & getPixelsRef();

	void	close();

	float	getHeight();
	float	getWidth();

	bool setPixelFormat(ofPixelFormat pixelFormat);
	ofPixelFormat getPixelFormat();

	void setVerbose(bool bTalkToMe);
	void setDeviceID(int _deviceID);
	void setDesiredFrameRate(int framerate);
	void videoSettings();


	// axisgrabber only
	enum AxisCodec{
		MJPG,
		H264
	};
	void triggerAutoFocus();
	void setAuth(string user, string pwd); // without authentication parameter changing won't work
	void setParametersRefreshRate(int ms);  //default 0 no refresh
	void setCodec(AxisCodec codec);
	void setCompression(int compression=30);
	void setRecording(bool recording);
	void setCameraAddress(string address);
	string getCameraAddress();
	ofRectangle getFocusWindow();
	void setFocusWindow(const ofRectangle & focusWindow);

	ofParameter<int> focus;
	ofParameter<int> focusMeasure;
	ofParameter<bool> manualIris;
	ofParameter<float> fps;
	ofParameterGroup parameters;

private:
	void focusChanged(int & focus);
	void requestFocusWindow();
	void setManualIris(bool & manual);
	void setFocusWindow();

	ofGstVideoUtils gst;
	int prevFocus;
	bool updating;

	ofxHttpUtils http;

	int paramRefreshRateMs;
	u_long lastTimeFocusQueriedMs;
	ofRectangle focusWindow;
	ofRectangle focusWindowScaled;

	int startTimeOneSecMicros;
	int framesInOneSec;
	string user,pwd;
	int desiredFramerate;
	AxisCodec codec;
	int compression;
	bool recording;

	string cameraAddress;
};

#endif /* OFXAXISGRABBER_H_ */
