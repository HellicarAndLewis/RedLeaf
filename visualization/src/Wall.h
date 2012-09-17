/*
 * Wall.h
 *
 *  Created on: Sep 11, 2012
 *      Author: arturo
 */

#ifndef WALL_H_
#define WALL_H_

#include <list>
#include "ofConstants.h"
#include "ofParameterGroup.h"
#include "ofFbo.h"
#include "ofVboMesh.h"
#include "ofImage.h"
#include "LEDStrip.h"
#include "EnergyBurst.h"

class Wall {
public:
	void setup();
	void update();
	void draw();

	void energyBurst(float x, float y, const ofColor & c);

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
		Output,
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
	ofVboMesh building;
	ofVboMesh buildingWireframe;
	ofImage outputBuffer;
};

#endif /* WALL_H_ */
