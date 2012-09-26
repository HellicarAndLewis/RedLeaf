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

class ComputerVision {
public:
	ComputerVision();

	void newFrame(ofPixels & pixels);

	void update();
	void draw(float x, float y, float w, float h);

	static ofParameter<int> thresholdLevel;


private:
	ofPixels gray, prevFrame, diffFrameBack, diffFrameFront, diffFrameDraw;
	vector<cv::Rect> contoursBBs, contoursBBsDraw;
	ofTexture tex;
	ofxCv::ContourFinder contourFinder;
	ofMutex mutex;
	bool newFrameBack,newFrameFront;
};

#endif /* COMPUTERVISION_H_ */
