/*
 * Wall.cpp
 *
 *  Created on: Sep 11, 2012
 *      Author: arturo
 */

#include "Wall.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"


ofMesh generateBuilding(float sizeW, float sizeH, float sizeD, bool fill){
	ofMesh vertexData;
	if(ofGetCoordHandedness() == OF_LEFT_HANDED){
		ofScale(1, 1, -1);
	}
	float w = sizeW * .5;
	float h = sizeH * .5;
	float d = sizeD * .5;

	if(fill){
		ofVec3f vertices[] = {
			ofVec3f(+w,-h,+d), ofVec3f(+w,-h,-d), ofVec3f(+w,+h,-d), ofVec3f(+w,+h,+d),
			ofVec3f(+w,+h,+d), ofVec3f(+w,+h,-d), ofVec3f(-w,+h,-d), ofVec3f(-w,+h,+d),
			ofVec3f(+w,+h,+d), ofVec3f(-w,+h,+d), ofVec3f(-w,-h,+d), ofVec3f(+w,-h,+d),
			ofVec3f(-w,-h,+d), ofVec3f(-w,+h,+d), ofVec3f(-w,+h,-d), ofVec3f(-w,-h,-d),
			ofVec3f(-w,-h,+d), ofVec3f(-w,-h,-d), ofVec3f(+w,-h,-d), ofVec3f(+w,-h,+d),
			ofVec3f(-w,-h,-d), ofVec3f(-w,+h,-d), ofVec3f(+w,+h,-d), ofVec3f(+w,-h,-d)
		};
		vertexData.addVertices(vertices,24);

		static ofVec2f tex[] = {
			ofVec2f(0,0), ofVec2f(.25,0), ofVec2f(.25,1), ofVec2f(0,1),
			ofVec2f(0,0), ofVec2f(0,1), ofVec2f(0,0), ofVec2f(0,0),
			ofVec2f(1,1), ofVec2f(.75,1), ofVec2f(.75,0), ofVec2f(1,0),
			ofVec2f(.75,0), ofVec2f(.75,1),  ofVec2f(.50,1),ofVec2f(.50,0),
			ofVec2f(0,0), ofVec2f(0,0), ofVec2f(0,0), ofVec2f(0,0),
			ofVec2f(.50,0), ofVec2f(.5,1), ofVec2f(.25,1), ofVec2f(.25,0)
		};
		vertexData.addTexCoords(tex,24);

		static ofIndexType indices[] = {
			0,1,2, // right top left
			0,2,3, // right bottom right
			4,5,6, // bottom top right
			4,7,6, // bottom bottom left
			8,9,10, // back bottom right
			8,10,11, // back top left
			12,13,14, // left bottom right
			12,14,15, // left top left
			16,17,18, // ... etc
			16,18,19,
			20,21,22,
			20,22,23
		};
		vertexData.addIndices(indices,36);
		vertexData.setMode(OF_PRIMITIVE_TRIANGLES);
	} else {
		ofVec3f vertices[] = {
			ofVec3f(+w,+h,+d),
			ofVec3f(+w,+h,-d),
			ofVec3f(+w,-h,+d),
			ofVec3f(+w,-h,-d),
			ofVec3f(-w,+h,+d),
			ofVec3f(-w,+h,-d),
			ofVec3f(-w,-h,+d),
			ofVec3f(-w,-h,-d)
		};
		vertexData.addVertices(vertices,8);

		static ofIndexType indices[] = {
			0,1, 1,3, 3,2, 2,0,
			4,5, 5,7, 7,6, 6,4,
			0,4, 5,1, 7,3, 6,2
		};
		vertexData.addIndices(indices,24);
		vertexData.setMode(OF_PRIMITIVE_LINES);
	}

	return vertexData;
}


void Wall::setAudioManager(AudioManager & _audio){
	audio = &_audio;
	audio->setLeds(strips);
}

