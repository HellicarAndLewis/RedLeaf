/*
 * Wall.h
 *
 *  Created on: Sep 11, 2012
 *      Author: arturo
 */

#ifndef WALL_H_
#define WALL_H_

#include "LEDStrip.h"
#include "ofConstants.h"
#include "EnergyBurst.h"
#include "ofParameterGroup.h"
#include <list>

class Wall {
public:
	void setup();
	void update();
	void draw();

	void energyBurst(float x, float y);

	ofParameter<int> w,h;
	ofParameterGroup parameters;
private:
	void sizeChanged(int & size);
	void reset();
	vector<LEDStrip> strips;
	list<EnergyBurst> bursts;
};

#endif /* WALL_H_ */
