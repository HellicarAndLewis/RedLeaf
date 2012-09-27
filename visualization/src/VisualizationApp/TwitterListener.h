/*
 * TwitterListener.h
 *
 *  Created on: Sep 10, 2012
 *      Author: arturo
 */

#ifndef TWITTERLISTENER_H_
#define TWITTERLISTENER_H_


#include "ofxTwitter.h"
#include "ofThread.h"

class TwitterListener: public ofThread {
public:
	void setup();
	void start();

	ofEvent<const Tweet> newTweetE;
	ofParameterGroup parameters;
	ofParameter<string> hashtag;
	ofParameter<int> checkEvery;

private:
	void threadedFunction();
	void processResult(const vector<Tweet> & results);

	ofxTwitter twitter;
	Poco::DateTime timeLastTweet;
};

#endif /* TWITTERLISTENER_H_ */
