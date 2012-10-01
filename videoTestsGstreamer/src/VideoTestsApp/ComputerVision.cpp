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
ofParameter<bool> ComputerVision::showThreshold("showThreshold",false);
ofParameter<bool> ComputerVision::showContours("showContours",false);
ofParameter<int> ComputerVision::minTimeTrigger("minTimeTrigger",300,0,2000);
ofParameter<float> ComputerVision::distanceSameTrigger("distanceSameTrigger",.1,0,.5);
ofParameterGroup ComputerVision::globalParameters;


static void gaussian_elimination(float *input, int n){
	// ported to c from pseudocode in
	// http://en.wikipedia.org/wiki/Gaussian_elimination

	float * A = input;
	int i = 0;
	int j = 0;
	int m = n-1;
	while (i < m && j < n){
	  // Find pivot in column j, starting in row i:
	  int maxi = i;
	  for(int k = i+1; k<m; k++){
	    if(fabs(A[k*n+j]) > fabs(A[maxi*n+j])){
	      maxi = k;
	    }
	  }
	  if (A[maxi*n+j] != 0){
	    //swap rows i and maxi, but do not change the value of i
		if(i!=maxi)
		for(int k=0;k<n;k++){
			float aux = A[i*n+k];
			A[i*n+k]=A[maxi*n+k];
			A[maxi*n+k]=aux;
		}
	    //Now A[i,j] will contain the old value of A[maxi,j].
	    //divide each entry in row i by A[i,j]
		float A_ij=A[i*n+j];
		for(int k=0;k<n;k++){
			A[i*n+k]/=A_ij;
		}
	    //Now A[i,j] will have the value 1.
	    for(int u = i+1; u< m; u++){
    		//subtract A[u,j] * row i from row u
	    	float A_uj = A[u*n+j];
	    	for(int k=0;k<n;k++){
	    		A[u*n+k]-=A_uj*A[i*n+k];
	    	}
	      //Now A[u,j] will be 0, since A[u,j] - A[i,j] * A[u,j] = A[u,j] - 1 * A[u,j] = 0.
	    }

	    i++;
	  }
	  j++;
	}

	//back substitution
	for(int i=m-2;i>=0;i--){
		for(int j=i+1;j<n-1;j++){
			A[i*n+m]-=A[i*n+j]*A[j*n+m];
			A[i*n+j]=0;
		}
	}
}


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

	srcPoints.resize(4);
	for(u_int i=0;i<srcPoints.size();i++){
		srcPoints[i]=.20*(i+1);
	}
	realPoints = srcPoints;
	currentPoint = 0;
	solveEquation();
}

void ComputerVision::setup(string name){
	gui.setup(name);
	gui.add(calibrationMode.set("calibrationMode",false));
	gui.add(currentPoint.set("calibrationPoint",0,0,3));
	gui.add(resetCalibration.setup("resetCalibration"));

	calibrationMode.addListener(this,&ComputerVision::calibrationModeChanged);
	currentPoint.addListener(this,&ComputerVision::currentPointChanged);
	resetCalibration.addListener(this,&ComputerVision::resetCalibrationPressed);
}


void ComputerVision::calibrationModeChanged(bool & calibrationMode){
	currentPoint = 0;
	ofNotifyEvent(calibrationPoint,srcPoints[currentPoint],this);
	if(calibrationMode) ofAddListener(ofEvents().mousePressed,this,&ComputerVision::mousePressed);
	else ofRemoveListener(ofEvents().mousePressed,this,&ComputerVision::mousePressed);
}

void ComputerVision::currentPointChanged(int & currentPoint){
	solveEquation();
	ofNotifyEvent(calibrationPoint,srcPoints[currentPoint],this);
}

void ComputerVision::resetCalibrationPressed(bool & pressed){
	if(!pressed) realPoints = srcPoints;
}

void ComputerVision::solveEquation(){
	float equations[4][5] = {
			{pow(realPoints[0],3),pow(realPoints[0],2),realPoints[0],1,srcPoints[0]},
			{pow(realPoints[1],3),pow(realPoints[1],2),realPoints[1],1,srcPoints[1]},
			{pow(realPoints[2],3),pow(realPoints[2],2),realPoints[2],1,srcPoints[2]},
			{pow(realPoints[3],3),pow(realPoints[3],2),realPoints[3],1,srcPoints[3]}
	};

	gaussian_elimination(&equations[0][0],5);

	a = equations[0][4];
	b = equations[1][4];
	c = equations[2][4];
	d = equations[3][4];
}

float ComputerVision::calibratedDistance(float x){
	float x2 = x*x;
	float x3 = x2*x;
	return a*x3 + b*x2 + c*x + d;
}

