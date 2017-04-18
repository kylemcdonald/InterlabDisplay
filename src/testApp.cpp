#include "testApp.h"

void setupBlending() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
}

void testApp::startFriction(ofImage& img) {
	Particle::addFriction(img);
	Particle::enableFriction = true;
}

void testApp::stopFriction() {
	Particle::enableFriction = false;
}

void testApp::setup() {
	ofHideCursor();
	
	ofSetVerticalSync(true);
	
	Particle::setup();
	debug = false;
	int n = 50000;
	float radius = ofGetWidth();
	for(int i = 0; i < n; i++)
		Particle::particles.push_back(Particle(radius));
	
	glDisable(GL_DEPTH_TEST);
	setupBlending();
	
	ycam.loadImage("YCAM.png");
	ycam.setImageType(OF_IMAGE_GRAYSCALE);
	interlab.loadImage("interlab.png");
	interlab.setImageType(OF_IMAGE_GRAYSCALE);
	lastState = -1;
}

void testApp::update() {
	Particle::updateAll(60 * ofGetLastFrameTime());
	
	int stateCount = 6; // spacing quantization
	float cycleLength = 40; // seconds
	int curState = stateCount * (fmodf(ofGetElapsedTimef(), cycleLength) / cycleLength);
	if(curState != lastState) {
		cout << curState << endl;
		switch(curState) {
			case 0:
				startFriction(ycam);
				Particle::viscosity = .01;
			break;
			case 1:
				stopFriction();
				Particle::viscosity = .1;
			break;
			case 3:
				startFriction(interlab);
				Particle::viscosity = .01;
			break;
			case 4:
				stopFriction();
				Particle::viscosity = .1;
			break;
		}
	}
	lastState = curState;
}

void testApp::draw() {
	ofClear(0, 0, 0, 1);
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);
//	ofScale(.2, .2);
    ofScale(.3, .3);
	ofSetColor(255);
	Particle::drawAll();
	ofPopMatrix();
	
	if(debug) {
		ofSetColor(255, 255, 255, 255);
		ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
		if(Particle::frictionTotal != 0) {
			ofDrawBitmapString(ofToString((int) Particle::frictionCount) + "/" + ofToString((int) Particle::frictionTotal), 10, 40);
			ofDrawBitmapString(ofToString((int) 100 * Particle::frictionCount / Particle::frictionTotal) + "%", 10, 60);
		}
	}
}

void testApp::keyPressed(int key) {
	if(key == 'f')
		ofToggleFullscreen();
	if(key == 'e') {
		startFriction(ycam);
	}
	if(key == 'r') {
		startFriction(interlab);
	}
	if(key == 's') {
		stopFriction();
	}
	if(key == 'd') {
		debug = !debug;
	}
}