void Wall::setup(){
	parameters.setName("Wall");
	parameters.add(w.set("w",180,0,200));
	parameters.add(h.set("h",100,0,200));
	parameters.add(useColors.set("useColors",false));
	parameters.add(radiusScale.set("radiusScale",1,0.1,3));
	parameters.add(z.set("z",.7,.5,2));
	parameters.add(secondScreenPos.set("secondScreenPos",ofVec2f(1280,0),ofVec2f(-2560,0),ofVec2f(2560,768)));
	parameters.add(renderMode.set("renderMode",Continuous,Continuous,NumModes-1));
	parameters.add(testStateMillis.set("testStateMillis",1000,100,10000));
	parameters.add(muted.set("muted",false));
	parameters.add(showTweets.set("showTweets",false));

	rotation3D.set("rotation3D",ofVec2f(0,0),ofVec2f(-180,-180),ofVec2f(180,180));
	vizX.set("vizX",250,0,300);
	renderW.set("renderW",1024,256,2048);
	renderH.set("renderH",512,256,2048);

	w.addListener(this,&Wall::sizeChanged);
	h.addListener(this,&Wall::sizeChanged);
	renderMode.addListener(this,&Wall::sizeChanged);
	showTweets.addListener(this,&Wall::showTweetsChanged);

	enableMouseEvents();

	runningTest = false;
	
	font.loadFont("verdana.ttf",40,true,false);

	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	glEnable(GL_POINT_SMOOTH);

	reset();
}


void Wall::enableMouseEvents(){
	ofAddListener(ofEvents().mousePressed,this,&Wall::mousePressed);
	ofAddListener(ofEvents().mouseDragged,this,&Wall::mouseDragged);
}

void Wall::disableMouseEvents(){
	ofRemoveListener(ofEvents().mousePressed,this,&Wall::mousePressed);
	ofRemoveListener(ofEvents().mouseDragged,this,&Wall::mouseDragged);
}

void Wall::sizeChanged(int & size){
	reset();
}

void Wall::showTweetsChanged(bool & showTweets){
	if(!showTweets){
		while(!tweets.empty()) tweets.pop();
		for(u_int i=0;i<strips.size();i++){
			strips[i].clearColorCoords();
		}
	}else{
		colorsFromTweets.assign(h,ofFloatColor(0,0,0,255));
		bursts.clear();
		for(u_int i=0;i<strips.size();i++){
			strips[i].setColorCoords(colorsFromTweets);
		}
	}
}

void Wall::reset(){
	strips.resize(w);
	float stripRadius = 1./(float((strips.size()+1)*2.));
	float stripSize = stripRadius*2;
	for(u_int i=0;i<strips.size();i++){
		strips[i].setup(h,(i+1)*stripSize,stripRadius);
	}
	building = generateBuilding(renderW,renderH,renderW,true);
	buildingWireframe = generateBuilding(renderW+3,renderH+3,renderW+3,false);

	outputFBO.allocate(w,h,GL_RGBA);
	outputBuffer.allocate(w,h,4);
	colorsFromTweets.resize(h);


	ofFbo::Settings settings;
	settings.width = renderW*2;
	settings.height = renderH*.5;
	settings.numSamples = 8;
	settings.internalformat = GL_RGBA;
	renderFbo.allocate(settings);
	ofEnableNormalizedTexCoords();
}

