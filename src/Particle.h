#pragma once

#include "ofMain.h"

inline void randomize(ofVec2f& v) {
	v.x = ofRandomf();
	v.y = ofRandomf();
	v.normalize();
}

class Particle {
public:
	static ofImage* friction;
	static ofPixels frictionMarker;
	static bool enableFriction;
	static unsigned int frictionTotal, frictionCount;
	
	static ofVec2f centeringForce, globalOffset, avg;
	static float speed, spread, viscosity, independence, rebirthRadius, neighborhood, confusion, indecision, age, globalDirection;
	static vector<Particle> particles;
	static void setup();
  static void drawAll();
  static void updateAll(float dt);
	static void addFriction(ofImage& img);

  ofVec2f position, velocity, force, localOffset;
  Particle(float radius) {
    randomize(localOffset);
  	randomize(position);
  	position *= radius;
  }
  inline void applyFlockingForce() {
		float basex = position.x / neighborhood;
		float basey = position.y / neighborhood;
    force.x +=
      ofSignedNoise(
				basex + globalOffset.x + localOffset.x * independence,
				basey);
		force.y +=
      ofSignedNoise(
				basex,
				basey + globalOffset.y  + localOffset.y * independence);
  }
  inline void applyViscosityForce() {
    force += velocity * -viscosity;
  }
  inline void applyCenteringForce() {
    centeringForce.set(position);
    //centeringForce -= avg;
    float distanceToCenter = centeringForce.length();
    centeringForce.normalize();
    centeringForce *= -distanceToCenter / (spread * spread);
    force += centeringForce;
  }
	inline void applyFrictionForce() {
		ofVec2f imgPosition = position;
		imgPosition *= .2;
		imgPosition += ofVec2f(friction->getWidth() / 2, friction->getHeight() / 2);
		imgPosition.x = ofClamp(imgPosition.x, 0, friction->getWidth() - 1);
		imgPosition.y = ofClamp(imgPosition.y, 0, friction->getHeight() - 1);
		int i = (int) imgPosition.y * friction->getWidth() + imgPosition.x;
		unsigned char cur = friction->getPixels()[i];
		if(cur > 128) { // on friction land
			unsigned char curMarker = frictionMarker.getPixels()[i];
			if(curMarker) { // inhabited, refract randomly
				force.rotateRad(ofRandomf() * HALF_PI);
			} else { // uninhabited, slow down, mark inhabited
				force *= .2;
				velocity *= .2;
				frictionMarker.getPixels()[i] = true;
				frictionCount++;
			}
		}
	}
  inline void update(float dt) {			
    force.set(0, 0);
    applyFlockingForce();
    applyViscosityForce();
		applyCenteringForce();
		if(enableFriction) {
			applyFrictionForce();
		}
    velocity += force * dt; // mass = 1, acceleration = force
    position += velocity * speed * dt;
  }
};
