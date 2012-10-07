#include "testApp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTime.h"
#include "TweetText.h"

//--------------------------------------------------------------
void RedLeafApp::setup(){
	videoApp.setup();
	visualizationApp.setup();
	activeApp=&visualizationApp;
	showGui = true;

	gui.setup("Settings");

	videoVisualization.addListener(this,&RedLeafApp::activeAppChanged);
	verticalSync.addListener(this,&RedLeafApp::verticalSyncChanged);

	gui.add(videoVisualization.set("Video/Viz",true));
	gui.add(verticalSync.set("vsync",false));
	gui.add(LEDStrip::parameters);
	gui.add(EnergyBurst::parameters);

	gui.add(visualizationApp.twitterListener.parameters);
	gui.getGroup("Twitter").add(&visualizationApp.currentTag);
	gui.getGroup("Twitter").add(&visualizationApp.changeTag);
	gui.getGroup("Twitter").add(&visualizationApp.changeTweetFont);
	gui.getGroup("Twitter").add(TweetText::speedPixelsPerSec);
	gui.getGroup("Twitter").add(TweetText::y);
	gui.getGroup("Twitter").add(TweetText::xScale);
	gui.getGroup("Twitter").add(TweetText::fontSize);
	gui.getGroup("Twitter").add(TweetText::useColors);
	gui.getGroup("Twitter").add(TweetText::alpha);
	gui.getGroup("Twitter").add(visualizationApp.testTweetActivated);

	gui.add(visualizationApp.wall.parameters);
	gui.getGroup("Wall").add(&visualizationApp.startTest);

	gui.add(visualizationApp.audio.parameters);

	gui.add(videoApp.videoParameters);
	gui.add(ComputerVision::globalParameters);
	for(u_int i=0;i<videoApp.cvModules.size();i++){
		gui.add(&videoApp.cvModules[i]->gui);
		ofAddListener(videoApp.cvModules[i]->calibrationPoint,this,&RedLeafApp::calibrationPointTriggered);
		videoApp.cvModules[i]->calibrationMode.addListener(this,&RedLeafApp::calibrationModeChanged);
	}
	for(u_int i=0;i<videoApp.axisCameras.size();i++){
		gui.add(&videoApp.axisCameras[i]->gui);
	}

	gui.loadFromFile("settings.xml");

	gui.getGroup("Player").getIntSlider("record").setUpdateOnReleaseOnly(true);
	gui.getGroup("Twitter").getIntSlider("fontSize").setUpdateOnReleaseOnly(true);


	ofAddListener(gui.savePressedE,&videoApp,&VideoTestsApp::savePressed);
	ofAddListener(gui.loadPressedE,&videoApp,&VideoTestsApp::loadPressed);

	bool yes=true;
	videoApp.loadPressed(yes);

	gui.minimizeAll();
    
    ofSetFrameRate(30); //added frame rate...
}

void RedLeafApp::calibrationPointTriggered(const void * sender, float & distance){
	for(u_int i=0;i<videoApp.cvModules.size();i++){
		if(videoApp.cvModules[i]==sender){
			float wallDistance = distance*.25+.25*i;
			visualizationApp.wall.turnOnForCalibration(wallDistance);
			break;
		}
	}
}

void RedLeafApp::calibrationModeChanged(const void * sender, bool & calibrationMode){
	bool calibrationModeAll = false;
	for(u_int i=0;i<videoApp.cvModules.size();i++){
		calibrationModeAll |= videoApp.cvModules[i]->calibrationMode;
	}
	if(!calibrationModeAll) visualizationApp.wall.turnOffCalibration();
}

//--------------------------------------------------------------
void RedLeafApp::update(){
	videoApp.update();
	visualizationApp.update();
	for(u_int i=0;i<videoApp.cvModules.size();i++){
		if(videoApp.cvModules[i]->isFrameNew()){
			for(u_int j=0;j<videoApp.cvModules[i]->getTriggers().size();j++){
				visualizationApp.wall.energyBurst(ofMap(videoApp.cvModules[i]->getTriggers()[j],0,1,i*0.25,(i+1)*0.25),.5);
			}
		}
	}
}

//--------------------------------------------------------------
void RedLeafApp::draw(){
	if(showGui){
		gui.draw();
		if(!videoVisualization){
			visualizationApp.wall.drawActiveArea(Wall::ThreeD);
			visualizationApp.wall.drawOutput();
			videoApp.drawCameras();
		}else{
			visualizationApp.wall.draw();
		}
	}else{
		visualizationApp.wall.drawOutput();
	}

	float textX = visualizationApp.wall.vizX+visualizationApp.wall.renderW-210;
	Poco::LocalDateTime date;
	ofDrawBitmapString(Poco::DateTimeFormatter::format(date,Poco::DateTimeFormat::ASCTIME_FORMAT ),textX,20);
	ofDrawBitmapString("app fps: " + ofToString((int)ofGetFrameRate()),textX,40);
}

void RedLeafApp::activeAppChanged(bool & videoVisualization){
	if(videoVisualization){
		activeApp=&visualizationApp;
		visualizationApp.wall.enableMouseEvents();
	}else{
		activeApp=&videoApp;
		visualizationApp.wall.disableMouseEvents();
	}
}

void RedLeafApp::verticalSyncChanged(bool & vsync){
	ofSetVerticalSync(vsync);
}

//--------------------------------------------------------------
void RedLeafApp::keyPressed(int key){
	if(key=='v'){
		videoVisualization = !videoVisualization;
	}
	if(key=='h'){
		showGui=!showGui;
	}
	activeApp->keyPressed(key);
}

//--------------------------------------------------------------
void RedLeafApp::keyReleased(int key){
	activeApp->keyReleased(key);
}

//--------------------------------------------------------------
void RedLeafApp::mouseMoved(int x, int y ){
	activeApp->mouseMoved(x,y);
}

//--------------------------------------------------------------
void RedLeafApp::mouseDragged(int x, int y, int button){
	activeApp->mouseDragged(x,y,button);
}

//--------------------------------------------------------------
void RedLeafApp::mousePressed(int x, int y, int button){
	activeApp->mousePressed(x,y,button);
}

//--------------------------------------------------------------
void RedLeafApp::mouseReleased(int x, int y, int button){
	activeApp->mouseReleased(x,y,button);
}

//--------------------------------------------------------------
void RedLeafApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void RedLeafApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void RedLeafApp::dragEvent(ofDragInfo dragInfo){

}
