#include "testApp.h"
#include "ofxXmlSettings.h"
using namespace ofxCv;


//--------------------------------------------------------------
void testApp::setup(){
	settingFocusWindow = false;
	updatingPosition = false;
	
	//ofSetLogLevel(OF_LOG_VERBOSE);
	axis = ofPtr<ofxAxisGrabber>(new ofxAxisGrabber);

	ofxXmlSettings xml("settings.xml");

	gui.setup("camera","settings.xml",650,10);
	parameters.setName("generalConfig");
	parameters.add(showFocusWindow.set("showFocusWindow",false));
	parameters.add(record.set("record",false));
	parameters.add(resolution.set("resolution",1,0,3));
	parameters.add(address.set("address",xml.getValue("camera:generalConfig:address","")));

	parameters.add(usePlayer.set("usePlayer",false));
	parameters.add(playerPaused.set("playerPaused",false));
	parameters.add(playerPosition.set("playerPosition",0,0,1));
				   
	gui.add(autofocus.setup("autofocus"));
	gui.add(changeIp.setup("changeIp"));
	gui.add(parameters);


	cvParameters.add(cv.set("cv",false));
	cvParameters.add(thresholdLevel.set("threshold",80,0,255));
	gui.add(cvParameters);

	
	axis->setCameraAddress(address);
	axis->setParametersRefreshRate(5000);
	axis->setDesiredFrameRate(30);
	axis->setCodec(ofxAxisGrabber::H264);
	axis->setAuth(xml.getValue("camera:generalConfig:user",""),xml.getValue("camera:generalConfig:pwd",""));
	
	gui.add(axis->parameters);
	gui.getGroup(address).getIntSlider("focus").setUpdateOnReleaseOnly(true);
	gui.getGroup(address).getIntSlider("exposure").setUpdateOnReleaseOnly(true);
	gui.getGroup(address).getIntSlider("irFilterCut").setUpdateOnReleaseOnly(true);
	gui.getGroup(address).getIntSlider("compression").setUpdateOnReleaseOnly(true);
	gui.getGroup("generalConfig").getIntSlider("resolution").setUpdateOnReleaseOnly(true);
	
	contourFinder.setAutoThreshold(false);

	grabber.setGrabber(axis);
	reset();

	autofocus.addListener(this,&testApp::autofocusPressed);
	showFocusWindow.addListener(this,&testApp::showFocusWindowChanged);
	record.addListener(this,&testApp::recordPressed);
	changeIp.addListener(this,&testApp::changeIpPressed);
	resolution.addListener(this,&testApp::resolutionChanged);
	usePlayer.addListener(this,&testApp::usePlayerChanged);
	playerPosition.addListener(this,&testApp::playerPositionChanged);
	playerPaused.addListener(this,&testApp::playerPausedChanged);
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
			
void testApp::changeIpPressed(bool & pressed){
	if(!pressed){
		string newAddress = ofSystemTextBoxDialog("ip", address);
		if(string(address) != newAddress){
			address = newAddress;
			reset();
		}
	}
}

void testApp::resolutionChanged(int & resolution){
	reset();
}

void testApp::reset(){
	grabber.close();
	player.close();

	if(usePlayer){
		player.loadMovie(videoPath);
		player.play();
		video = &player;
	}else{
		videoPath = "";
		axis->setCameraAddress(address);
		switch (resolution) {
			case 0:
				grabber.initGrabber(320, 240);
				break;
			case 1:
				grabber.initGrabber(640, 480);
				break;
			case 2:
				grabber.initGrabber(800, 600);
				break;
			case 3:
				grabber.initGrabber(1280, 720);
				break;
			default:
				break;
		}
		video = &grabber;
	}

	tex.allocate(video->getWidth(), video->getHeight(), GL_LUMINANCE);
	gray.allocate(video->getWidth(), video->getHeight(), 1);
	prevFrame.allocate(video->getWidth(), video->getHeight(), 1);
	diffFrame.allocate(video->getWidth(), video->getHeight(), 1);
}

void testApp::autofocusPressed(bool & pressed){
	if(!pressed && !usePlayer)
		axis->triggerAutoFocus();
}


void testApp::showFocusWindowChanged(bool & sfw){
	if(sfw && !usePlayer) focusWindow = axis->getFocusWindow();
}

void testApp::recordPressed(bool & record){
	if(record){
		string dir = ofGetTimestampString();
		//ofDirectory(dir).create();
		gstRecorder.setup(640,480,24,dir+"0.avi",ofxGstVideoRecorder::YUV,round(axis->fps));
	}else{
		gstRecorder.shutdown();
	}
}


//--------------------------------------------------------------
void testApp::update(){
	if(settingFocusWindow  && showFocusWindow && ofGetMousePressed(0)){
		focusWindow.width = ofGetMouseX() - focusWindow.x;
		focusWindow.height = ofGetMouseY() - focusWindow.y;
	}
	video->update();
	if(usePlayer){
		updatingPosition = true;
		playerPosition = player.getPosition();
		updatingPosition = false;
	}
	if(video->isFrameNew()){
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
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	if(cv){
		if(video->isFrameNew()){
			tex.loadData(diffFrame);
		}

		tex.draw(0,0);
		/*ofSetColor(0,255,0);
		for(u_int i=0;i<contourFinder.getContours().size();i++){
			ofRect(toOf(contourFinder.getBoundingRect(i)));
		}*/
		ofSetColor(255);
	}else{
		video->draw(0,0);
	}
	gui.draw();
	if(showFocusWindow){
		ofNoFill();
		ofRect(focusWindow);
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
	if(button == 0 && showFocusWindow && x<grabber.getWidth() && y<grabber.getHeight()){
		focusWindow.x = x;
		focusWindow.y = y;
		focusWindow.width = 0;
		focusWindow.height = 0;
		settingFocusWindow = true;
	}

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	if(settingFocusWindow && button==0 && showFocusWindow){
		axis->setFocusWindow(focusWindow);
		settingFocusWindow = false;
	}
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
