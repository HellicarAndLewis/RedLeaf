/*
 * Logo.h
 *
 *  Created on: Oct 7, 2012
 *      Author: arturo
 */

#ifndef LOGO_H_
#define LOGO_H_

#include "ofConstants.h"
#include "ofParameter.h"
#include "ofParameterGroup.h"
#include "ofColor.h"
#include "ofVec2f.h"
#include "ofTrueTypeFont.h"
#include "ofImage.h"

class Logo {
public:
	static void initParameters();
	Logo(u_long now, int x);

	void update();
	void draw();
	bool isAlive();

	void setAmplitude(float amp);



	static ofParameter<float> speedPixelsPerSec;
	static ofParameter<int> y;
	static ofParameter<int> limitScreenX;
	static ofParameter<float> xScale,yScale;
	static ofParameter<int> fontSize;
	static ofParameter<float> alpha;
	static ofParameter<float> separation;
	static ofParameterGroup parameters;
	static ofImage logo;

	ofVec2f position;

private:
	ofColor color;
	u_long time;
};

#endif /* LOGO_H_ */
