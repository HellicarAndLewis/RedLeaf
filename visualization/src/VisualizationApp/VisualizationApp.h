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
		void tweetFontSizeChanged(int & fontSize);
		void changeFontPressed(bool & pressed);
		void testTweetActivatedChanged(bool & testTweetActivated);

		ofColor niceRandomColor();

		ofxPanel gui;
		ofxButton changeTag;
		ofxButton changeTweetFont;
		ofxButton startTest;
		ofxLabel currentTag;
		ofParameter<string> testTweet;
		ofParameter<bool> testTweetActivated;
		bool cursorHidden;
		u_long lastTestTweetTime;


		Wall wall;
		TwitterListener twitterListener;
		AudioManager audio;

};
