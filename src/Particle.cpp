#include "Particle.h"

ofImage*
	Particle::friction;
ofPixels
	Particle::frictionMarker;
	
unsigned int
	Particle::frictionTotal,
	Particle::frictionCount;

vector<Particle>
	Particle::particles;

ofVec2f
	Particle::centeringForce,
	Particle::globalOffset,
	Particle::avg;

float
	Particle::speed,
	Particle::spread,
	Particle::viscosity,
	Particle::independence,
	Particle::neighborhood,
	Particle::confusion,
	Particle::indecision,
	Particle::age,
	Particle::globalDirection;
	
bool
	Particle::enableFriction;

void Particle::setup() {
	globalOffset.set(1. / 3, 2. / 3);
	speed = 5;
  spread = 60;
  viscosity = .01;
  independence = .1;
  neighborhood = 1200;
	confusion = .01;
	indecision = .01;
	age = 0;
	globalDirection = 0;
	enableFriction = false;
}

void Particle::drawAll() {
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_POINTS);
	for(int i = 0; i < particles.size(); i++) {
        mesh.addVertex(ofVec3f(particles[i].position));
	}
	mesh.drawVertices();
}

void Particle::updateAll(float dt = 1. / 60.) {
	if(enableFriction) {
		// clear friction markers
		int n = frictionMarker.getWidth() * frictionMarker.getHeight();
		unsigned char* frictionMarkerPixels = frictionMarker.getData();
		for(int i = 0; i < n; i++) {
			frictionMarkerPixels[i] = false;
		}
	}
	frictionCount = 0;
	
	avg.set(0, 0);
	ofVec2f sum;
	for(int i = 0; i < particles.size(); i++) {
		particles[i].update(dt);
		sum += particles[i].position;
	}
	avg = sum / particles.size();
	
	globalDirection += ofSignedNoise(indecision * age);
	ofVec2f direction(0, 1);
	direction.rotate(globalDirection);
	globalOffset += confusion * direction * dt;
	age += dt;
}

void Particle::addFriction(ofImage& img) {
	friction = &img;
	frictionMarker.allocate(img.getWidth(), img.getHeight(), OF_IMAGE_GRAYSCALE);
	int n = img.getWidth() * img.getHeight();
	unsigned char* pixels = img.getPixels().getData();
	frictionTotal = 0;
	for(int i = 0; i < n; i++) {
		if(pixels[i] > 128) {
			frictionTotal++;
		}
	}
}
