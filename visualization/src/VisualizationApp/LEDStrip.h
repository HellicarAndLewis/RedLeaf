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
#include "ofParameterGroup.h"
#include "ofAppRunner.h"
#include "ofVboMesh.h"

class LEDStrip {
public:
	static void initClassParameters();

	void setup(u_int h, float position,float radius);
	void trigger(const ofColor & rgb, u_long now);
	void update(u_long now);
	void draw(float x, float radiusScale, float w=ofGetWidth(), float h=ofGetHeight());

	void setTestMode(bool testMode);

	float getPosition();
	const ofColor & getColor() const;
	void setAmplitude(float amp);

	void setColorCoords(vector<ofFloatColor> & colors);
	void clearColorCoords();

	static ofParameter<int> fadeTime;
	static ofParameterGroup parameters;
private:
	ofColor color,triggerColor;
	float position, radius;
	u_int h;
	u_long triggerTime;
	bool testMode;
	ofVboMesh stripMesh;
};

#endif /* LEDSTRIP_H_ */
