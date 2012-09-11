/*
 * TwitterListener.cpp
 *
 *  Created on: Sep 10, 2012
 *      Author: arturo
 */

#include "TwitterListener.h"


void TwitterListener::setup(){
	twitter.setup();
	twitter.setTimeout(20);
	timeLastTweet.assign(1970,1,1);
	parameters.add(hashtag.set("hashtag","#RedLeaf"));
	parameters.add(checkEvery.set("checkEvery",1000,500,20000));
}

void TwitterListener::start(){
	startThread();
}

void TwitterListener::processResult(const vector<Tweet> & results){
	for(int i=results.size()-1;i>=0;--i){
		if(results[i].updated>timeLastTweet){
			cout << results[i].title << ": " << ofToString(results[i].updated.year())+"/"+ofToString(results[i].updated.month())+"/"+ofToString(results[i].updated.day())+" " +ofToString(results[i].updated.hour())+":"+ofToString(results[i].updated.minute())+":"+ofToString(results[i].updated.second()) << endl;
			timeLastTweet = results[i].updated;
			ofNotifyEvent(newTweetE,results[i]);
		}
	}
}

void TwitterListener::threadedFunction(){
	while(isThreadRunning()){
		u_long time = ofGetElapsedTimeMillis();
		processResult(twitter.syncTwitterQuery(hashtag,timeLastTweet));
		time = ofGetElapsedTimeMillis()-time;
		if(time<checkEvery){
			ofSleepMillis(checkEvery-time);
		}
	}
}
