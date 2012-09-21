#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	tex.allocate(1280,720,GL_RGB);
	//gst.setPipeline("rtspsrc location=rtsp://10.42.0.23:554/mjpg/media.amp latency=0 ! decodebin2 ! ffmpegcolorspace ! video/x-raw-rgb, width=(int)1280, height=(int)720, bpp=24, depth=24",24,true,1280,720);
	gst.loadMovie("http://10.42.0.23/axis-cgi/mjpg/video.cgi?fps=30&nbrofframes=0&resolution=320x240");
	//gst.loadMovie("rtsp://10.42.0.23:554/axis-media/media.amp?videocodec=h264");
	gst.play();
	ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void testApp::update(){
	gst.update();
	if(gst.isFrameNew()){

	}
}

//--------------------------------------------------------------
void testApp::draw(){
	//tex.loadData(gst.getPixelsRef());
	gst.draw(0,0);
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
