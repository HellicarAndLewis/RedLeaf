/*
 * Warping.cpp
 *
 *  Created on: 18/04/2011
 *      Author: arturo
 */

#include "Warping.h"
#include <float.h>

#include "ofGraphics.h"

namespace gui{
Warping::Warping(float minDistance)
:scaleXY(1,1)
,minDistance(minDistance)
,selectedPoint(-1)
{
	quad.resize(4);
	quad.setClosed(true);
}

void Warping::enableEvents(){
	ofRegisterMouseEvents(this);
}

void Warping::disableEvents(){
	ofUnregisterMouseEvents(this);
}

void Warping::update(){

}

void Warping::draw(){
	ofPushStyle();
	ofSetColor(0,255,0);
	quad.draw();
	for(int i=0; i<4; ++i){
		if(selectedPoint==i) ofSetColor(255,0,0);
		else ofSetColor(0,255,0);
		ofCircle(quad[i],4);
	}
	ofPopStyle();
}

void Warping::mousePressed(ofMouseEventArgs & mouse){
	float minDistance=FLT_MAX;
	int nearestPoint = -1;
	for(int i=0; i<4; ++i){
		if(quad[i].distance(ofPoint(mouse.x,mouse.y))<minDistance){
			minDistance = quad[i].distance(ofPoint(mouse.x,mouse.y));
			nearestPoint = i;
		}
	}

	if(minDistance<this->minDistance || this->minDistance==0){
		selectedPoint = nearestPoint;
	}
}

void Warping::mouseDragged(ofMouseEventArgs & mouse){
	if(selectedPoint!=-1){
		if(maxQuad.size()==4){
			ofVec2f scaledMouse(mouse.x,mouse.y);
			scaledMouse -= offset;
			scaledMouse /= scaleXY;
			if(!maxQuad.inside(scaledMouse)) return;
		}
		quad[selectedPoint].set(mouse.x,mouse.y);
	}
}

void Warping::mouseReleased(ofMouseEventArgs & mouse){
	selectedPoint=-1;
}

ofPolyline Warping::getQuad(){
	ofPolyline ret_quad;
	ret_quad.resize(4);
	ret_quad.setClosed(true);
	for(int i=0;i<4;i++){
		ret_quad[i]=(quad[i]-offset)/scaleXY;
	}
	return ret_quad;
}

ofPolyline Warping::getQuad(const ofPoint & offset, const ofVec2f & scale){
	ofPolyline ret_quad;
	ret_quad.resize(4);
	ret_quad.setClosed(true);
	for(int i=0;i<4;i++){
		ret_quad[i]=((quad[i]-this->offset)/this->scaleXY)*scale+offset;
	}
	return ret_quad;
}

void Warping::setMinDistance(float _minDistance){
	minDistance = _minDistance;
}

void Warping::setInitialQuad(const vector<ofPoint> & _quad, const ofPoint & _offset, const ofPoint & _scaleXY){
	offset = _offset;
	scaleXY = _scaleXY;
	if(_quad.size()<4){
		ofLog(OF_LOG_ERROR,"gui::Warping::setInitialQuad: error quad has less than 4 points");
		return;
	}

	for(int i=0; i<4; ++i){
		quad[i] = _quad[i] * scaleXY + offset;
	}

}

void Warping::setOffset(const ofPoint & _offset){
	for(int i=0; i<4; ++i){
		quad[i] = (quad[i] - offset + _offset);
	}
	offset = _offset;
}

void Warping::setScaleXY(const ofPoint & _scaleXY){
	for(int i=0; i<4; ++i){
		quad[i] = (quad[i] - offset) / scaleXY * _scaleXY + offset;
	}
	scaleXY = _scaleXY;

}

void Warping::setMaxQuad(const vector<ofPoint> & quad){
	maxQuad.clear();
	maxQuad.addVertices(quad);
}

}
