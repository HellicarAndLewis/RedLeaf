#include "testApp.h"
#include "ofxXmlSettings.h"
using namespace ofxCv;


//--------------------------------------------------------------
void testApp::setup(){
	settingFocusWindow = false;
	
	ofSetLogLevel(OF_LOG_VERBOSE);
	axis = ofPtr<ofxAxisGrabber>(new ofxAxisGrabber);

	ofxXmlSettings xml("settings.xml");

	gui.setup("camera","settings.xml",650,10);
	parameters.setName("generalConfig");
	parameters.add(showFocusWindow.set("showFocusWindow",false));
	parameters.add(record.set("record",false));
	parameters.add(resolution.set("resolution",1,0,3));
	parameters.add(address.set("address",xml.getValue("camera:generalConfig:address","")));

				   
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
	axis->setCameraAddress(address);
	switch (resolution) {
		case 0:
			grabber.initGrabber(320, 240);
			tex.allocate(320, 240, GL_LUMINANCE);
			gray.allocate(320,240,1);
			prevFrame.allocate(320,240,1);
			diffFrame.allocate(320,240,1);
			break;
		case 1:
			grabber.initGrabber(640, 480);
			tex.allocate(640, 480, GL_LUMINANCE);
			gray.allocate(640, 480,1);
			prevFrame.allocate(640, 480,1);
			diffFrame.allocate(640, 480,1);
			break;
		case 2:
			grabber.initGrabber(800, 600);
			tex.allocate(800, 600, GL_LUMINANCE);
			gray.allocate(800, 600,1);
			prevFrame.allocate(800, 600,1);
			diffFrame.allocate(800, 600,1);
			break;
		case 3:
			grabber.initGrabber(1280, 720);
			tex.allocate(1280, 720, GL_LUMINANCE);
			gray.allocate(1280, 720,1);
			prevFrame.allocate(1280, 720,1);
			diffFrame.allocate(1280, 720,1);
			break;
		default:
			break;
	}
}

void testApp::autofocusPressed(bool & pressed){
	if(!pressed)
		axis->triggerAutoFocus();
}


void testApp::showFocusWindowChanged(bool & sfw){
	if(sfw) focusWindow = axis->getFocusWindow();
}

void testApp::recordPressed(bool & record){
	if(record){
		string dir = ofGetTimestampString();
		ofDirectory(dir).create();
		gstRecorder.setup(640,480,24,dir+"/0.png",ofxGstVideoRecorder::PNG_SEQUENCE,round(axis->fps));
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
	grabber.update();
	if(grabber.isFrameNew()){
		//cout << "new frame" << endl;
		if(cv){
			if(prevFrame.isAllocated()){
				convertColor(grabber.getPixelsRef(),gray,CV_RGB2GRAY);
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
				gstRecorder.newFrame(grabber.getPixelsRef());
			}
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	if(cv){
		if(grabber.isFrameNew()){
			tex.loadData(diffFrame);
		}

		tex.draw(0,0);
		/*ofSetColor(0,255,0);
		for(u_int i=0;i<contourFinder.getContours().size();i++){
			ofRect(toOf(contourFinder.getBoundingRect(i)));
		}*/
		ofSetColor(255);
	}else{
		grabber.draw(0,0);
	}
	gui.draw();
	if(showFocusWindow){
		ofNoFill();
		ofRect(focusWindow);
	}
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
