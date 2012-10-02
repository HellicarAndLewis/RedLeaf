/*
 * EnergyBurst.cpp
 *
 *  Created on: Sep 11, 2012
 *      Author: arturo
 */

#include "EnergyBurst.h"
#include "ofAppRunner.h"
#include "ofGraphics.h"

ofParameter<float> EnergyBurst::decayPct("decayPct",.8,0,1);
ofParameter<float> EnergyBurst::minLifeTime("minLifeTime",500,0,2000);
ofParameter<float> EnergyBurst::maxLifeTime("maxLifeTime",2000,500,4000);
ofParameter<float> EnergyBurst::minSpeed("minSpeed",.08,0.01,1);
ofParameter<float> EnergyBurst::maxSpeed("maxSpeed",.2,0.01,1);
ofParameter<float> EnergyBurst::alpha("alpha",1,0,1);
ofParameter<bool> EnergyBurst::useColor("useColor",false);
ofParameterGroup EnergyBurst::parameters;

void EnergyBurst::initClassParameters(){
	parameters.setName("EnergyBurst");
	parameters.add(decayPct);
	parameters.add(minLifeTime);
	parameters.add(maxLifeTime);
	parameters.add(minSpeed);
	parameters.add(maxSpeed);
	parameters.add(useColor);
	parameters.add(alpha);
}

void EnergyBurst::update(u_long nowMS){
	now = nowMS;
	if(nowMS>startTime+lifeTime){
		alive=false;
		return;
	}
	currentPositionL.set(startPosition.x - speed*float(nowMS-startTime)/1000.,startPosition.y);
	currentPositionR.set(startPosition.x + speed*float(nowMS-startTime)/1000.,startPosition.y);
	if(currentPositionL.x<0){
		lCycle=true;
		currentPositionL.x+=1;
	}
	if(currentPositionR.x>1){
		rCycle=true;
		currentPositionR.x-=1;
	}

	color.a = 255. * alpha;
}


bool EnergyBurst::triggeredAlready(LEDStrip & strip){
	return triggeredStrips.find(&strip)!=triggeredStrips.end();
}

void EnergyBurst::trigger(LEDStrip & strip){
	ofColor color = this->color;
	float lifePct = float(now-startTime)/float(lifeTime);
	if(lifePct>=decayPct){
		color *= ofMap(lifePct,decayPct,1,1,0);
	}
	strip.trigger(color,now);
	triggeredStrips.insert(&strip);
}

void EnergyBurst::draw(){
	ofCircle(startPosition*ofVec2f(ofGetWidth(),ofGetHeight()),3);
	if(!lCycle){
		ofLine(startPosition*ofVec2f(ofGetWidth(),ofGetHeight()),currentPositionL*ofVec2f(ofGetWidth(),ofGetHeight()));
	}else{
		ofLine(startPosition*ofVec2f(ofGetWidth(),ofGetHeight()),ofVec2f(0,ofGetHeight()*.5));
		ofLine(currentPositionL*ofVec2f(ofGetWidth(),ofGetHeight()),ofVec2f(ofGetWidth(),ofGetHeight()*.5));
	}
	if(!rCycle){
		ofLine(startPosition*ofVec2f(ofGetWidth(),ofGetHeight()),currentPositionR*ofVec2f(ofGetWidth(),ofGetHeight()));
	}else{
		ofLine(startPosition*ofVec2f(ofGetWidth(),ofGetHeight()),ofVec2f(ofGetWidth(),ofGetHeight()*.5));
		ofLine(ofVec2f(0,ofGetHeight()*.5),currentPositionR*ofVec2f(ofGetWidth(),ofGetHeight()));
	}
}

bool EnergyBurst::leftHasCycled(){
	return lCycle;
}

bool EnergyBurst::rightHasCycled(){
	return rCycle;
}
