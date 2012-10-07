/*
 * TweetText.cpp
 *
 *  Created on: Sep 28, 2012
 *      Author: arturo
 */

#include "TweetText.h"
#include "ofAppRunner.h"
#include "ofGraphics.h"

ofParameter<float> TweetText::speedPixelsPerSec("speedPixelsPerSec",50,1,1000);
ofParameter<int> TweetText::y("y",50,0,200);
ofParameter<float> TweetText::xScale("xScale",1,0.1,3);
ofParameter<int> TweetText::limitScreenX("limitScreenX",0,0,200);
ofParameter<int> TweetText::fontSize("fontSize",20,10,100);
ofParameter<bool> TweetText::useColors("useColors",true);
ofParameter<float> TweetText::alpha("alpha",1,0,1);
string TweetText::fontName("telegrama_render.otf");
ofTrueTypeFont TweetText::font;

TweetText::TweetText(const string & tweet, const ofColor & color, u_long now, int x)
:tweet(tweet)
,triggerColor(color)
,color(color)
,time(now)
,position(x,y) {
}

void TweetText::update(){
	position.x-=ofGetLastFrameTime()*speedPixelsPerSec;
	position.y = y;
	triggerColor.a = 255.*alpha;
	color = triggerColor;
}

void TweetText::setAmplitude(float amp){
	color.a *= amp;
}

void TweetText::draw(){
	ofColor c = ofGetStyle().color;
	ofSetColor(color);
	ofPushMatrix();
	ofScale(xScale,1,1);
	font.drawString(tweet,position.x,position.y);
	ofPopMatrix();
	ofSetColor(c);
}

bool TweetText::isAlive(){
	return font.getStringBoundingBox(tweet,position.x,position.y).getMaxX()>limitScreenX;
}
