#pragma once

#include "ofMain.h"
#include "Particle.h"

class testApp : public ofBaseApp {
public:
	void setup();	
	void update();
	void draw();
	
	void startFriction(ofImage& img);
	void stopFriction();
	
	void keyPressed(int key);
	
	ofImage ycam, interlab;
	int lastState;
	bool debug;
};
