#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAxisGui.h"
#include "ofxGstVideoRecorder.h"
#include "ComputerVision.h"

class VideoTestsApp : public ofBaseApp{

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

		void recordChanged(int & record);
		void usePlayerChanged(bool & usePlayer);
		void playerPositionChanged(float & position);
		void playerPausedChanged(bool & paused);
		void drawCameraChanged(int & drawCamera);

		vector<ofxAxisGui*> axisCameras;
		vector<ComputerVision*> cvModules;

		ofxPanel gui;


		ofVideoPlayer player;
		ofParameter<int> record;
		ofParameter<bool> usePlayer;
		ofParameter<bool> playerPaused;
		ofParameter<float> playerPosition;
		ofParameter<int> drawCamera;
		ofParameterGroup videoParameters;

		bool updatingPosition;
		string videoPath;


		ofxGstVideoRecorder gstRecorder;

};
