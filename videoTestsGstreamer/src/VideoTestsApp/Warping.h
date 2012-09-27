/*
 * Warping.h
 *
 *  Created on: 18/04/2011
 *      Author: arturo
 */

#ifndef WARPING_H_
#define WARPING_H_

#include <vector>
#include "ofEvents.h"
#include "ofPoint.h"
#include "ofPolyline.h"

namespace gui{
	class Warping {
	public:
		Warping(float minDistance=0);

		void setMinDistance(float minDistance);
		void setInitialQuad(const vector<ofPoint> & quad, const ofPoint & offset = ofPoint(), const ofPoint & _scaleXY = ofPoint(1,1));
		void setMaxQuad(const vector<ofPoint> & quad);
		void setOffset(const ofPoint & offset);
		void setScaleXY(const ofPoint & scaleXY);
		void enableEvents();
		void disableEvents();

		void update();
		void draw();

		void mousePressed(ofMouseEventArgs & mouse);
		void mouseDragged(ofMouseEventArgs & mouse);
		void mouseReleased(ofMouseEventArgs & mouse);
		void mouseMoved(ofMouseEventArgs & mouse){}

		ofPolyline getQuad();
		ofPolyline getQuad(const ofPoint & offset, const ofVec2f & scale);

	private:
		ofPolyline quad;
		ofPolyline maxQuad;
		ofPoint offset;
		ofPoint scaleXY;
		float minDistance;
		int selectedPoint;
	};
}

#endif /* WARPING_H_ */
