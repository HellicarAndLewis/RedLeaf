/*
 * Wall.cpp
 *
 *  Created on: Sep 11, 2012
 *      Author: arturo
 */

#include "Wall.h"
#include "ofAppRunner.h"

void Wall::setup(u_int w, u_int h){
	strips.resize(w);
	float stripRadius = 1./float((strips.size()+1)*2.);
	float stripSize = stripRadius*2;
	for(u_int i=0;i<strips.size();i++){
		strips[i].setup(h,(i+1)*stripSize,stripRadius);
	}
}

void Wall::update(){
	u_long now = ofGetElapsedTimeMillis();
	vector<list<EnergyBurst>::iterator > toDelete;
	list<EnergyBurst>::iterator it;
	for(it = bursts.begin(); it!=bursts.end(); ++it){
		EnergyBurst & burst = *it;
		burst.update(now);
		if(!burst.alive){
			toDelete.push_back(it);
		}else{
			for(u_int i=0;i<strips.size();++i){
				if(!burst.triggeredAlready(strips[i])){
					if(strips[i].getPosition()<burst.currentPositionR.x && strips[i].getPosition()>=burst.startPosition.x)
						burst.trigger(strips[i]);
					if(strips[i].getPosition()>burst.currentPositionL.x && strips[i].getPosition()<=burst.startPosition.x)
						burst.trigger(strips[i]);
				}
			}
		}
	}
	for(u_int i=0;i<toDelete.size();++i){
		bursts.erase(toDelete[i]);
	}

	for(u_int i=0;i<strips.size();++i){
		strips[i].update(now);
	}
}

void Wall::draw(){
	for(u_int i=0;i<strips.size();i++){
		strips[i].draw();
	}
}

void Wall::energyBurst(float x, float y){
	bursts.push_back(EnergyBurst(ofVec2f(x,y)));
}
