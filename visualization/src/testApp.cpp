#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofEnableAlphaBlending();
	ofBackground(0);
	ofSetVerticalSync(true);
	wall.setup();
	vizX = 220;

	twitterListener.setup();

	gui.setup("parameters");
	gui.add(LEDStrip::fadeTime);
	gui.add(EnergyBurst::decayPct);
	gui.add(EnergyBurst::minLifeTime);
	gui.add(EnergyBurst::maxLifeTime);
	gui.add(EnergyBurst::minSpeed);
	gui.add(EnergyBurst::maxSpeed);
	gui.add(twitterListener.parameters);
	gui.add(wall.parameters);
	gui.loadFromFile("settings.xml");

	//twitterListener.hashtag = "#RedLeaf";

	ofAddListener(twitterListener.newTweetE,this,&testApp::newTweet);
	twitterListener.start();
}

//--------------------------------------------------------------
void testApp::update(){
	wall.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofPushView();
	ofViewport(vizX,0,1024,768);
	wall.draw();
	ofPopView();

	gui.draw();
}

void testApp::newTweet(const Tweet & tweet){
	wall.energyBurst(ofRandom(1),.5);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if(x>vizX)
		wall.energyBurst(float(x-vizX)/float(1024),float(y)/float(ofGetHeight()));
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
