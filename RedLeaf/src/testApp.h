#pragma once

#include "ofMain.h"
#include "VideoTestsApp.h"
#include "VisualizationApp.h"
#include "ofxGui.h"

class RedLeafApp : public ofBaseApp{

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
		
		void activeAppChanged(bool & videoVisualization);
		void verticalSyncChanged(bool & vsync);
		void calibrationPointTriggered(const void * sender, float & distance);
		void calibrationModeChanged(const void * sender, bool & calibrationMode);


		VideoTestsApp videoApp;
		VisualizationApp visualizationApp;
		ofBaseApp * activeApp;
		ofxPanel gui;
		ofParameter<bool> videoVisualization;
		ofParameter<bool> verticalSync;
		bool showGui;
};
