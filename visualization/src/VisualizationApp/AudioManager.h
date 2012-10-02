/*
 * AudioManager.h
 *
 *  Created on: Sep 20, 2012
 *      Author: arturo
 */

#ifndef AUDIOMANAGER_H_
#define AUDIOMANAGER_H_

#include "ofSoundStream.h"
#include "ofSoundBuffer.h"
#include "ofParameter.h"
#include "ofParameterGroup.h"
#include "LEDStrip.h"
#include "TweetText.h"


class AudioManager: public ofBaseSoundInput {
public:
	AudioManager();
	virtual ~AudioManager();

	void setup();
	void update();
	void audioIn(float * in,int bufferSize,int nChannels,int deviceID,unsigned long long tickCount);
	void audioTestChanged(bool & enabled);
	void setLeds(vector<LEDStrip> & leds);
	void setCurrentTweet(TweetText * tweet);

	ofParameter<float> vumeter;
	ofParameter<float> cutDown,cutUp;
	ofParameter<float> mappedVumeter;
	ofParameter<float> smoothedVumeter;
	ofParameter<float> smoothFactor;
	ofParameter<float> fineSmoothFactor;
	ofParameter<bool> audioEnabledForBursts;
	ofParameter<bool> audioEnabledForText;
	ofParameter<bool> audioTest;

	ofParameterGroup parameters;

private:
	ofSoundStream sound;
	ofSoundBuffer soundBuffer;
	vector<LEDStrip> * leds;
	TweetText * tweet;
};

#endif /* AUDIOMANAGER_H_ */
