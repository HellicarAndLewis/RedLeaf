/*
 * ComputerVision.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: arturo
 */

#include "ComputerVision.h"
using namespace ofxCv;

ofParameter<int> ComputerVision::thresholdLevel("thresholdLevel",30,0,255);

ComputerVision::ComputerVision() {
	newFrameBack=false;
	newFrameFront=false;

	contourFinder.setAutoThreshold(false);
}

void ComputerVision::newFrame(ofPixels & pixels){
	convertColor(pixels,gray,CV_RGB2GRAY);
	if(prevFrame.isAllocated()){
		absdiff(gray,prevFrame,diffFrameBack);
		//blur(diffFrame,5);
		threshold(diffFrameBack,thresholdLevel);
		contourFinder.findContours(diffFrameBack);
	}
	prevFrame = gray;

	mutex.lock();
	swap(diffFrameBack,diffFrameFront);
	contoursBBs = contourFinder.getBoundingRects();
	newFrameBack=true;
	mutex.unlock();
}

void ComputerVision::update(){
	mutex.lock();
	newFrameFront = newFrameBack;
	if(newFrameBack){
		swap(diffFrameFront,diffFrameDraw);
		swap(contoursBBs,contoursBBsDraw);
		newFrameBack = false;
	}
	mutex.unlock();
}

void ComputerVision::draw(float x, float y, float w, float h){
	if(newFrameFront){
		if(!tex.isAllocated() || tex.getWidth()!=diffFrameDraw.getWidth() || tex.getHeight()!=diffFrameDraw.getHeight()){
			tex.allocate(diffFrameDraw);
		}
		tex.loadData(diffFrameDraw);
	}

	if(diffFrameDraw.size()==0) return;
	ofPushMatrix();
	ofTranslate(x,y);
	ofScale(w/diffFrameDraw.getWidth(),h/diffFrameDraw.getHeight());
	ofPushStyle();
	tex.draw(0,0);
	ofNoFill();
	ofSetColor(0,255,0);
	for(u_int i=0;i<contoursBBsDraw.size();i++){
		ofRect(toOf(contoursBBsDraw[i]));
	}
	ofPopStyle();
	ofPopMatrix();
}