void Wall::update(){
	u_long now = ofGetElapsedTimeMillis();
	if(runningTest){
		u_long msStripOn = double(testStateMillis)/double(w);
		u_long currentTestEndTime = prevTestEndTime+testStateMillis;
		if(testState>=9){
			nextStripOn = double(now-prevTestEndTime)/double(testStateMillis)*w;
		}else{
			if(now-lastTimeStripChangedTest>=msStripOn){
				prevStripOn = nextStripOn;
				nextStripOn++;
				lastTimeStripChangedTest = now;
			}
		}
		switch(testState){
		case AllRed:
			for(u_int i=0;i<strips.size();++i){
				strips[i].trigger(ofColor::red,now);
			}
			if(now>=currentTestEndTime){
				testState++;
				prevTestEndTime = now;
			}
			break;
		case AllGreen:
			for(u_int i=0;i<strips.size();++i){
				strips[i].trigger(ofColor::green,now);
			}
			if(now>=currentTestEndTime){
				testState++;
				prevTestEndTime = now;
			}
			break;
		case AllBlue:
			for(u_int i=0;i<strips.size();++i){
				strips[i].trigger(ofColor::blue,now);
			}
			if(now>=currentTestEndTime){
				testState++;
				prevTestEndTime = now;
			}
			break;
		case AllWhite:
			for(u_int i=0;i<strips.size();++i){
				strips[i].trigger(ofColor::white,now);
			}
			if(now>=currentTestEndTime){
				testState++;
				prevStripOn = -1;
				nextStripOn = 0;
				lastTimeStripChangedTest = now + msStripOn;
				for(u_int i=0;i<strips.size();++i){
					strips[i].trigger(ofColor::black,now);
				}
				prevTestEndTime = now;
			}
			break;
		case RedOneByOne:
			if(prevStripOn>0) strips[prevStripOn].trigger(ofColor::black,now);
			if(nextStripOn>=(u_int)w){
				testState++;
				prevStripOn = -1;
				nextStripOn = 0;
				lastTimeStripChangedTest = now + msStripOn;
				prevTestEndTime = now;
			}else{
				strips[nextStripOn].trigger(ofColor::red,now);
			}
			break;
		case GreenOneByOne:
			if(prevStripOn>0) strips[prevStripOn].trigger(ofColor::black,now);
			if(nextStripOn>=(u_int)w){
				testState++;
				prevStripOn = -1;
				nextStripOn = 0;
				lastTimeStripChangedTest = now + msStripOn;
				prevTestEndTime = now;
			}else{
				strips[nextStripOn].trigger(ofColor::green,now);
			}
			break;
		case BlueOneByOne:
			if(prevStripOn>0) strips[prevStripOn].trigger(ofColor::black,now);
			if(nextStripOn>=(u_int)w){
				testState++;
				prevStripOn = -1;
				nextStripOn = 0;
				lastTimeStripChangedTest = now + msStripOn;
				prevTestEndTime = now;
			}else{
				strips[nextStripOn].trigger(ofColor::blue,now);
			}
			break;
		case WhiteOneByOne:
			if(prevStripOn>0) strips[prevStripOn].trigger(ofColor::black,now);
			if(nextStripOn>=(u_int)w){
				testState++;
				prevStripOn = -1;
				nextStripOn = 0;
				lastTimeStripChangedTest = now + msStripOn;
				lastStripTestOn = 0;
				prevTestEndTime = now;
			}else{
				strips[nextStripOn].trigger(ofColor::white,now);
			}
			break;
		case ProgressiveRed:
			for(u_int i=lastStripTestOn;i<nextStripOn && i<(u_int)w;i++){
				strips[i].trigger(ofColor::red,now);
			}
			lastStripTestOn = min((int)nextStripOn,(int)w);
			if(now>=currentTestEndTime){
				testState++;
				lastStripTestOn = 0;
				prevTestEndTime = now;
			}
			break;
		case ProgressiveGreen:
			for(u_int i=lastStripTestOn;i<nextStripOn && i<(u_int)w;i++){
				strips[i].trigger(ofColor::green,now);
			}
			lastStripTestOn = min((int)nextStripOn,(int)w);
			if(now>=currentTestEndTime){
				testState++;
				lastStripTestOn = 0;
				prevTestEndTime = now;
			}
			break;
		case ProgressiveBlue:
			for(u_int i=lastStripTestOn;i<nextStripOn && i<(u_int)w;i++){
				strips[i].trigger(ofColor::blue,now);
			}
			lastStripTestOn = min((int)nextStripOn,(int)w);
			if(now>=currentTestEndTime){
				testState++;
				lastStripTestOn = 0;
				prevTestEndTime = now;
			}
			break;
		case ProgressiveWhite:
			for(u_int i=lastStripTestOn;i<nextStripOn && i<(u_int)w;i++){
				strips[i].trigger(ofColor::white,now);
			}
			lastStripTestOn = min((int)nextStripOn,(int)w);
			if(now>=currentTestEndTime){
				testState=0;
				lastStripTestOn = 0;
				runningTest = false;
				for(u_int i=0;i<strips.size();++i){
					strips[i].setTestMode(false);
				}
			}
			break;
			break;
		}
	}else if(!audio->audioTest){
		if(!showTweets){
			vector<list<EnergyBurst>::iterator > toDelete;
			list<EnergyBurst>::iterator it;
			for(it = bursts.begin(); it!=bursts.end(); ++it){
				EnergyBurst & burst = *it;
				burst.update(now);
				if(!burst.alive){
					toDelete.push_back(it);
				}else{
					for(u_int i=0;i<strips.size();++i){
						if(!burst.triggeredAlready(strips[i])){
							if((strips[i].getPosition()<burst.currentPositionR.x && strips[i].getPosition()>=burst.startPosition.x) ||
								(burst.rightHasCycled() && strips[i].getPosition()<burst.currentPositionR.x))
								burst.trigger(strips[i]);
							if((strips[i].getPosition()>burst.currentPositionL.x && strips[i].getPosition()<=burst.startPosition.x) ||
								(burst.leftHasCycled() && strips[i].getPosition()>burst.currentPositionL.x))
								burst.trigger(strips[i]);
						}
					}
				}
			}
			for(u_int i=0;i<toDelete.size();++i){
				bursts.erase(toDelete[i]);
			}

			for(u_int i=0;i<strips.size();++i){
				strips[i].update(now);
			}
		}else{
			if(!tweets.empty()){
				if(!tweets.front().isAlive()) tweets.pop();
				audio->setCurrentTweet(&tweets.front());
				tweets.front().update();
			}else{
				audio->setCurrentTweet(NULL);
			}
		}
	}
}

