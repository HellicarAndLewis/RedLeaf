#include "testApp.h"
//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	axis = ofPtr<ofxAxisGrabber>(new ofxAxisGrabber);
	axis->setCameraAddress("10.42.0.23");
	axis->setParametersRefreshRate(5000);
	axis->setDesiredFrameRate(30);
	axis->setCodec(ofxAxisGrabber::H264);
	axis->setAuth("root","asdqwe");
	grabber.setGrabber(axis);
	grabber.initGrabber(640,480);

	gui.setup("","settings.xml",650,10);
	gui.add(autofocus.setup("autofocus"));
	gui.add(showFocusWindow.set("showFocusWindow",false));
	gui.add(record.set("record",false));
	gui.add(axis->parameters);
	gui.getGroup("10.42.0.23").getIntSlider("focus").setUpdateOnReleaseOnly(true);

	autofocus.addListener(this,&testApp::autofocusPressed);
	showFocusWindow.addListener(this,&testApp::showFocusWindowChanged);
	record.addListener(this,&testApp::recordPressed);
}

void testApp::autofocusPressed(bool & pressed){
	axis->triggerAutoFocus();
}


void testApp::showFocusWindowChanged(bool & sfw){
	if(sfw) focusWindow = axis->getFocusWindow();
}

void testApp::recordPressed(bool & record){
	if(record){
		gstRecorder.setup(640,480,24,ofGetTimestampString()+".mov",ofxGstVideoRecorder::JPEG,30);
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
		if(record){
			gstRecorder.newFrame(grabber.getPixelsRef());
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	grabber.draw(0,0);
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
