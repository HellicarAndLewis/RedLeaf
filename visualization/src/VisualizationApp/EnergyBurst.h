/*
 * EnergyBurst.h
 *
 *  Created on: Sep 11, 2012
 *      Author: arturo
 */

#ifndef ENERGYBURST_H_
#define ENERGYBURST_H_

#include <set>
#include "ofUtils.h"
#include "ofVec2f.h"
#include "ofConstants.h"
#include "ofParameter.h"
#include "ofParameterGroup.h"
#include "LEDStrip.h"

class EnergyBurst {
public:
	static void initClassParameters();

	EnergyBurst(ofVec2f pos,  const ofColor & color=ofColor(), u_long startTime=ofGetElapsedTimeMillis())
	:startPosition(pos)
	,currentPositionL(pos)
	,currentPositionR(pos)
	,alive(true)
	,startTime(startTime)
	,lifeTime(ofRandom(minLifeTime,maxLifeTime))
	,speed(ofRandom(minSpeed,maxSpeed))
	,now(startTime)
	,color(color)
	,lCycle(false)
	,rCycle(false){}

	EnergyBurst()
	:alive(true)
	,startTime(ofGetElapsedTimeMillis())
	,lifeTime(ofRandom(500,2000))
	,speed(ofRandom(.08,.2))
	,now(startTime)
	,lCycle(false)
	,rCycle(false){}

	void update(u_long nowMS);
	bool triggeredAlready(LEDStrip & strip);
	void trigger(LEDStrip & strip);
	void draw();
	bool leftHasCycled();
	bool rightHasCycled();

	ofVec2f startPosition;
	ofVec3f currentPositionL,currentPositionR;
	bool alive;
	u_long startTime;
	u_long lifeTime;
	float speed;
	set<LEDStrip*> triggeredStrips;
	u_long now;
	ofColor color;
	static ofParameter<float> decayPct;
	static ofParameter<float> minLifeTime;
	static ofParameter<float> maxLifeTime;
	static ofParameter<float> minSpeed;
	static ofParameter<float> maxSpeed;
	static ofParameter<float> alpha;
	static ofParameter<bool> useColor;
	static ofParameterGroup parameters;

private:
	bool lCycle,rCycle;
};

#endif /* ENERGYBURST_H_ */