void Wall::draw(){
	if(showTweets && !tweets.empty()){
		outputFBO.begin();
		ofClear(0,255);
		tweets.front().draw();
		outputFBO.end();
		outputFBO.readToPixels(outputBuffer);
		int stride = w*4;
		for(int i=0;i<w;i++){
			int index = i*4;
			for(int j=0;j<h;j++){
				colorsFromTweets[j] = *((ofFloatColor*)&outputBuffer[index]);
				index+=stride;
			}
			strips[i].setColorCoords(colorsFromTweets);
		}
	}
	switch (renderMode){
	case Continuous:{
		ofRectangle viewport(vizX,(ofGetHeight()-renderH)*.5,renderW,renderH);
		ofNoFill();
		ofSetColor(200);
		ofRect(viewport);

		ofFill();
		ofSetColor(255);
		ofPushView();
		ofViewport(viewport);
		for(u_int i=0;i<strips.size();i++){
			strips[i].draw(strips[i].getPosition(),1,ofGetWidth(),ofGetHeight());
		}
		ofPopView();
	}break;
	case Separate:{
		ofRectangle viewport(vizX,float(ofGetHeight()-renderH)/3.,renderW*.5,renderH*.5);
		ofNoFill();
		ofSetColor(200);
		ofRect(viewport);

		ofFill();
		ofSetColor(255);
		ofPushView();
		ofViewport(viewport);
		for(u_int i=0;i<strips.size()*.25;i++){
			strips[i].draw(strips[i].getPosition()*4,radiusScale);
		}
		ofPopView();


		viewport.x += renderW*.5+20;
		ofNoFill();
		ofSetColor(200);
		ofRect(viewport);

		ofFill();
		ofSetColor(255);
		ofPushView();
		ofViewport(viewport);
		ofPushMatrix();
		for(u_int i=strips.size()*.25;i<strips.size()*.5;i++){
			strips[i].draw((strips[i].getPosition()-.25)*4,radiusScale);
		}
		ofPopMatrix();
		ofPopView();


		viewport.x -= renderW*.5+20;
		viewport.y += float(ofGetHeight()-renderH)/3. + renderH*.5;
		ofNoFill();
		ofSetColor(200);
		ofRect(viewport);

		ofFill();
		ofSetColor(255);
		ofPushView();
		ofViewport(viewport);
		ofPushMatrix();
		for(u_int i=strips.size()*.5;i<strips.size()*.75;i++){
			strips[i].draw((strips[i].getPosition()-.5)*4,radiusScale);
		}
		ofPopMatrix();
		ofPopView();


		viewport.x += renderW*.5+20;
		ofNoFill();
		ofSetColor(200);
		ofRect(viewport);

		ofFill();
		ofSetColor(255);
		ofPushView();
		ofViewport(viewport);
		ofPushMatrix();
		for(u_int i=strips.size()*.75;i<strips.size();i++){
			strips[i].draw((strips[i].getPosition()-.75)*4,radiusScale);
		}
		ofPopMatrix();
		ofPopView();
	}break;
	case ThreeD:{
		renderFbo.begin();
		ofClear(0,255);

		ofFill();
		ofSetColor(255);
		for(u_int i=0;i<strips.size();i++){
			strips[i].draw(strips[i].getPosition(),radiusScale,renderFbo.getWidth(),renderFbo.getHeight());
		}

		ofDrawBitmapString("0",20,20);
		ofDrawBitmapString("1",renderFbo.getWidth()*.25+20,20);
		ofDrawBitmapString("2",renderFbo.getWidth()*.5+20,20);
		ofDrawBitmapString("3",renderFbo.getWidth()*.75+20,20);

		renderFbo.end();

		glEnable(GL_DEPTH_TEST);
		ofPushView();
		ofRectangle viewport(vizX,(ofGetHeight()-renderH)*.5,renderW,renderH);
		//ofTranslate(vizX,(ofGetHeight()-renderH)*.5);
		ofViewport(viewport);
		ofSetupScreenPerspective(viewport.width, viewport.height);
		ofTranslate(ofPoint(renderW*.5,viewport.height*.5,-renderW*z));
		ofRotateX(rotation3D->y);
		ofRotateY(rotation3D->x);
		ofFill();
		renderFbo.getTextureReference().bind();
		building.draw();
		renderFbo.getTextureReference().unbind();
		ofNoFill();
		buildingWireframe.draw();
		ofPopView();
		glDisable(GL_DEPTH_TEST);
		}break;
	}
	
	drawActiveArea((RenderMode)(int)renderMode);
	drawOutput();

	if(muted){
		ofPushStyle();
		ofSetColor(255,0,0);
		font.drawString("Muted",vizX+20,80);
		ofPopStyle();
	}
}

