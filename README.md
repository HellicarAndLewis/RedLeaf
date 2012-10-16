RedLeaf
=======

An interactive installation for the Nissan Leaf Electric vehicle, taking input from Twitter #hashtags and a network of security cameras.


Compiling
=========

clone a copy of OF from github the branch being used is in arturoc: arturodevelop:

git clone https://github.com/arturoc/openFrameworks
git checkout arturodevelop

or if you already have an OF repo cloned:

git checkout -b arturodevelop
git pull https://github.com/arturoc/openFrameworks arturodevelop

to compile the application clone the repository with submodules inside OF_ROOT/apps:

git clone --recursive git@github.com:HellicarAndLewis/RedLeaf

the ofxHttpUtils addon needs the experimental branch to compile, once cloned the whole repository:

cd addons/ofxHttpUtils
git checkout experimental

the computer vision part is using ofxCv, the ofxCv addon should be cloned in OF_ROOT/addons:

cd OF_ROOT/addons
git clone https://github.com/arturoc/ofxCv


download gstreamer SDK from:

http://docs.gstreamer.com/display/GstSDK/Installing+on+Mac+OS+X

the development package is needed to compile the application. then go into:

/Library/Frameworks/GStreamer.framework/Headers

and delete assert.h which is clashing with the standard header from gcc



User manual
===========


General Parameters
==================

- *video/visualization:* switches between the visualization and video views of the app, can also be changed pressing 'v'

- *vsync:* (de)activates the vertical sinchronization with the monitor refresh


LEDStrip
========

- *fadeTime:* Time it takes for a led strip to fade since it was triggered by an energy burst


EnergyBurst
===========

- *decayPct:* 0..1 percentage at the end of the burst where the intensity of the light begins to decay, this avoids a sudden end of the burst. Apart from this decay each LED strip decays using the fadeTime parameter

- *minLifeTime / maxLifeTime:* minimum/maximum life of an energy burst in milliseconds, the life of each burst will be a random time between this two values

- *minSpeed / maxSpeed:* minimum/maximum speed of an energy burst in 0..1 percentage of the whole wall, where the whole wall is 1. the speed of each burst will be a random number between this two values


Twitter
=======

- *checkEvery:* the application checks for new hashtags every x milliseconds 

- *changeHashTag:* when pressing it, it shows a text box that allows the current twitter hashtag that the application listens for. Whenever there's a new hashtag the application creates an energy burst in a random position in the wall that travels across it with a random life and speed

- *changeFont:* shows a load file dialog to choose a new truetype font to show tweets text on the wall

- *speedPixelsPerSec:* speed of the tweets texts on the wall expressed in pixels(strips) per second

- *y:* height at which the tweet texts are shown on the wall

- *fontSize:* size of the font for the tweet texts


Wall
====

- *w:* number of strips in the whole wall

- *h:* visualization only value. Number of leds per strip

- *radiusScale:* visualization only value. The radius of each LED is calculated automatically, this value allows to scale them so they look better in some visualization modes

- *z:* visualization only value. z position of the 3d mode visualization

- *secondScreenPosition:* position of the output screen, allows to offset the screen that drives the LED strips, can be fine adjusted with the cursor keys

- *renderMode:* visualization only value. Changes the mode of the visualization, the available modes are:

		** Continuous:  one rectangle with all the strips, good to check the effect of the bursts mixing. Press the mouse in the rectangle to create a burst in that position

		** Separate: four rectangles, one per wall, good to check the continuity from wall to wall. Press in the line in the middle to create a burst in that position

		** 3D!: yay, drag the mouse over the model to rotate it in 3D. Press on the line at the bottom to create a burst in that position, the walls in the 3d model are numbered
		
- *rotation3D:* shows the current rotation in the model, not really needed cause the model can be rotated dragging the mouse over it.

- *useColors:* if activated the energy bursts will use random hue instead of pure white

- *testStateMillis:* duration of each test state in milliseconds

- *muted:* mutes the output on the wall

- *showTweets:* shows the texts of the received tweets instead of the energy bursts
		
- *startTest:* starts the test mode showing 3 different test patterns, the whole wall with one color, strip by strip with and strip by strip without turning off the previous strips. All the test patterns are repeated 4 times one per primary color (rgb) + white
		
		
Audio
=====

- *audioInAmp:*  audio in immediate amplitude

- *cutDown/cutUp:*  litmit the range of the input, cutDown helps to reduce noise, cutUp amplifies the input

- *mappedAudioIn:* audio in amplitude mapped between cutDown and cutUp values

