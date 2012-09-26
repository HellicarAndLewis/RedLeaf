#include "testApp.h"
#include "ofxXmlSettings.h"
using namespace ofxCv;


//--------------------------------------------------------------
void testApp::setup(){
	updatingPosition = false;
	
	//ofSetLogLevel(OF_LOG_VERBOSE);

	ofxXmlSettings xml("settings.xml");
	for(u_int i=0;i<4;i++){
		axisCameras.push_back(new ofxAxisGui());
		axisCameras[i]->setDrawGui(false);
		axisCameras[i]->setAuth(xml.getValue("camera:generalConfig:user",""),xml.getValue("camera:generalConfig:pwd",""));
		axisCameras[i]->setup(xml.getValue("camera:generalConfig:address",""),"camera"+ofToString(i+1),220,10);
	}

	gui.setup("general","settings.xml");

	videoParameters.setName("video");
	videoParameters.add(record.set("record",false));
	videoParameters.add(usePlayer.set("usePlayer",false));
	videoParameters.add(playerPaused.set("playerPaused",false));
	videoParameters.add(playerPosition.set("playerPosition",0,0,1));
	videoParameters.add(drawCamera.set("drawCamera",1,1,axisCameras.size()+1));
	gui.add(videoParameters);

	for(u_int i=0;i<axisCameras.size();i++){
		gui.add(&axisCameras[i]->gui);
	}

	cvParameters.setName("cv");
	cvParameters.add(cv.set("cv",false));
	cvParameters.add(thresholdLevel.set("threshold",80,0,255));
	gui.add(cvParameters);

	contourFinder.setAutoThreshold(false);

	record.addListener(this,&testApp::recordPressed);
	usePlayer.addListener(this,&testApp::usePlayerChanged);
	playerPosition.addListener(this,&testApp::playerPositionChanged);
	playerPaused.addListener(this,&testApp::playerPausedChanged);

	drawCamera=5;
}

void testApp::playerPausedChanged(bool & paused){
	if(usePlayer) player.setPaused(paused);
}

void testApp::usePlayerChanged(bool & usePlayer){
	if(usePlayer){
		ofFileDialogResult result = ofSystemLoadDialog("select video",false);
		if(result.bSuccess){
			videoPath=result.filePath;
		}else{
			usePlayer = false;
		}
	}
	reset();
}

void testApp::playerPositionChanged(float & position){
	if(usePlayer && !updatingPosition){
		player.setPosition(position);
	}
}
			

void testApp::reset(){
	player.close();

	if(usePlayer){
		player.loadMovie(videoPath);
		player.play();
	}

	/*tex.allocate(video->getWidth(), video->getHeight(), GL_LUMINANCE);
	gray.allocate(video->getWidth(), video->getHeight(), 1);
	prevFrame.allocate(video->getWidth(), video->getHeight(), 1);
	diffFrame.allocate(video->getWidth(), video->getHeight(), 1);*/
}

void testApp::recordPressed(bool & record){
	if(record){
		string dir = ofGetTimestampString();
		//ofDirectory(dir).create();
		gstRecorder.setup(640,480,24,dir+"0.avi",ofxGstVideoRecorder::YUV,round(axisCameras[0]->axis->fps));
	}else{
		gstRecorder.shutdown();
	}
}


//--------------------------------------------------------------
void testApp::update(){
	for(u_int i=0;i<axisCameras.size();i++){
		axisCameras[i]->update();
	}
	if(usePlayer){
		player.update();
		updatingPosition = true;
		playerPosition = player.getPosition();
		updatingPosition = false;
	}
	/*if(video->isFrameNew()){
		//cout << "new frame" << endl;
		if(cv){
			if(prevFrame.isAllocated()){
				convertColor(video->getPixelsRef(),gray,CV_RGB2GRAY);
				absdiff(gray,prevFrame,diffFrame);
				//blur(diffFrame,5);
				threshold(diffFrame,thresholdLevel);
				contourFinder.findContours(diffFrame);
			}
			prevFrame = gray;
			if(record){
				convertColor(diffFrame,color,CV_GRAY2RGB);
				gstRecorder.newFrame(color);
			}
		}else{
			if(record){
				gstRecorder.newFrame(video->getPixelsRef());
			}
		}
	}*/
}

//--------------------------------------------------------------
void testApp::draw(){
	/*if(cv){
		if(video->isFrameNew()){
			tex.loadData(diffFrame);
		}

		tex.draw(0,0);
		ofNoFill();
		ofSetColor(0,255,0);
		for(u_int i=0;i<contourFinder.getContours().size();i++){
			ofRect(toOf(contourFinder.getBoundingRect(i)));
		}
		ofFill();
		ofSetColor(255);
	}else{
		video->draw(0,0);
	}*/

	gui.draw();

	if(drawCamera<=axisCameras.size()){
		axisCameras[drawCamera-1]->draw(220,10,640,480);
	}else{
		axisCameras[0]->draw(220,10,320,240);
		axisCameras[1]->draw(550,10,320,240);
		axisCameras[2]->draw(220,260,320,240);
		axisCameras[3]->draw(550,260,320,240);
	}
	if(usePlayer){
		player.draw(200,500);
	}
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
