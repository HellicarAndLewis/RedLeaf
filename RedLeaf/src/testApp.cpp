#include "testApp.h"

//--------------------------------------------------------------
void RedLeafApp::setup(){
	videoApp.setup();
	visualizationApp.setup();
	activeApp = &visualizationApp;
}

//--------------------------------------------------------------
void RedLeafApp::update(){
	videoApp.update();
	visualizationApp.update();
	for(u_int i=0;i<videoApp.cvModules.size();i++){
		if(videoApp.cvModules[i]->isFrameNew()){
			for(u_int j=0;j<videoApp.cvModules[i]->getTriggers().size();j++){
				visualizationApp.wall.energyBurst(ofMap(videoApp.cvModules[i]->getTriggers()[j],0,1,i*0.25,(i+1)*0.25),.5,ofColor::white);
			}
		}
	}
}

//--------------------------------------------------------------
void RedLeafApp::draw(){
	activeApp->draw();
	/*visualizationApp.draw();
	for(int i=0;i<videoApp.axisCameras.size();i++){
		videoApp.axisCameras[i]->draw(visualizationApp.wall.vizX+visualizationApp.wall.renderW-320,i*250+50,320,240);
	}*/
}

//--------------------------------------------------------------
void RedLeafApp::keyPressed(int key){
	if(key=='l') activeApp=&visualizationApp;
	if(key=='v') activeApp=&videoApp;
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
