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

ofParameter<int> LEDStrip::fadeTime("LED_fadeTime",200,0,2000);

void LEDStrip::setup(u_int _h, float _position,float _radius){
	position = _position;
	h = _h;
	radius = _radius;
	color.set(0,0,0);
	triggerColor = color;
	triggerTime = 0;
}

void LEDStrip::trigger(const ofColor & rgb, u_long now){
	color.r = max(color.r,rgb.r);
	color.g = max(color.g,rgb.g);
	color.b = max(color.b,rgb.b);
	triggerColor = color;
	triggerTime = now;
}

void LEDStrip::update(u_long now){
	float t = float(now-triggerTime)/float(fadeTime);
	ofxEasingSine easing;
	color.set(ofxTween::map(t,0,1,triggerColor.r,0,true,easing),ofxTween::map(t,0,1,triggerColor.g,0,true,easing),ofxTween::map(t,0,1,triggerColor.b,0,true,easing));
}

void LEDStrip::draw(float x, float radiusScale, float renderW, float renderH){
	ofColor c = ofGetStyle().color;
	ofSetColor(color);
	float sep = renderH/float(h+1);
	for(u_int y=0; y<h; y++){
		ofCircle(x*renderW,(y+1)*sep,radius*renderH*radiusScale);
	}
	ofSetColor(c);
}


float LEDStrip::getPosition(){
	return position;
}
