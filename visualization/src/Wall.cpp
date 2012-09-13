/*
 * Wall.cpp
 *
 *  Created on: Sep 11, 2012
 *      Author: arturo
 */

#include "Wall.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"


static ofMesh vertexData;
void ofBox(const ofPoint& position, float sizeW, float sizeH, float sizeD){
	ofPushMatrix();
	ofTranslate(position);
	if(ofGetCoordHandedness() == OF_LEFT_HANDED){
		ofScale(1, 1, -1);
	}
	float w = sizeW * .5;
	float h = sizeH * .5;
	float d = sizeD * .5;

	vertexData.clear();
	if(ofGetStyle().bFill){
		ofVec3f vertices[] = {
			ofVec3f(+w,-h,+d), ofVec3f(+w,-h,-d), ofVec3f(+w,+h,-d), ofVec3f(+w,+h,+d),
			ofVec3f(+w,+h,+d), ofVec3f(+w,+h,-d), ofVec3f(-w,+h,-d), ofVec3f(-w,+h,+d),
			ofVec3f(+w,+h,+d), ofVec3f(-w,+h,+d), ofVec3f(-w,-h,+d), ofVec3f(+w,-h,+d),
			ofVec3f(-w,-h,+d), ofVec3f(-w,+h,+d), ofVec3f(-w,+h,-d), ofVec3f(-w,-h,-d),
			ofVec3f(-w,-h,+d), ofVec3f(-w,-h,-d), ofVec3f(+w,-h,-d), ofVec3f(+w,-h,+d),
			ofVec3f(-w,-h,-d), ofVec3f(-w,+h,-d), ofVec3f(+w,+h,-d), ofVec3f(+w,-h,-d)
		};
		vertexData.addVertices(vertices,24);

		static ofVec3f normals[] = {
			ofVec3f(+1,0,0), ofVec3f(+1,0,0), ofVec3f(+1,0,0), ofVec3f(+1,0,0),
			ofVec3f(0,+1,0), ofVec3f(0,+1,0), ofVec3f(0,+1,0), ofVec3f(0,+1,0),
			ofVec3f(0,0,+1), ofVec3f(0,0,+1), ofVec3f(0,0,+1), ofVec3f(0,0,+1),
			ofVec3f(-1,0,0), ofVec3f(-1,0,0), ofVec3f(-1,0,0), ofVec3f(-1,0,0),
			ofVec3f(0,-1,0), ofVec3f(0,-1,0), ofVec3f(0,-1,0), ofVec3f(0,-1,0),
			ofVec3f(0,0,-1), ofVec3f(0,0,-1), ofVec3f(0,0,-1), ofVec3f(0,0,-1)
		};
		vertexData.addNormals(normals,24);

		static ofVec2f tex[] = {
			ofVec2f(0,0), ofVec2f(.25,0), ofVec2f(.25,1), ofVec2f(0,1),
			ofVec2f(0,0), ofVec2f(0,1), ofVec2f(0,0), ofVec2f(0,0),
			ofVec2f(1,1), ofVec2f(.75,1), ofVec2f(.75,0), ofVec2f(1,0),
			ofVec2f(.75,0), ofVec2f(.75,1),  ofVec2f(.50,1),ofVec2f(.50,0),
			ofVec2f(0,0), ofVec2f(0,0), ofVec2f(0,0), ofVec2f(0,0),
			ofVec2f(.50,0), ofVec2f(.5,1), ofVec2f(.25,1), ofVec2f(.25,0),
			/*ofVec2f(.50,1), ofVec2f(.5,0), ofVec2f(.25,0), ofVec2f(.25,1),
			ofVec2f(.50,1), ofVec2f(.75,1), ofVec2f(.75,0), ofVec2f(.50,0),
			ofVec2f(.25,0), ofVec2f(0,0), ofVec2f(0,1), ofVec2f(.25,1),
			ofVec2f(.50,1), ofVec2f(.75,1), ofVec2f(.75,0), ofVec2f(.50,0),
			ofVec2f(.75,0), ofVec2f(.75,1), ofVec2f(1,1), ofVec2f(1,0),*/
		};
		vertexData.addTexCoords(tex,24);

		static ofIndexType indices[] = {
			0,1,2, // right top left
			0,2,3, // right bottom right
			4,5,6, // bottom top right
			4,6,7, // bottom bottom left
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
		ofGetCurrentRenderer()->draw(vertexData,vertexData.usingColors(),vertexData.usingTextures(),vertexData.usingNormals());
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

		static float n = sqrtf(3);
		static ofVec3f normals[] = {
			ofVec3f(+n,+n,+n),
			ofVec3f(+n,+n,-n),
			ofVec3f(+n,-n,+n),
			ofVec3f(+n,-n,-n),
			ofVec3f(-n,+n,+n),
			ofVec3f(-n,+n,-n),
			ofVec3f(-n,-n,+n),
			ofVec3f(-n,-n,-n)
		};
		vertexData.addNormals(normals,8);

		static ofIndexType indices[] = {
			0,1, 1,3, 3,2, 2,0,
			4,5, 5,7, 7,6, 6,4,
			0,4, 5,1, 7,3, 6,2
		};
		vertexData.addIndices(indices,24);

		vertexData.setMode(OF_PRIMITIVE_LINES);
		ofGetCurrentRenderer()->draw(vertexData, vertexData.usingColors(),vertexData.usingTextures(),vertexData.usingNormals());
	}


	ofPopMatrix();
}

void Wall::setup(){
	parameters.setName("Wall");
	parameters.add(w.set("w",180,0,200));
	parameters.add(h.set("h",100,0,200));
	parameters.add(radiusScale.set("radiusScale",1,0.1,3));
	parameters.add(z.set("z",.7,.5,2));
	vizX.set("vizX",250,0,300);
	renderW.set("renderW",1024,256,2048);
	renderH.set("renderH",512,256,2048);
	parameters.add(renderMode.set("renderMode",Continuous,Continuous,NumModes-1));
	parameters.add(rotation3D.set("rotation3D",ofVec2f(0,0),ofVec2f(-180,-180),ofVec2f(180,180)));
	w.addListener(this,&Wall::sizeChanged);
	h.addListener(this,&Wall::sizeChanged);
	renderMode.addListener(this,&Wall::sizeChanged);

	ofAddListener(ofEvents().mousePressed,this,&Wall::mousePressed);
	ofAddListener(ofEvents().mouseDragged,this,&Wall::mouseDragged);

	reset();
}

void Wall::sizeChanged(int & size){
	reset();
}

void Wall::reset(){
	strips.resize(w);
	float stripRadius = 1./(float((strips.size()+1)*2.));
	float stripSize = stripRadius*2;
	for(u_int i=0;i<strips.size();i++){
		strips[i].setup(h,(i+1)*stripSize,stripRadius);
	}

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
}

void Wall::draw(){
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

		viewport.set(vizX,float(ofGetHeight()-renderH)/3.,renderW*.5,renderH*.5);
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
		//ofTranslate(-renderW*.5,0);
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
		//ofTranslate(-renderW*.5,0);
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
		//ofTranslate(-renderW*.5,0);
		for(u_int i=strips.size()*.75;i<strips.size();i++){
			strips[i].draw((strips[i].getPosition()-.75)*4,radiusScale);
		}
		ofPopMatrix();
		ofPopView();
	}break;
	case ThreeD:
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

		renderFbo.begin();
		ofClear(0,255);

		ofFill();
		ofSetColor(255);
		//ofRectangle viewport(0,0,renderFbo.getWidth()*.25,renderFbo.getHeight());
		//ofViewport(viewport);
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
		viewport.set(vizX,(ofGetHeight()-renderH)*.5,renderW,renderH);
		ofViewport(viewport);
		ofTranslate(ofPoint(ofGetWidth()*.5,ofGetHeight()*.5,-renderW*z));
		ofRotateX(rotation3D->y);
		ofRotateY(rotation3D->x);
		ofFill();
		renderFbo.getTextureReference().bind();
		ofBox(ofPoint(0,0,0),renderW,renderH,renderW);
		renderFbo.getTextureReference().unbind();
		ofNoFill();
		ofBox(ofPoint(0,0,0),renderW+3,renderH+3,renderW+3);
		ofPopView();
		glDisable(GL_DEPTH_TEST);
		break;

	}
	//renderFbo.draw(vizX,20,renderFbo.getWidth()*.25,renderFbo.getHeight()*.25);
}

void Wall::mousePressed(ofMouseEventArgs & mouse){
	if(mouse.x>=vizX){
		dragStart.set(mouse.x,mouse.y);
		startRotation3D = rotation3D;
	}
}

void Wall::mouseDragged(ofMouseEventArgs & mouse){
	if(mouse.x>=vizX && renderMode==ThreeD){
		ofVec2f delta = ofVec2f(mouse.x,mouse.y)-dragStart;
		rotation3D = startRotation3D + ofVec2f(ofMap(delta.x,-(ofGetWidth()-vizX), ofGetWidth()-vizX, -180, 180),ofMap(delta.y,-ofGetHeight(), ofGetHeight(), 180, -180));
	}
}

void Wall::energyBurst(float x, float y){
	bursts.push_back(EnergyBurst(ofVec2f(x,.5)));
}
