#include "testApp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTime.h"

//--------------------------------------------------------------
void RedLeafApp::setup(){
    ofSetFrameRate(30.f);
    
    cPixel = ofColor::red; //red to start with...
    
    xPixel = 1440; //TOP right to start with on second head, -1 as we are going from 0
    yPixel = 0;
    
    widthPixel = 1280;
    heightPixel = 1024;
    
    textMode = false;
    boxMode = true;
}

//--------------------------------------------------------------
void RedLeafApp::update(){

}

//--------------------------------------------------------------
void RedLeafApp::draw(){
    ofBackground(ofColor::black);
    
    ofSetColor(cPixel);
    
    if(textMode){
        ofDrawBitmapString("Leaf", xPixel, yPixel);
    }else{
        if(boxMode){
            //drawTestBox();
            drawTestSides();
        }else{
            ofRect(xPixel, yPixel, widthPixel, heightPixel);
        }
    }
    
    ofSetColor(ofColor::white);
    
	float textX = 0;
	Poco::LocalDateTime date;
	ofDrawBitmapString(Poco::DateTimeFormatter::format(date,Poco::DateTimeFormat::ASCTIME_FORMAT ),textX,20);
	ofDrawBitmapString("x to switch pixel/box, t to draw text, f to full screen, m to take mouse pos, z to reset, wasd to move pixel, ijkl to scale pixel, r,g,b to change colour of pixel. App fps: " + ofToString((int)ofGetFrameRate()),textX,40);
    
    ofSetColor(cPixel);
    ofDrawBitmapString("Pixel is at" + ofToString(xPixel) + "," + ofToString(yPixel) + "Width:" + ofToString(widthPixel) + ", Height:" + ofToString(heightPixel),textX,60);
    
    if(textMode){
        ofDrawBitmapString("TEXT MODE",textX,80);
    }else{
        if(boxMode){
            ofDrawBitmapString("BOX MODE",textX,80);
        }else{
            ofDrawBitmapString("PIXEL MODE",textX,80);
        }
        
    }
}

void RedLeafApp::drawTestBox(){
    float halfWidth = widthPixel/2.f;
    float halfHeight = heightPixel/2.f;
    
    ofSetColor(ofColor::red);
    ofRect(xPixel, yPixel, halfWidth, halfHeight);
    ofSetColor(ofColor::blue);
    ofRect(xPixel+halfWidth, yPixel, halfWidth, halfHeight);
    ofSetColor(ofColor::green);
    ofRect(xPixel, yPixel+halfHeight, halfWidth, halfHeight);
    ofSetColor(ofColor::yellow);
    ofRect(xPixel+halfWidth, yPixel+halfHeight, halfWidth, halfHeight);
}

void RedLeafApp::drawTestSides(){
    float halfWidth = widthPixel/2.f;
    float halfHeight = heightPixel/2.f;
    float quarterWidth = widthPixel/4.f;
    float fullHeight = heightPixel;
    
    ofSetColor(ofColor::red);
    ofRect(xPixel, yPixel, quarterWidth, fullHeight);
    ofSetColor(ofColor::blue);
    ofRect(xPixel+quarterWidth, yPixel, quarterWidth, fullHeight);
    ofSetColor(ofColor::green);
    ofRect(xPixel+quarterWidth+quarterWidth, yPixel, quarterWidth, fullHeight);
    ofSetColor(ofColor::yellow);
    ofRect(xPixel+quarterWidth+quarterWidth+quarterWidth, yPixel, quarterWidth, fullHeight);
}

//--------------------------------------------------------------
void RedLeafApp::keyPressed(int key){
    switch (key) {
        case 'l':{
            widthPixel++;
            break;
        }
        case 'j':{
            widthPixel--;
            break;
        }
        case 'i':{
            heightPixel--;
            break;
        }
        case 'k':{
            heightPixel++;
            break;
        }
        case 'd':{
            xPixel++;
            break;
        }
        case 'a':{
            xPixel--;
            break;
        }
        case 'w':{
            yPixel--;
            break;
        }
        case 's':{
            yPixel++;
            break;
        }
        case 'D':{
            xPixel+=10;
            break;
        }
        case 'A':{
            xPixel-=10;
            break;
        }
        case 'W':{
            yPixel-=10;
            break;
        }
        case 'S':{
            yPixel+=10;
            break;
        }
        case 'z':{
            xPixel = 1440+1280-1;
            yPixel = 0;
            widthPixel = 1;
            heightPixel = 1;
        }
        case 'r':{
            cPixel = ofColor::red;
            break;
        }
        case 'g':{
            cPixel = ofColor::green;
            break;
        }
        case 'b':{
            cPixel = ofColor::blue;
            break;
        }
        case 'f':{
            ofToggleFullscreen();
            break;
        }
        case 'm':{
            xPixel = mouseX;
            yPixel = mouseY;
            break;
        }
        case 't':{
            textMode = !textMode;
            break;
        }
        case 'x':{
            boxMode = !boxMode;
            break;
        }
    }
}

//--------------------------------------------------------------
void RedLeafApp::keyReleased(int key){

}

//--------------------------------------------------------------
void RedLeafApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void RedLeafApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void RedLeafApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void RedLeafApp::mouseReleased(int x, int y, int button){

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
