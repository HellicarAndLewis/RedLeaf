/*
 * EnergyBurst.cpp
 *
 *  Created on: Sep 11, 2012
 *      Author: arturo
 */

#include "EnergyBurst.h"
#include "ofAppRunner.h"

ofParameter<float> EnergyBurst::decayPct("Burst_decayPct",.8,0,1);
ofParameter<float> EnergyBurst::minLifeTime("Burst_minLifeTime",500,0,2000);
ofParameter<float> EnergyBurst::maxLifeTime("Burst_maxLifeTime",2000,500,4000);
ofParameter<float> EnergyBurst::minSpeed("Burst_minSpeed",.08,0.01,1);
ofParameter<float> EnergyBurst::maxSpeed("Burst_maxSpeed",.2,0.01,1);

void EnergyBurst::update(u_long nowMS){
	now = nowMS;
	if(nowMS>startTime+lifeTime){
		alive=false;
		return;
	}
	currentPositionL.set(startPosition.x - speed*float(nowMS-startTime)/1000.,startPosition.y);
	currentPositionR.set(startPosition.x + speed*float(nowMS-startTime)/1000.,startPosition.y);
	if(currentPositionL.x<0 && currentPositionR.x>1) alive=false;
}


bool EnergyBurst::triggeredAlready(LEDStrip & strip){
	return triggeredStrips.find(&strip)!=triggeredStrips.end();
}

void EnergyBurst::trigger(LEDStrip & strip){
	ofColor color = ofColor::white;
	float lifePct = float(now-startTime)/float(lifeTime);
	if(lifePct>=decayPct){
		color *= ofMap(lifePct,decayPct,1,1,0);
	}
	strip.trigger(color,now);
	triggeredStrips.insert(&strip);
}
