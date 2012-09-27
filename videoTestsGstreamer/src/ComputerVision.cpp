/*
 * ComputerVision.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: arturo
 */

#include "ComputerVision.h"
using namespace ofxCv;

ofParameter<int> ComputerVision::thresholdLevel("thresholdLevel",30,0,255);
ofParameter<bool> ComputerVision::showGui("showGui",false);
ofParameter<bool> ComputerVision::showImages("showImages",false);
ofParameter<bool> ComputerVision::showContours("showContours",false);
ofParameter<int> ComputerVision::minTimeTrigger("minTimeTrigger",300,0,2000);
ofParameter<float> ComputerVision::distanceSameTrigger("distanceSameTrigger",.1,0,.5);
ofParameterGroup ComputerVision::parameters;

ComputerVision::ComputerVision() {
	newFrameBack=false;
	newFrameFront=false;
	w=640,h=480;

	contourFinder.setAutoThreshold(false);
	vector<ofVec3f> quad(4);
	quad[0].set(0.1,0.1);
	quad[1].set(0.9,0.1);
	quad[2].set(0.9,0.9);
	quad[3].set(0.1,0.9);
	warp.setInitialQuad(quad,ofVec3f(0,0),ofVec3f(1,1));
	quad[0].set(0,0);
	quad[1].set(1,0);
	quad[2].set(1,1);
	quad[3].set(0,1);
	warp.setMaxQuad(quad);
	warp.setMinDistance(6);
	warp.enableEvents();
	/*flow.setQualityLevel(0.04);
	flow.setMinDistance(1);
	flow.setMaxFeatures(60);
	flow.setWindowSize(3);*/
}

void ComputerVision::newFrame(ofPixels & pixels){
	u_long now = ofGetElapsedTimeMillis();
	convertColor(pixels,gray,CV_RGB2GRAY);

	//flow.calcOpticalFlow(gray);
	if(prevFrame.isAllocated()){
		absdiff(gray,prevFrame,diffFrameBack);
		//blur(diffFrame,5);
		threshold(diffFrameBack,thresholdLevel);
		contourFinder.findContours(diffFrameBack);
	}
	prevFrame = gray;

	contoursBBsBack.clear();
	ofPolyline quad = warp.getQuad(ofVec2f(0,0),ofVec2f(diffFrameBack.getWidth(), diffFrameBack.getHeight()));
	topHalfBack = quad[0]+(quad[1]-quad[0])*.5;
	bottomHalfBack = quad[3]+(quad[2]-quad[3])*.5;
	float totalDistance = topHalfBack.distance(bottomHalfBack);
	float x1 = topHalfBack.x;
	float y1 = topHalfBack.y;
	float x2 = bottomHalfBack.x;
	float y2 = bottomHalfBack.y;
	crossPointsBack.clear();
	distancesBack.clear();
	triggersBack.clear();

	for(u_int i=0;i<contourFinder.getBoundingRects().size();i++){
		if(quad.inside(toOf(contourFinder.getCentroid(i)))){
			contoursBBsBack.push_back(contourFinder.getBoundingRect(i));
			float x3= contourFinder.getCentroid(i).x;
			float y3= contourFinder.getCentroid(i).y;
			float k = ((y2-y1) * (x3-x1) - (x2-x1) * (y3-y1)) / ((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1));
			ofVec2f crossPoint(x3 - k * (y2-y1), y3 + k * (x2-x1));
			crossPointsBack.push_back(crossPoint);
			distancesBack.push_back(bottomHalfBack.distance(crossPoint)/totalDistance);
		}
	}
	vector<list<Trigger>::iterator> toRemove;
	for(list<Trigger>::iterator it=triggersHistoric.begin();it!=triggersHistoric.end();it++){
		if(now-it->time>minTimeTrigger){
			toRemove.push_back(it);
		}
	}
	for(u_int i=0;i<toRemove.size();i++){
		triggersHistoric.erase(toRemove[i]);
	}
	for(u_int i=0;i<distancesBack.size();i++){
		bool insert = true;
		for(list<Trigger>::iterator it=triggersHistoric.begin();it!=triggersHistoric.end();it++){
			if(abs(it->distance-distancesBack[i])<=distanceSameTrigger){
				insert = false;
				break;
			}
		}
		if(insert){
			triggersHistoric.push_back(Trigger(distancesBack[i],now));
			triggersBack.push_back(distancesBack[i]);
		}
	}
	/*featuresBack = flow.getFeatures();
	motionBack = flow.getMotion();*/
	mutex.lock();
	swap(diffFrameBack,diffFrameFront);
	swap(contoursBBsBack,contoursBBsFront);
	swap(crossPointsBack,crossPointsFront);
	swap(triggersBack,triggersFront);
	topHalfFront = topHalfBack;
	bottomHalfFront = bottomHalfBack;
	newFrameBack=true;
	mutex.unlock();
}


bool ComputerVision::isFrameNew() const{
	return newFrameFront;
}

void ComputerVision::update(){
	mutex.lock();
	newFrameFront = newFrameBack;
	if(newFrameBack){
		swap(diffFrameFront,diffFrameDraw);
		swap(contoursBBsFront,contoursBBsDraw);
		swap(crossPointsFront,crossPointsDraw);
		swap(triggersFront,triggersDraw);
		topHalfDraw = topHalfFront;
		bottomHalfDraw = bottomHalfFront;
		newFrameBack = false;
		/*swap(featuresBack,featuresDraw);
		swap(motionBack,motionDraw);*/
	}
	mutex.unlock();
	warp.update();
}

void ComputerVision::setPosition(float x, float y){
	position.set(x,y);
	warp.setOffset(position);
}

void ComputerVision::setSize(float _w, float _h){
	w = _w;
	h = _h;
	warp.setScaleXY(ofVec2f(_w,_h));
}

void ComputerVision::draw(){
	ofPushStyle();
	if(showImages){
		if(newFrameFront && diffFrameDraw.size()!=0){
			if(!tex.isAllocated() || tex.getWidth()!=diffFrameDraw.getWidth() || tex.getHeight()!=diffFrameDraw.getHeight()){
				tex.allocate(diffFrameDraw);
			}
			tex.loadData(diffFrameDraw);
		}
	}
	if(showImages || showContours){
		ofPushMatrix();
		ofTranslate(position);
		ofScale(w/diffFrameDraw.getWidth(),h/diffFrameDraw.getHeight());
		ofPushStyle();
		ofSetColor(255);
		if(showImages) tex.draw(0,0);
		if(showContours){
			ofNoFill();
			ofSetColor(0,255,0);
			for(u_int i=0;i<contoursBBsDraw.size();i++){
				ofRect(toOf(contoursBBsDraw[i]));
			}
			ofLine(bottomHalfDraw,topHalfDraw);
			ofFill();
			ofSetColor(255,0,0);
			for(u_int i=0;i<crossPointsDraw.size();i++){
				ofCircle(crossPointsDraw[i],3);
			}
		}
		/*for(u_int i=0; i<featuresDraw.size();i++){
			ofCircle(featuresDraw[i],3);
			ofLine(featuresDraw[i],featuresDraw[i]+motionDraw[i]);
		}*/
		ofPopStyle();
		ofPopMatrix();
	}

	if(showGui){
		warp.draw();
	}
	ofPopStyle();
}

const vector<float> & ComputerVision::getTriggers() const{
	return triggersFront;
}
