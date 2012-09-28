/*
 * Wall.h
 *
 *  Created on: Sep 11, 2012
 *      Author: arturo
 */

#ifndef WALL_H_
#define WALL_H_

#include <list>
#include <queue>
#include "ofConstants.h"
#include "ofParameterGroup.h"
#include "ofFbo.h"
#include "ofVboMesh.h"
#include "ofImage.h"
#include "LEDStrip.h"
#include "EnergyBurst.h"
#include "AudioManager.h"
#include "ofTrueTypeFont.h"
#include "TweetText.h"

class Wall {
public:
	void setAudioManager(AudioManager & audio);
	void setup();
	void update();
	void draw();

	enum RenderMode{
		Continuous,
		Separate,
		ThreeD,
		NumModes
	};
	void drawActiveArea(RenderMode renderMode);
	void drawOutput();

	void energyBurst(float x, float y);
	void newTweet(string text);
	void startTest();
	ofColor niceRandomColor();

	void enableMouseEvents();
	void disableMouseEvents();

	ofParameter<int> w,h;
	ofParameter<int> vizX;
	ofParameter<int> renderW,renderH;
	ofParameter<int> renderMode;
	ofParameter<ofVec2f> rotation3D;
	ofParameter<float> radiusScale;
	ofParameter<float> z;
	ofParameter<ofVec2f> secondScreenPos;
	ofParameter<float> testStateMillis;
	ofParameter<bool> useColors;
	ofParameter<bool> muted;
	ofParameter<bool> showTweets;
	ofParameterGroup parameters;
private:
	void mouseDragged(ofMouseEventArgs & mouse);
	void mousePressed(ofMouseEventArgs & mouse);
	void sizeChanged(int & size);
	void showTweetsChanged(bool & showTweets);
	void reset();
	vector<LEDStrip> strips;
	list<EnergyBurst> bursts;
	ofFbo renderFbo;
	ofPoint dragStart;
	ofVec2f startRotation3D;
	ofVboMesh building;
	ofVboMesh buildingWireframe;
	AudioManager * audio;

	enum TestState{
		AllRed=1,
		AllGreen,
		AllBlue,
		AllWhite,
		RedOneByOne,
		GreenOneByOne,
		BlueOneByOne,
		WhiteOneByOne,
		ProgressiveRed,
		ProgressiveGreen,
		ProgressiveBlue,
		ProgressiveWhite,
		NumTestStates
	};
	int testState;
	u_long testStartTime;
	bool runningTest;
	u_int lastStripTestOn;
	u_int lastTimeStripChangedTest;
	u_int nextStripOn;
	int prevStripOn;
	u_long prevTestEndTime;
	ofTrueTypeFont font;

	queue<TweetText> tweets;
	ofFbo outputFBO;
	ofPixels outputBuffer;
};

#endif /* WALL_H_ */
