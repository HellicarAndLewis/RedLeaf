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
ofParameter<int> TweetText::limitScreenX("limitScreenX",0,0,200);
ofParameter<int> TweetText::fontSize("fontSize",20,10,100);
string TweetText::fontName("Anonymous.ttf");
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
	color = triggerColor;
}

void TweetText::setAmplitude(float amp){
	color *= amp;
}

void TweetText::draw(){
	ofColor c = ofGetStyle().color;
	ofSetColor(color);
	font.drawString(tweet,position.x,position.y);
	ofSetColor(c);
}

bool TweetText::isAlive(){
	return font.getStringBoundingBox(tweet,position.x,position.y).getMaxX()>limitScreenX;
}