void Wall::drawActiveArea(RenderMode renderMode){
	switch(renderMode){
		case Separate:{
			ofRectangle viewport(vizX,(ofGetHeight()-renderH)*.5,renderW,renderH);
			ofPushView();
			ofViewport(viewport);
			list<EnergyBurst>::iterator it;
			for(it = bursts.begin(); it!=bursts.end(); ++it){
				it->draw();
			}
			for(int i=0;i<5;i++){
				float x = ofGetWidth()*.25*i;
				ofLine(x,ofGetHeight()*.5-10,x,ofGetHeight()*.5+10);
			}
			ofPopView();
		}break;
		case ThreeD:{
			ofRectangle viewport(vizX,0,renderW,ofGetHeight());
			ofPushView();
			ofViewport(viewport);
			ofTranslate(0,ofGetHeight()*.5-40);
			list<EnergyBurst>::iterator it;
			for(it = bursts.begin(); it!=bursts.end(); ++it){
				it->draw();
			}
			for(int i=0;i<5;i++){
				float x = ofGetWidth()*.25*i;
				ofLine(x,ofGetHeight()*.5-10,x,ofGetHeight()*.5+10);
			}
			ofPopView();
		}break;

	}
}

void Wall::drawOutput(){
	ofSetColor(0);
	ofRect(secondScreenPos->x,0,ofGetWidth()-secondScreenPos->x,ofGetHeight());

	ofSetColor(255);
	if(!muted){
		ofPushMatrix();
		ofTranslate((ofVec2f)secondScreenPos);

		if(!showTweets){
			for(int i=0;i<w;i++){
				ofSetColor(strips[i].getColor());
				ofLine(i,0,i,h);
			}
		}else{
			if(!tweets.empty()){
				ofPushView();
				ofViewport(secondScreenPos->x,secondScreenPos->y,w,h);
				ofSetupScreenPerspective(w,h);
				tweets.front().draw();
				ofPopView();
			}
		}
		ofPopMatrix();

		ofSetColor(255);
	}
}


