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
#include "ofFbo.h"
#include <list>

class Wall {
public:
	void setup();
	void update();
	void draw();

	void energyBurst(float x, float y);

	ofParameter<int> w,h;
	ofParameter<int> vizX;
	ofParameter<int> renderW,renderH;
	ofParameter<int> renderMode;
	ofParameter<ofVec2f> rotation3D;
	ofParameter<float> radiusScale;
	ofParameter<float> z;
	ofParameterGroup parameters;
	enum RenderMode{
		Continuous,
		Separate,
		ThreeD,
		NumModes
	};
private:
	void mouseDragged(ofMouseEventArgs & mouse);
	void mousePressed(ofMouseEventArgs & mouse);
	void sizeChanged(int & size);
	void reset();
	vector<LEDStrip> strips;
	list<EnergyBurst> bursts;
	ofFbo renderFbo;
	ofPoint dragStart;
	ofVec2f startRotation3D;
};

#endif /* WALL_H_ */
