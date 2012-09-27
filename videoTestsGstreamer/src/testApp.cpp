#include "testApp.h"
#include "ofxXmlSettings.h"
#include "ofGstVideoPlayer.h"


//--------------------------------------------------------------
void testApp::setup(){
	updatingPosition = false;
	
	//ofSetLogLevel(OF_LOG_VERBOSE);

	ofxXmlSettings xml("settings.xml");
	for(u_int i=0;i<4;i++){
		axisCameras.push_back(new ofxAxisGui());
		axisCameras[i]->setDrawGui(false);
		axisCameras[i]->setAuth(xml.getValue("settings:camera"+ofToString(i+1)+":user",""),xml.getValue("settings:camera"+ofToString(i+1)+":pwd",""));
		axisCameras[i]->setup(xml.getValue("settings:camera"+ofToString(i+1)+":address",""),"camera"+ofToString(i+1),220,10);
		cvModules.push_back(new ComputerVision);
		ofAddListener(axisCameras[i]->axis->getGstUtils().bufferEvent,cvModules[i],&ComputerVision::newFrame);
	}

	gui.setup("settings","settings.xml");

	videoParameters.setName("video");
	videoParameters.add(record.set("record",0,0,axisCameras.size()+1));
	videoParameters.add(usePlayer.set("usePlayer",false));
	videoParameters.add(playerPaused.set("playerPaused",false));
	videoParameters.add(playerPosition.set("playerPosition",0,0,1));
	videoParameters.add(drawCamera.set("drawCamera",axisCameras.size()+1,1,axisCameras.size()+1));
	gui.add(videoParameters);


	ComputerVision::parameters.setName("cv");
	ComputerVision::parameters.add(ComputerVision::thresholdLevel);
	ComputerVision::parameters.add(ComputerVision::showGui);
	ComputerVision::parameters.add(ComputerVision::showImages);
	ComputerVision::parameters.add(ComputerVision::showContours);
	ComputerVision::parameters.add(ComputerVision::minTimeTrigger);
	ComputerVision::parameters.add(ComputerVision::distanceSameTrigger);
	gui.add(ComputerVision::parameters);


	for(u_int i=0;i<axisCameras.size();i++){
		gui.add(&axisCameras[i]->gui);
	}

	gui.getGroup("video").getIntSlider("record").setUpdateOnReleaseOnly(true);
	gui.loadFromFile("settings.xml");

	record.addListener(this,&testApp::recordChanged);
	usePlayer.addListener(this,&testApp::usePlayerChanged);
	playerPosition.addListener(this,&testApp::playerPositionChanged);
	playerPaused.addListener(this,&testApp::playerPausedChanged);
	drawCamera.addListener(this,&testApp::drawCameraChanged);

	int activeCamera = drawCamera;
	drawCameraChanged(activeCamera);
}

void testApp::playerPausedChanged(bool & paused){
	if(usePlayer) player.setPaused(paused);
}

void testApp::usePlayerChanged(bool & usePlayer){
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
			ofRemoveListener(((ofGstVideoPlayer*)player.getPlayer().get())->getGstVideoUtils()->bufferEvent,cvModules[0],&ComputerVision::newFrame);
			ofRemoveListener(((ofGstVideoPlayer*)player.getPlayer().get())->getGstVideoUtils()->prerollEvent,cvModules[0],&ComputerVision::newFrame);
			ofAddListener(axisCameras[0]->axis->getGstUtils().bufferEvent,cvModules[0],&ComputerVision::newFrame);
			usePlayer = false;
		}
	}else{
		ofRemoveListener(((ofGstVideoPlayer*)player.getPlayer().get())->getGstVideoUtils()->bufferEvent,cvModules[0],&ComputerVision::newFrame);
		ofRemoveListener(((ofGstVideoPlayer*)player.getPlayer().get())->getGstVideoUtils()->prerollEvent,cvModules[0],&ComputerVision::newFrame);
		ofAddListener(axisCameras[0]->axis->getGstUtils().bufferEvent,cvModules[0],&ComputerVision::newFrame);
	}
}

void testApp::playerPositionChanged(float & position){
	if(usePlayer && !updatingPosition){
		player.setPosition(position);
	}
}
			
void testApp::recordChanged(int & record){
	if(record){
		string dir = ofGetTimestampString();
		//ofDirectory(dir).create();
		gstRecorder.setup(640,480,24,dir+"0.avi",ofxGstVideoRecorder::YUV,round(axisCameras[0]->axis->fps));
	}else{
		gstRecorder.shutdown();
	}
}

void testApp::drawCameraChanged(int & drawCamera){
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
void testApp::update(){
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
void testApp::draw(){
	gui.draw();
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

	ofLine(250,ofGetHeight()-30,850,ofGetHeight()-30);
	for(u_int i=0;i<cvModules.size();i++){
		for(u_int j=0;j<cvModules[i]->getTriggers().size();j++){
			ofCircle(ofMap(cvModules[i]->getTriggers()[j],0,1,0.25*i,0.25*(i+1))*600+250,ofGetHeight()-30,3);
		}
	}
	ofDrawBitmapString("app fps: " + ofToString((int)ofGetFrameRate()),ofGetWidth()-110,20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
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
