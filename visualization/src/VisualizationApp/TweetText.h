/*
 * TweetText.h
 *
 *  Created on: Sep 28, 2012
 *      Author: arturo
 */

#ifndef TWEETTEXT_H_
#define TWEETTEXT_H_

#include "ofConstants.h"
#include "ofParameter.h"
#include "ofParameterGroup.h"
#include "ofColor.h"
#include "ofVec2f.h"
#include "ofTrueTypeFont.h"


class TweetText {
public:
	TweetText(const string & tweet, const ofColor & color, u_long now, int x);

	void update();
	void draw();
	bool isAlive();

	void setAmplitude(float amp);



	static ofParameter<float> speedPixelsPerSec;
	static ofParameter<int> y;
	static ofParameter<int> limitScreenX;
	static ofParameter<int> fontSize;
	static ofParameter<bool> useColors;
	static ofParameter<float> alpha;
	static ofTrueTypeFont font;
	static string fontName;


private:
	string tweet;
	ofColor triggerColor,color;
	u_long time;
	ofVec2f position;
};

#endif /* TWEETTEXT_H_ */
