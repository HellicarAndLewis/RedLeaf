/*
 * AudioManager.cpp
 *
 *  Created on: Sep 20, 2012
 *      Author: arturo
 */

#include "AudioManager.h"

AudioManager::AudioManager() {
	tweet = NULL;
	leds = NULL;

}

AudioManager::~AudioManager() {
	// TODO Auto-generated destructor stub
}

void AudioManager::setup(){
	audioTest.addListener(this,&AudioManager::audioTestChanged);

	parameters.setName("Audio");
	parameters.add(vumeter.set("audioInAmp",0,0,1));
	parameters.add(cutDown.set("cutDown",0,0,1));
	parameters.add(cutUp.set("cutUp",1,0,1));
	parameters.add(mappedVumeter.set("mappedAudioIn",0,0,1));
	parameters.add(smoothedVumeter.set("smoothedAudioIn",0,0,1));
	parameters.add(smoothFactor.set("smoothFactor",0.9,0,0.9));
	parameters.add(fineSmoothFactor.set("fineSmoothFactor",0,0,0.09999));
	parameters.add(audioEnabledForBursts.set("audioEnabledForBursts",false));
	parameters.add(audioEnabledForText.set("audioEnabledForText",false));
	parameters.add(audioTest.set("audioTest",false));

	soundBuffer.resize(256,0);
	soundBuffer.setSampleRate(44100);
	sound.setup(0,1,44100,256,1);
	sound.setInput(this);

}

void AudioManager::audioTestChanged(bool & enabled){
	for(u_int i=0;i<leds->size();i++){
		leds->at(i).setTestMode(enabled);
	}
}

void AudioManager::setLeds(vector<LEDStrip> & _leds){
	leds = &_leds;
}

void AudioManager::setCurrentTweet(TweetText * _tweet){
	tweet = _tweet;
}

void AudioManager::update(){
	u_long now = ofGetElapsedTimeMillis();
	if(audioTest){
		for(u_int i=0;i<leds->size();i++){
			leds->at(i).trigger(ofColor::white*smoothedVumeter,now);
		}
	}else{
		if(audioEnabledForBursts){
			for(u_int i=0;i<leds->size();i++){
				leds->at(i).setAmplitude(smoothedVumeter);
			}
		}

		if(tweet && audioEnabledForText){
			tweet->setAmplitude(smoothedVumeter);
		}
	}
}

void AudioManager::audioIn(float * in,int bufferSize,int nChannels,int deviceID,unsigned long long tickCount){
	soundBuffer.set(in,bufferSize,nChannels);
	vumeter = soundBuffer.getRMSAmplitude();
	mappedVumeter = ofMap(vumeter,cutDown,cutUp,0,1,true);
	smoothedVumeter = max((float)mappedVumeter,smoothedVumeter*(smoothFactor+fineSmoothFactor) + mappedVumeter*(1-(smoothFactor+fineSmoothFactor)));

}
