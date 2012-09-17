#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofEnableAlphaBlending();
	ofBackground(0);
	ofSetVerticalSync(true);
	wall.setup();

	twitterListener.setup();

	gui.setup("parameters");
	gui.add(currentTag.setup("",twitterListener.hashtag));
	gui.add(changeTag.setup("changeHashtag"));
	gui.add(LEDStrip::fadeTime);
	gui.add(EnergyBurst::decayPct);
	gui.add(EnergyBurst::minLifeTime);
	gui.add(EnergyBurst::maxLifeTime);
	gui.add(EnergyBurst::minSpeed);
	gui.add(EnergyBurst::maxSpeed);
	gui.add(useColors.set("useColors",false));
	gui.add(twitterListener.parameters);
	gui.add(wall.parameters);
	gui.loadFromFile("settings.xml");

	//twitterListener.hashtag = "#RedLeaf";
	changeTag.addListener(this,&testApp::changeHashTag);

	ofAddListener(twitterListener.newTweetE,this,&testApp::newTweet);
	twitterListener.start();
}

void testApp::changeHashTag(bool & pressed){
	if(!pressed) twitterListener.hashtag = ofSystemTextBoxDialog("new hashtag",twitterListener.hashtag);
}

//--------------------------------------------------------------
void testApp::update(){
	wall.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	wall.draw();


	gui.draw();
}

ofColor testApp::niceRandomColor(){
	ofColor c;
	unsigned char hue = ofRandom(255);
	unsigned char sat = ofRandom(190,256);
	unsigned char bri = ofRandom(190,256);
	c.setHsb(hue,sat,bri);
	return c;
}

void testApp::newTweet(const Tweet & tweet){
	wall.energyBurst(ofRandom(1),.5,useColors ? niceRandomColor() : ofColor::white);
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
	if(x>wall.vizX)
		wall.energyBurst(float(x-wall.vizX)/float(1024), float(y)/float(ofGetHeight()), useColors ? niceRandomColor() : ofColor::white);
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
