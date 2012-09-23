#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAxisGrabber.h"
#include "ofxGstVideoRecorder.h"

class testApp : public ofBaseApp{

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
		
		void autofocusPressed(bool & pressed);
		void recordPressed(bool & record);
		void showFocusWindowChanged(bool & sfw);

		ofVideoGrabber 			grabber;
		ofPtr<ofxAxisGrabber> 	axis;
		bool settingFocusWindow;
		ofRectangle focusWindow;

		ofxPanel gui;
		ofxButton autofocus;
		ofParameter<bool> showFocusWindow;
		ofParameter<bool> record;

		ofVideoPlayer player;
		ofxGstVideoRecorder gstRecorder;
};
