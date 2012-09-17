RedLeaf
=======

An interactive installation for the Nissan Leaf Electric vehicle, taking input from Twitter #hashtags and a network of Laser scanners.

Visualization App
=================

The visualization app shows a simulation of the real wall were the parameters for the lights can be adjusted.

In the parameters gui there's controls to change:

General Parameters
==================

- *changeHashTag:* when pressing it, it shows a text box that allows the current twitter hashtag that the application listens for. Whenever there's a new hashtag the application creates an energy burst in a random position in the wall that travels across it with a random life and speed

- *useColors:* if activated the energy bursts will use random hue instead of pure white

Twitter
=======

- *checkEvery:* the application checks for new hashtags every x milliseconds 


LEDStrip
========

- *fadeTime:* Time it takes for a led strip to fade since it was triggered by an energy burst


EnergyBurst
===========

- *decayPct:* 0..1 percentage at the end of the burst where the intensity of the light begins to decay, this avoids a sudden end of the burst. Apart from this decay each LED strip decays using the previous parameter

- *minLifeTime / maxLifeTime:* minimum/maximum life of an energy burst in milliseconds, the life of each burst will be a random time between this two values

- *minSpeed / maxSpeed:* minimum/maximum speed of an energy burst in 0..1 percentage of the whole wall, where the whole wall is 1. the speed of each burst will be a random number between this two values

Wall
====

- *w:* number of strips in the whole wall

- *h:* visualization only value. Number of leds per strip

- *radiusScale:* visualization only value. The radius of each LED is calculated automatically, this value allows to scale them so they look better in some visualization modes

- *z:* visualization only value. z position of the 3d mode visualization

- *renderMode:* visualization only value. Changes the mode of the visualization, the available modes are:

		** Continuous:  one rectangle with all the strips, good to check the effect of the bursts mixing. Press the mouse in the rectangle to create a burst in that position

		** Separate: four rectangles, one per wall, good to check the continuity from wall to wall. Press in the line in the middle to create a burst in that position

		** 3D!: yay, drag the mouse over the model to rotate it in 3D. Press on the line at the bottom to create a burst in that position, the walls in the 3d model are numbered
		
- *rotation3D:* shows the current rotation in the model, not really needed cause the model can be rotated dragging the mouse over it.
		

		



