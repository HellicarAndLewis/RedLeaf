#include "testApp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTime.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofEnableAlphaBlending();
	ofBackground(0);
	//ofSetVerticalSync(true);


	LEDStrip::initClassParameters();
	EnergyBurst::initClassParameters();

	wall.setAudioManager(audio);
	wall.setup();
	audio.setup();

	twitterListener.setup();



	gui.setup("parameters");
	gui.add(currentTag.setup("",twitterListener.hashtag));
	gui.add(changeTag.setup("changeHashtag"));
	gui.add(startTest.setup("startTest"));
	gui.add(LEDStrip::parameters);
	gui.add(EnergyBurst::parameters);
	gui.add(twitterListener.parameters);
	gui.add(wall.parameters);
	gui.add(audio.parameters);
	gui.loadFromFile("settings.xml");

	//twitterListener.hashtag = "#RedLeaf";
	changeTag.addListener(this,&testApp::changeHashTag);
	startTest.addListener(this,&testApp::startTestPressed);

	cursorHidden = false;

	ofAddListener(twitterListener.newTweetE,this,&testApp::newTweet);
	twitterListener.start();
}

void testApp::changeHashTag(bool & pressed){
	if(!pressed) twitterListener.hashtag = ofSystemTextBoxDialog("new hashtag",twitterListener.hashtag);
}

void testApp::startTestPressed(bool & pressed){
	if(!pressed) wall.startTest();
}

//--------------------------------------------------------------
void testApp::update(){
	wall.update();
	audio.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	wall.draw();
	gui.draw();
	Poco::LocalDateTime date;
	ofDrawBitmapString(Poco::DateTimeFormatter::format(date,Poco::DateTimeFormat::ASCTIME_FORMAT ),wall.vizX+wall.renderW-210,20);
}

void testApp::newTweet(const Tweet & tweet){
	wall.energyBurst(ofRandom(1),.5,wall.useColors ? wall.niceRandomColor() : ofColor::white);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key=='m'){
		if(!cursorHidden) ofHideCursor();
		else ofShowCursor();
		cursorHidden = !cursorHidden;
	}
	if(key=='f'){
		ofToggleFullscreen();
	}
	if(key==OF_KEY_LEFT){
		wall.secondScreenPos = ofVec2f(wall.secondScreenPos->x-1,wall.secondScreenPos->y);
	}
	if(key==OF_KEY_RIGHT){
		wall.secondScreenPos = ofVec2f(wall.secondScreenPos->x+1,wall.secondScreenPos->y);
	}
	if(key==OF_KEY_UP){
		wall.secondScreenPos = ofVec2f(wall.secondScreenPos->x,wall.secondScreenPos->y-1);
	}
	if(key==OF_KEY_DOWN){
		wall.secondScreenPos = ofVec2f(wall.secondScreenPos->x,wall.secondScreenPos->y+1);
	}
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