ofColor Wall::niceRandomColor(){
	ofColor c;
	unsigned char hue = ofRandom(255);
	unsigned char sat = ofRandom(190,256);
	unsigned char bri = ofRandom(190,256);
	c.setHsb(hue,sat,bri);
	return c;
}

void Wall::mousePressed(ofMouseEventArgs & mouse){
	switch(renderMode){
	case Continuous:
		if(ofRectangle(vizX,(ofGetHeight()-renderH)*.5,renderW,renderH).inside(mouse.x,mouse.y)){
			energyBurst(float(mouse.x-vizX)/float(renderW), float(mouse.y)/float(ofGetHeight()));
		}
		break;
	case Separate:
		if(ofRectangle(vizX,ofGetHeight()*.5-15,renderW,30).inside(mouse.x,mouse.y)){
			energyBurst(float(mouse.x-vizX)/float(renderW), float(mouse.y)/float(ofGetHeight()));
		}
		break;
	case ThreeD:
		if(ofRectangle(vizX,(ofGetHeight()-renderH)*.5,renderW,renderH-50).inside(mouse.x,mouse.y)){
			dragStart.set(mouse.x,mouse.y);
			startRotation3D = rotation3D;
		}
		if(ofRectangle(vizX,ofGetHeight()-50,renderW,50).inside(mouse.x,mouse.y)){
			energyBurst(float(mouse.x-vizX)/float(renderW), float(mouse.y)/float(ofGetHeight()));
		}
		break;

	}

}

void Wall::mouseDragged(ofMouseEventArgs & mouse){
	if(ofRectangle(vizX,(ofGetHeight()-renderH)*.5,renderW,renderH-50).inside(mouse.x,mouse.y) && renderMode==ThreeD){
		ofVec2f delta = ofVec2f(mouse.x,mouse.y)-dragStart;
		rotation3D = startRotation3D + ofVec2f(ofMap(delta.x,-(ofGetWidth()-vizX), ofGetWidth()-vizX, -180, 180),ofMap(delta.y,-ofGetHeight(), ofGetHeight(), 180, -180));
	}
}

void Wall::energyBurst(float x, float y){
	if(!runningTest && !audio->audioTest){
		bursts.push_back(EnergyBurst(ofVec2f(x,.5),useColors ? niceRandomColor() : ofColor::white));
	}
}


void Wall::newTweet(string text){
	if(!runningTest && !audio->audioTest){
		if(showTweets){
			u_long now = ofGetElapsedTimeMillis();
			tweets.push(TweetText(text,useColors ? niceRandomColor() : ofColor::white,now,w));
		}else{
			energyBurst(ofRandom(1),.5);
		}

	}
}

void Wall::startTest(){
	u_long now = ofGetElapsedTimeMillis();
	for(u_int i=0;i<strips.size();++i){
		strips[i].setTestMode(true);
		strips[i].trigger(ofColor::black,now);
	}
	testState = AllRed;
	testStartTime = now;
	runningTest = true;
	lastStripTestOn = 0;
	nextStripOn = 0;
	prevTestEndTime = now;
}
