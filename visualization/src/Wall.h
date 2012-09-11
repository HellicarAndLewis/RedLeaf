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
#include <list>

class Wall {
public:
	void setup(u_int w, u_int h);
	void update();
	void draw();

	void energyBurst(float x, float y);

private:
	vector<LEDStrip> strips;
	list<EnergyBurst> bursts;
};

#endif /* WALL_H_ */
