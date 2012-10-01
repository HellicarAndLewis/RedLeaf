#include "VideoTestsApp.h"
#include "ofxXmlSettings.h"
#include "ofGstVideoPlayer.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTime.h"


//--------------------------------------------------------------
void VideoTestsApp::setup(){
	updatingPosition = false;
	
	//ofSetLogLevel(OF_LOG_VERBOSE);

	ofxXmlSettings xml("settings.xml");
	for(u_int i=0;i<4;i++){
		axisCameras.push_back(new ofxAxisGui());
		axisCameras[i]->setDrawGui(false);
		axisCameras[i]->setAuth(xml.getValue("Settings:Camera"+ofToString(i+1)+":user",""),xml.getValue("Settings:Camera"+ofToString(i+1)+":pwd",""));
		axisCameras[i]->setup(xml.getValue("Settings:Camera"+ofToString(i+1)+":address",""),"Camera"+ofToString(i+1),220,10);
		cvModules.push_back(new ComputerVision);
		ofAddListener(axisCameras[i]->axis->getGstUtils().bufferEvent,cvModules[i],&ComputerVision::newFrame);
	}

	gui.setup("Settings","settings.xml");

	videoParameters.setName("Player");
	videoParameters.add(record.set("record",0,0,axisCameras.size()+1));
	videoParameters.add(usePlayer.set("usePlayer",false));
	videoParameters.add(playerPaused.set("playerPaused",false));
	videoParameters.add(playerPosition.set("playerPosition",0,0,1));
	videoParameters.add(drawCamera.set("drawCamera",axisCameras.size()+1,1,axisCameras.size()+1));
	gui.add(videoParameters);


	ComputerVision::parameters.setName("CV");
	ComputerVision::parameters.add(ComputerVision::thresholdLevel);
	ComputerVision::parameters.add(ComputerVision::showGui);
	ComputerVision::parameters.add(ComputerVision::showThreshold);
	ComputerVision::parameters.add(ComputerVision::showContours);
	ComputerVision::parameters.add(ComputerVision::minTimeTrigger);
	ComputerVision::parameters.add(ComputerVision::distanceSameTrigger);
	gui.add(ComputerVision::parameters);


	for(u_int i=0;i<axisCameras.size();i++){
		gui.add(&axisCameras[i]->gui);
	}

	gui.getGroup("Player").getIntSlider("record").setUpdateOnReleaseOnly(true);
	gui.loadFromFile("settings.xml");
	gui.minimizeAll();

	ofAddListener(gui.savePressedE,this,&VideoTestsApp::savePressed);
	ofAddListener(gui.loadPressedE,this,&VideoTestsApp::loadPressed);

	record.addListener(this,&VideoTestsApp::recordChanged);
	usePlayer.addListener(this,&VideoTestsApp::usePlayerChanged);
	playerPosition.addListener(this,&VideoTestsApp::playerPositionChanged);
	playerPaused.addListener(this,&VideoTestsApp::playerPausedChanged);
	drawCamera.addListener(this,&VideoTestsApp::drawCameraChanged);

	int activeCamera = drawCamera;
	drawCameraChanged(activeCamera);
}

void VideoTestsApp::loadPressed(bool & pressed){
	ofxXmlSettings xml("settings.xml");
	ofVec3f defaultQuad[4] = {
			ofVec3f(0.1,0.1),
			ofVec3f(0.9,0.1),
			ofVec3f(0.9,0.9),
			ofVec3f(0.1,0.9),
	};
	for(u_int i=0;i<cvModules.size();i++){
		vector<ofPoint> quad(4);
		for(u_int j=0;j<quad.size();j++){
			stringstream strpt;
			strpt << defaultQuad[j];
			stringstream ptstr;
			ptstr << xml.getValue("Settings:Camera"+ofToString(i+1)+":Quad:Pt"+ofToString(j),strpt.str());
			ptstr >> quad[j];
		}
		cvModules[i]->setQuad(quad);
	}
}

void VideoTestsApp::savePressed(bool & pressed){
	ofxXmlSettings xml("settings.xml");
	for(u_int i=0;i<cvModules.size();i++){
		const vector<ofPoint> & quad = cvModules[i]->getQuad();
		for(u_int j=0;j<quad.size();j++){
			stringstream strpt;
			strpt << quad[j];
			xml.setValue("Settings:Camera"+ofToString(i+1)+":Quad:Pt"+ofToString(j),strpt.str());
		}
	}
	xml.saveFile();
}

void VideoTestsApp::playerPausedChanged(bool & paused){
	if(usePlayer) player.setPaused(paused);
}

