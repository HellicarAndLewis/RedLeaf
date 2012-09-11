/*
 * LEDStrip.h
 *
 *  Created on: Sep 11, 2012
 *      Author: arturo
 */

#ifndef LEDSTRIP_H_
#define LEDSTRIP_H_

#include "ofColor.h"
#include "ofParameter.h"

class LEDStrip {
public:
	void setup(u_int h, float position,float radius);
	void trigger(const ofColor & rgb, u_long now);
	void update(u_long now);
	void draw();

	float getPosition();

	static ofParameter<int> fadeTime;
private:
	ofColor color,triggerColor;
	float x, radius;
	u_int h;
	u_long triggerTime;
};

#endif /* LEDSTRIP_H_ */
