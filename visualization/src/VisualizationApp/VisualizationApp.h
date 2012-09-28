#pragma once

#include "ofMain.h"
#include "Wall.h"
#include "ofxGui.h"
#include "TwitterListener.h"
#include "AudioManager.h"

class VisualizationApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void newTweet(const Tweet & tweet);
		void changeHashTag(bool & pressed);
		void startTestPressed(bool & pressed);

		ofColor niceRandomColor();

		ofxPanel gui;
		ofxButton changeTag;
		ofxButton startTest;
		ofxLabel currentTag;
		bool cursorHidden;


		Wall wall;
		TwitterListener twitterListener;
		AudioManager audio;

};