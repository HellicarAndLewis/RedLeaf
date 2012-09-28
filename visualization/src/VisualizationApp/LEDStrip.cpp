/*
 * LEDStrip.cpp
 *
 *  Created on: Sep 11, 2012
 *      Author: arturo
 */

#include "LEDStrip.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofxTween.h"

ofParameter<int> LEDStrip::fadeTime("fadeTime",200,0,2000);
ofParameterGroup LEDStrip::parameters;

void LEDStrip::initClassParameters(){
	parameters.setName("LEDStrip");
	parameters.add(fadeTime);
}

void LEDStrip::setup(u_int _h, float _position,float _radius){
	position = _position;
	h = _h;
	radius = _radius;
	color.set(0,0,0);
	triggerColor = color;
	triggerTime = 0;
	testMode = false;
	stripMesh.getVertices().resize(h);
	for(int i=0;i<stripMesh.getNumVertices();i++){
		stripMesh.getVertices()[i].set(0,i);
	}
	stripMesh.setMode(OF_PRIMITIVE_POINTS);
}

void LEDStrip::trigger(const ofColor & rgb, u_long now){
	if(testMode){
		color = rgb;
	}else{
		color.r = max(color.r,rgb.r);
		color.g = max(color.g,rgb.g);
		color.b = max(color.b,rgb.b);
	}
	triggerColor = color;
	triggerTime = now;
}

void LEDStrip::update(u_long now){
	if(testMode) return;
	float t = float(now-triggerTime)/float(fadeTime);
	ofxEasingSine easing;
	color.set(ofxTween::map(t,0,1,triggerColor.r,0,true,easing),
				ofxTween::map(t,0,1,triggerColor.g,0,true,easing),
				ofxTween::map(t,0,1,triggerColor.b,0,true,easing));
}

void LEDStrip::draw(float x, float radiusScale, float renderW, float renderH){
	ofColor c = ofGetStyle().color;
	ofSetColor(color);
	float sep = renderH/float(h+1);
	glPointSize(2*radius*renderH*radiusScale);
	ofPushMatrix();
	ofTranslate(x*renderW,sep);
	ofScale(1,sep);
	stripMesh.draw();
	ofPopMatrix();
	ofSetColor(c);
}


float LEDStrip::getPosition(){
	return position;
}

const ofColor & LEDStrip::getColor() const{
	return color;
}

void LEDStrip::setTestMode(bool _testMode){
	testMode = _testMode;
}

void LEDStrip::setAmplitude(float amp){
	color *= amp;
}
