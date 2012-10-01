/*
 * ComputerVision.h
 *
 *  Created on: Sep 26, 2012
 *      Author: arturo
 */

#ifndef COMPUTERVISION_H_
#define COMPUTERVISION_H_

#include "ofxCv.h"
#include "ofConstants.h"
#include "ofPixels.h"
#include "Warping.h"
#include "ofxGui.h"
#include <list>

class ComputerVision {
public:
	ComputerVision();

	void setup(string name);

	void update();
	void draw();

	void setPosition(float x, float y);
	void setSize(float w, float h);

	struct Trigger{
		Trigger(float distance, u_long time):distance(distance),time(time){}
		float distance;
		u_long time;
	};
	bool isFrameNew() const;
	const vector<float> & getTriggers() const;

	vector<ofPoint> getQuad();
	void setQuad(const vector<ofPoint> & quad);

	static ofParameter<bool> showGui;
	static ofParameter<bool> showThreshold;
	static ofParameter<bool> showContours;
	static ofParameter<int> thresholdLevel;
	static ofParameter<int> minTimeTrigger;
	static ofParameter<float> distanceSameTrigger;
	static ofParameterGroup globalParameters;

	ofParameter<bool> calibrationMode;
	ofParameter<int> currentPoint;
	ofxButton resetCalibration;
	ofxGuiGroup gui;

	void newFrame(ofPixels & pixels);

	ofEvent<float> calibrationPoint;

private:
	void calibrationModeChanged(bool & calibrationMode);
	void currentPointChanged(int & currentPoint);
	void resetCalibrationPressed(bool & pressed);

	void solveEquation();
	float calibratedDistance(float x);
	ofVec2f getCrossPoint(const ofVec2f & p);

	void mousePressed(ofMouseEventArgs & mouse);

	// frame diff buffers (triple buffering)
	ofPixels gray, prevFrame, diffFrameBack, diffFrameFront, diffFrameDraw;
	vector<cv::Rect> contoursBBsBack, contoursBBsFront, contoursBBsDraw;
	ofxCv::ContourFinder contourFinder;
	ofVec2f topHalfBack, bottomHalfBack, topHalfFront, bottomHalfFront, topHalfDraw, bottomHalfDraw;
	vector<ofVec2f> crossPointsBack,crossPointsFront,crossPointsDraw;
	vector<float> distancesBack;
	vector<float> triggersBack,triggersFront,triggersDraw;
	list<Trigger> triggersHistoric;

	// threholded image tex
	ofTexture tex;

	ofMutex mutex;
	bool newFrameBack,newFrameFront;

	// warping
	gui::Warping warp;
	ofVec2f position;
	float w,h;

	// calibration
	vector<float> srcPoints;
	vector<float> realPoints;
	float a,b,c,d;
};

#endif /* COMPUTERVISION_H_ */
