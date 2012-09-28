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
#include <list>

class ComputerVision {
public:
	ComputerVision();

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
	static ofParameter<bool> showImages;
	static ofParameter<bool> showContours;
	static ofParameter<int> thresholdLevel;
	static ofParameter<int> minTimeTrigger;
	static ofParameter<float> distanceSameTrigger;
	static ofParameterGroup parameters;

	void newFrame(ofPixels & pixels);

private:
	ofPixels gray, prevFrame, diffFrameBack, diffFrameFront, diffFrameDraw;
	vector<cv::Rect> contoursBBsBack, contoursBBsFront, contoursBBsDraw;
	ofxCv::ContourFinder contourFinder;
	ofVec2f topHalfBack, bottomHalfBack, topHalfFront, bottomHalfFront, topHalfDraw, bottomHalfDraw;
	vector<ofVec2f> crossPointsBack,crossPointsFront,crossPointsDraw;
	vector<float> distancesBack;
	vector<float> triggersBack,triggersFront,triggersDraw;
	list<Trigger> triggersHistoric;

	ofTexture tex;

	//vector<ofPoint> featuresBack, featuresDraw;
	//vector<ofVec2f> motionBack, motionDraw;
	//ofxCv::FlowPyrLK flow;

	ofMutex mutex;
	bool newFrameBack,newFrameFront;

	gui::Warping warp;
	ofVec2f position;
	float w,h;
};

#endif /* COMPUTERVISION_H_ */
