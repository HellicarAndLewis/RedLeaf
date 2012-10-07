/*
 * Logo.cpp
 *
 *  Created on: Oct 7, 2012
 *      Author: arturo
 */

#include "Logo.h"

/*
 * TweetText.cpp
 *
 *  Created on: Sep 28, 2012
 *      Author: arturo
 */

#include "TweetText.h"
#include "ofAppRunner.h"
#include "ofGraphics.h"

ofParameter<float> Logo::speedPixelsPerSec("speedPixelsPerSec",50,1,1000);
ofParameter<int> Logo::y("y",0,0,200);
ofParameter<float> Logo::xScale("xScale",.5,0.1,3);
ofParameter<float> Logo::yScale("yScale",.5,0.1,3);
ofParameter<int> Logo::limitScreenX("limitScreenX",0,0,200);
ofParameter<float> Logo::alpha("alpha",1,0,1);
ofParameter<float> Logo::separation("separation",10,0,200);
ofParameterGroup Logo::parameters;
ofImage Logo::logo;


void Logo::initParameters(){
	logo.loadImage("logo.png");
	parameters.setName("Logo");
	parameters.add(speedPixelsPerSec);
	parameters.add(y);
	parameters.add(xScale);
	parameters.add(yScale);
	parameters.add(alpha);
	parameters.add(separation);
}

Logo::Logo(u_long now, int x)
:position(x,y)
,time(now) {
}

void Logo::update(){
	color.a = 255;
	position.x-=ofGetLastFrameTime()*speedPixelsPerSec;
	position.y = y;
}

void Logo::setAmplitude(float amp){
	color.a *= amp;
}

void Logo::draw(){
	ofColor c = ofGetStyle().color;
	ofSetColor(color);
	ofPushMatrix();
	ofScale(xScale,yScale,1);
	logo.draw(position);
	ofPopMatrix();
	ofSetColor(c);
}

bool Logo::isAlive(){
	return position.x+logo.getWidth()*xScale>limitScreenX;
}