- *smoothedAudioIn:* mapped amplitude is smoothed according to the smooth values so the level decay is slower. This is the value used for the visualization

- *smoothFactor:* factor by which the amplitude is smoothed, goes from 0..0.9

- *fineSmoothedFactor:* fine tuning slider for the smoothing factor, adds to the smoothFactor value to calculate the smoothed amplitude, goes from 0..0.09999

- *audioEnabled:* when turned off the visualization is always at it's maximum brightness not influenced by the audio. The amplitude levels in the gui will still get updated

- *audioTest:* tests the audio levels on the wall, all the strips will be lighten up with the level of the audio
		

Player
======

Only for debugging, allows to load a video file and play it instead of camera 1, the CV of camera 1 will run on this input instead of the live feed

- *record:*  when moved from 0 to 1-4 it starts to record the selected camera to a video file, when switched back to 0 it stops recording

- *usePlayer:* starts playing a video file chosen from a file open dialog.

- *playerPaused:* (un)pauses the video file, can also be used with the space key

- *palyerPosition:* shows and allows to change the position in the video file

- *drawCamera:* 1-4 shows cameras 1 to 4 at full size, 5 shows the 4 cameras at the same time. if a videoFile is playing it'll be shown instead of camera1


Global CV
=========

The computer vision works by doing a frame differencing of each video feed. The difference between a frame and the previous is thresholded using the threshold level and in the binary image it runs a contour detection. The blobs found by the contour detection represent motion in that part of the image. The centroid of each blob detected inside the adjustable quad in each image is mapped to an imaginary line shown in the quad that goes along the wall and calibrated so it matches the real meassures of the wall. Once mapped each point along the wall is filtered so not too many bursts are generated in the same area, using the distanceSameTrigger and minTriggerTime. When motion is detected in a part of the wall the system checks if another event was triggered less than minTriggerTime in ms ago in an area less than distanceSameTrigger (in pct of the wall being the whole wall 1)

- *thresholdLevel:* the threshold applied to binarize the frame difference. less threshold makes the system more sensible generating also bigger blobs this value should be kept as high as posible while detecting enough motion

- *showGui:* shows/hides the quads in the video feeds

- *showThresholds:* when activated shows the thresholded image instead of the live feed

- *showContours:* shows the detected contours as bounding boxes and the corresponding bursts being triggered in the imaginary line in the center of the quad after filtering and calibrating

- *minTriggerTime:* minimum time it has to pass since a movement was detected near the current one for it to trigger a burst. If less time has passed it won't trigger a new burst. the lower the value the more bursts per second the system will generate in the same area

- *distanceSameTrigger:* size of the area to check for other bursts just been triggered

CVCameraX
=========

Allows to calibrate each feed

- *calibrationMode:* starts/finishes the calibration mode, when pressed a line will be shown in the wall, someone has to go in front of the ligthen strip and in the video feed view someone else has to press with the mouse wherever they see this person. a blue rectangle will appear in the center line

- *calibrationPoint:* changes the current calibration point from 0-3. The process described above has to be done for 4 points in each wall, for that change this slider to the next point and the next line will be lighten up in the wall

- *resetCalibration:* deletes the current calibration. defaults to no calibration

CameraX
=======

Allows to change parameters of each camera:

- *changeIP:* shows a text box to enter a new ip for the camera

- *reset:* stops and restarts the camera

- *resolution:* changes the resolution of the video feed from the camera, should be used at 640x480, the other modes are:
	0 - 320x240
	1 - 640x480
	2 - 800x600
	3 - 1280x720
	
- *showFocusWindow:* shows a rectangle in the video feed, with the mouse we can change it and the camera will focus on that area

- *autofocus:* the camera does the autofocus again on the marked focus window

- *focus:* allows to change the focus of the camera manually in case the autofocus doesn't work properly

- *manualIris:* should be left activated

- *exposure:* amount of light the camera show, the camera does this automatically changing the exposure time and sensibility so it can affect the fps

- *irCutFilter:* changes from 1 - color with ir cut filter to 2 - b&w with no ir cut. 0 is automatic and changes from one mode to another depending on the available amount of light. should be left at 2 so the cameras don't see the LED

- *compression:* compression level of the h264 stream, 20 is the default value for high quality, can be increased to get more compression in case the fps drops



Keys
====

h - shows/hides the gui, in case the fps drops it can help to hide the gui but from the tests the system is able to run everything and keep the 60fps of the vertical refresh
cursor keys - allows to change the output screen offset