void VideoTestsApp::usePlayerChanged(bool & usePlayer){
	player.close();
	if(usePlayer){
		ofFileDialogResult result = ofSystemLoadDialog("select video",false);
		if(result.bSuccess){
			videoPath=result.filePath;
			player.loadMovie(videoPath);
			player.play();
			ofRemoveListener(axisCameras[0]->axis->getGstUtils().bufferEvent,cvModules[0],&ComputerVision::newFrame);
			ofAddListener(((ofGstVideoPlayer*)player.getPlayer().get())->getGstVideoUtils()->bufferEvent,cvModules[0],&ComputerVision::newFrame);
			ofAddListener(((ofGstVideoPlayer*)player.getPlayer().get())->getGstVideoUtils()->prerollEvent,cvModules[0],&ComputerVision::newFrame);
		}else{
			usePlayer = false;
		}
	}else{
		if(player.getPlayer()){
			ofRemoveListener(((ofGstVideoPlayer*)player.getPlayer().get())->getGstVideoUtils()->bufferEvent,cvModules[0],&ComputerVision::newFrame);
			ofRemoveListener(((ofGstVideoPlayer*)player.getPlayer().get())->getGstVideoUtils()->prerollEvent,cvModules[0],&ComputerVision::newFrame);
		}
		ofAddListener(axisCameras[0]->axis->getGstUtils().bufferEvent,cvModules[0],&ComputerVision::newFrame);
	}
}

void VideoTestsApp::playerPositionChanged(float & position){
	if(usePlayer && !updatingPosition){
		player.setPosition(position);
	}
}
			
void VideoTestsApp::recordChanged(int & record){
	if(record){
		string dir = ofGetTimestampString();
		//ofDirectory(dir).create();
		gstRecorder.setup(640,480,24,dir+"0.avi",ofxGstVideoRecorder::YUV,round(axisCameras[0]->axis->fps));
	}else{
		gstRecorder.shutdown();
	}
}

void VideoTestsApp::drawCameraChanged(int & drawCamera){
	if(drawCamera<=(int)cvModules.size()){
		cvModules[drawCamera-1]->setPosition(220,10);
		cvModules[drawCamera-1]->setSize(640,480);
	}else{
		cvModules[0]->setPosition(220,10);
		cvModules[0]->setSize(320,240);
		cvModules[1]->setPosition(550,10);
		cvModules[1]->setSize(320,240);
		cvModules[2]->setPosition(220,260);
		cvModules[2]->setSize(320,240);
		cvModules[3]->setPosition(550,260);
		cvModules[3]->setSize(320,240);
	}
}


//--------------------------------------------------------------
void VideoTestsApp::update(){
	for(u_int i=0;i<axisCameras.size();i++){
		axisCameras[i]->update();
		cvModules[i]->update();
	}
	if(usePlayer){
		player.update();
		updatingPosition = true;
		playerPosition = player.getPosition();
		updatingPosition = false;
	}
	if(record && axisCameras[record-1]->grabber.isFrameNew()){
		gstRecorder.newFrame(axisCameras[record-1]->grabber.getPixelsRef());
	}

}

//--------------------------------------------------------------
void VideoTestsApp::draw(){
	gui.draw();
	drawCameras();
	Poco::LocalDateTime date;
	ofDrawBitmapString(Poco::DateTimeFormatter::format(date,Poco::DateTimeFormat::ASCTIME_FORMAT ),ofGetWidth()-110,20);
	ofDrawBitmapString("app fps: " + ofToString((int)ofGetFrameRate()),ofGetWidth()-110,40);
}


void VideoTestsApp::drawCameras(){
	if(drawCamera<=(int)axisCameras.size()){
		axisCameras[drawCamera-1]->draw(220,10,640,480);
	}else{
		axisCameras[0]->draw(220,10,320,240);
		axisCameras[1]->draw(550,10,320,240);
		axisCameras[2]->draw(220,260,320,240);
		axisCameras[3]->draw(550,260,320,240);
	}
	if(usePlayer){
		if(drawCamera<=(int)axisCameras.size()) player.draw(220,10,640,480);
		else  player.draw(220,10,320,240);
	}
	if(drawCamera<=(int)cvModules.size()){
		cvModules[drawCamera-1]->draw();
	}else{
		for(u_int i=0;i<cvModules.size();i++){
			cvModules[i]->draw();
		}
	}

	/*ofLine(250,ofGetHeight()-30,850,ofGetHeight()-30);
	for(u_int i=0;i<cvModules.size();i++){
		for(u_int j=0;j<cvModules[i]->getTriggers().size();j++){
			ofCircle(ofMap(cvModules[i]->getTriggers()[j],0,1,0.25*i,0.25*(i+1))*600+250,ofGetHeight()-30,3);
		}
	}*/
}

//--------------------------------------------------------------
void VideoTestsApp::keyPressed(int key){
	if(usePlayer && key==OF_KEY_RIGHT){
		player.setFrame(player.getCurrentFrame()+1);
	}
	if(usePlayer && key==OF_KEY_LEFT){
		player.setFrame(player.getCurrentFrame()-1);
	}
	if(usePlayer && key==' '){
		playerPaused = !playerPaused;
	}

}

//--------------------------------------------------------------
void VideoTestsApp::keyReleased(int key){

}

//--------------------------------------------------------------
void VideoTestsApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void VideoTestsApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void VideoTestsApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void VideoTestsApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void VideoTestsApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void VideoTestsApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void VideoTestsApp::dragEvent(ofDragInfo dragInfo){ 

}