ofVec2f ComputerVision::getCrossPoint(const ofVec2f & p){
	float x1 = topHalfBack.x;
	float y1 = topHalfBack.y;
	float x2 = bottomHalfBack.x;
	float y2 = bottomHalfBack.y;
	float x3 = p.x;
	float y3 = p.y;
	float k = ((y2-y1) * (x3-x1) - (x2-x1) * (y3-y1)) / ((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1));
	return ofVec2f(x3 - k * (y2-y1), y3 + k * (x2-x1));
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

	ofPolyline quad = warp.getQuad(ofVec2f(0,0),ofVec2f(diffFrameBack.getWidth(), diffFrameBack.getHeight()));
	topHalfBack = quad[0]+(quad[1]-quad[0])*.5;
	bottomHalfBack = quad[3]+(quad[2]-quad[3])*.5;
	float totalDistance = topHalfBack.distance(bottomHalfBack);
	contoursBBsBack.clear();
	crossPointsBack.clear();
	distancesBack.clear();
	triggersBack.clear();

	for(u_int i=0;i<contourFinder.getBoundingRects().size();i++){
		if(quad.inside(toOf(contourFinder.getCentroid(i)))){
			contoursBBsBack.push_back(contourFinder.getBoundingRect(i));
			ofVec2f crossPoint = getCrossPoint(toOf(contourFinder.getCentroid(i)));
			crossPointsBack.push_back(crossPoint);
			distancesBack.push_back(calibratedDistance(bottomHalfBack.distance(crossPoint)/totalDistance));
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
	if(showThreshold){
		if(newFrameFront && diffFrameDraw.size()!=0){
			if(!tex.isAllocated() || tex.getWidth()!=diffFrameDraw.getWidth() || tex.getHeight()!=diffFrameDraw.getHeight()){
				tex.allocate(diffFrameDraw);
			}
			tex.loadData(diffFrameDraw);
		}
	}
	if(showThreshold || showContours){
		ofPushMatrix();
		ofTranslate(position);
		ofScale(w/diffFrameDraw.getWidth(),h/diffFrameDraw.getHeight());
		ofPushStyle();
		ofSetColor(255);
		if(showThreshold) tex.draw(0,0);
		if(showContours){
			ofNoFill();
			ofSetColor(0,255,0);
			for(u_int i=0;i<contoursBBsDraw.size();i++){
				ofRect(toOf(contoursBBsDraw[i]));
			}
			ofLine(bottomHalfDraw,topHalfDraw);
			ofFill();
			/*ofSetColor(255,0,0);
			for(u_int i=0;i<crossPointsDraw.size();i++){
				ofCircle(crossPointsDraw[i],3);
			}*/

			ofVec2f dir = (topHalfDraw-bottomHalfDraw);
			ofSetColor(255,0,0);
			for(u_int i=0;i<triggersDraw.size();i++){
				ofCircle((bottomHalfDraw+dir*triggersDraw[i]),3);
			}
		}
		ofPopStyle();
		ofPopMatrix();
	}

	if(showGui){
		warp.draw();
	}

	if(calibrationMode){
		ofVec2f scale(w/diffFrameDraw.getWidth(),h/diffFrameDraw.getHeight());
		ofPushMatrix();
		ofTranslate(position);
		ofSetColor(0,255,0);
		ofLine(bottomHalfDraw*scale,topHalfDraw*scale);
		ofVec2f dir = (topHalfDraw-bottomHalfDraw);
		ofSetRectMode(OF_RECTMODE_CENTER);
		for(u_int i=0;i<srcPoints.size();i++){
			ofFill();
			if(currentPoint!=i) ofSetColor(0,255,0);
			else ofSetColor(255,0,0);
			ofCircle((bottomHalfDraw+dir*srcPoints[i])*scale,3);
			ofNoFill();
			ofSetColor(0,0,255);
			ofRect((bottomHalfDraw+dir*realPoints[i])*scale,6,6);
		}
		ofPopMatrix();
	}
	ofPopStyle();
}

const vector<float> & ComputerVision::getTriggers() const{
	return triggersFront;
}


vector<ofPoint> ComputerVision::getQuad(){
	return warp.getQuad(ofVec2f(0,0),ofVec2f(1,1)).getVertices();
}

void ComputerVision::setQuad(const vector<ofPoint> & quad){
	warp.setInitialQuad(quad,position,ofVec2f(w,h));
}

void ComputerVision::mousePressed(ofMouseEventArgs & mouse){
	if(calibrationMode){
		ofVec2f mousePos(mouse.x,mouse.y);
		mousePos -= position;
		mousePos /= ofVec2f(w,h);
		if(warp.getQuad().inside(mousePos)){
			mousePos *= ofVec2f(diffFrameBack.getWidth(),diffFrameDraw.getHeight());
			ofVec2f crossPoint = getCrossPoint(mousePos);
			float totalDistance = topHalfDraw.distance(bottomHalfDraw);
			realPoints[currentPoint] = bottomHalfDraw.distance(crossPoint)/totalDistance;
		}
	}
}
