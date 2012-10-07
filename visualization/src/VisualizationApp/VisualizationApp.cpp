#include "VisualizationApp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTime.h"

//--------------------------------------------------------------
void VisualizationApp::setup(){
	ofEnableAlphaBlending();
	ofBackground(0);

	LEDStrip::initClassParameters();
	EnergyBurst::initClassParameters();

	wall.setAudioManager(audio);
	wall.setup();
	audio.setup();

	twitterListener.setup();



	gui.setup("Settings");
	gui.add(currentTag.setup("",twitterListener.hashtag));
	gui.add(changeTag.setup("changeHashtag"));
	gui.add(startTest.setup("startTest"));
	gui.add(LEDStrip::parameters);
	gui.add(EnergyBurst::parameters);
	gui.add(twitterListener.parameters);
	gui.add(wall.parameters);
	gui.add(audio.parameters);

	gui.getGroup("Twitter").add(changeTweetFont.setup("changeFont"));
	gui.getGroup("Twitter").add(TweetText::speedPixelsPerSec);
	gui.getGroup("Twitter").add(TweetText::y);
	gui.getGroup("Twitter").add(TweetText::xScale);
	gui.getGroup("Twitter").add(TweetText::fontSize);
	gui.getGroup("Twitter").add(TweetText::useColors);
	gui.getGroup("Twitter").add(TweetText::alpha);
	gui.getGroup("Twitter").add(testTweetActivated.set("testTweetActivated",false));

	gui.loadFromFile("settings.xml");

	gui.minimizeAll();


	TweetText::font.loadFont(TweetText::fontName,TweetText::fontSize,true,true);

	gui.getGroup("Twitter").getIntSlider("fontSize").setUpdateOnReleaseOnly(true);

	changeTag.addListener(this,&VisualizationApp::changeHashTag);
	startTest.addListener(this,&VisualizationApp::startTestPressed);
	TweetText::fontSize.addListener(this,&VisualizationApp::tweetFontSizeChanged);
	changeTweetFont.addListener(this,&VisualizationApp::changeFontPressed);
	testTweetActivated.addListener(this,&VisualizationApp::testTweetActivatedChanged);

	cursorHidden = false;

	lastTestTweetTime = ofGetElapsedTimeMillis();
	ofAddListener(twitterListener.newTweetE,this,&VisualizationApp::newTweet);
	twitterListener.start();
}

void VisualizationApp::changeFontPressed(bool & pressed){
	if(!pressed){
		ofFileDialogResult result = ofSystemLoadDialog("choose font",false);
		if(result.bSuccess){
			TweetText::fontName = result.filePath;
			TweetText::font.loadFont(TweetText::fontName,TweetText::fontSize,true,true);
		}
	}
}

void VisualizationApp::changeHashTag(bool & pressed){
	if(!pressed) twitterListener.hashtag = ofSystemTextBoxDialog("new hashtag",twitterListener.hashtag);
}

void VisualizationApp::startTestPressed(bool & pressed){
	if(!pressed) wall.startTest();
}


void VisualizationApp::tweetFontSizeChanged(int & fontSize){
	TweetText::font.loadFont(TweetText::fontName,TweetText::fontSize,true,true);
}

void VisualizationApp::testTweetActivatedChanged(bool & testTweetActivated){
	if(testTweetActivated){
		testTweet = ofSystemTextBoxDialog("Enter test tweet","");
		if(""==(string)testTweet) testTweetActivated=false;
	}
}

//--------------------------------------------------------------
void VisualizationApp::update(){
	wall.update();
	audio.update();
	if(testTweetActivated && ofGetElapsedTimeMillis()-lastTestTweetTime>10000){
		Tweet tweet;
		tweet.title = testTweet;
		newTweet(tweet);

		lastTestTweetTime = ofGetElapsedTimeMillis();
	}
}

//--------------------------------------------------------------
void VisualizationApp::draw(){
	gui.draw();
	wall.draw();
	Poco::LocalDateTime date;
	ofDrawBitmapString(Poco::DateTimeFormatter::format(date,Poco::DateTimeFormat::ASCTIME_FORMAT ),wall.vizX+wall.renderW-210,20);
	ofDrawBitmapString("app fps: " + ofToString((int)ofGetFrameRate()),wall.vizX+wall.renderW-210,40);
}

void VisualizationApp::newTweet(const Tweet & tweet){
	wall.newTweet(tweet.title);
}

//--------------------------------------------------------------
void VisualizationApp::keyPressed(int key){
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
void VisualizationApp::keyReleased(int key){

}

//--------------------------------------------------------------
void VisualizationApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void VisualizationApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void VisualizationApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void VisualizationApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void VisualizationApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void VisualizationApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void VisualizationApp::dragEvent(ofDragInfo dragInfo){

}
