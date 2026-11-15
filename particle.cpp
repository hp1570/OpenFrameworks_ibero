#include "particle.h"

particle::particle()
{
}

void particle::setup()
{
	pos.x = ofRandomWidth();
	pos.y = ofRandomHeight();

	vel.x = ofRandom(-4, 4);
	vel.y = ofRandom(-4, 4);

	frc = ofVec2f(0, 0);

	uniqueVal.x = ofRandom(-1000, 1000);
	uniqueVal.y = ofRandom(-1000, 1000);
	
	size = ofRandom(3,5);

	drag = ofRandom(0.97, 0.99);

	color.r = ofRandom(255);
	color.g = ofRandom(255);
	color.b = ofRandom(255);
}

void particle::update(float speed, float noise)
{
	frc.x = ofSignedNoise(uniqueVal.x, ofGetElapsedTimeMillis()); // Gives values of -1 and 1 and
																  // ofGetElapsedTimeMillis() is for changing in every frame
	frc.y = ofSignedNoise(uniqueVal.y, ofGetElapsedTimeMillis());

	frc *= noise;

	vel *= drag;

	// If it's minus, we are repelling
	// If it's adding, we are attracting
	vel -= repelFrc;

	vel += frc;

	// RESET AT BORDER

	if (pos.x + vel.x > ofGetWidth()) {
		pos.x -= ofGetWidth();
	}
	else if (pos.x + vel.x < 0) {
		pos.x += ofGetWidth();
	}

	if (pos.y + vel.y > ofGetHeight()) {
		pos.y -= ofGetHeight();
	}
	else if (pos.y + vel.y < 0) {
		pos.y += ofGetHeight();
	}

	pos += vel*speed;
}

void particle::draw(float sizeDot)
{
	ofSetColor(color);
	ofDrawCircle(pos.x, pos.y, size*sizeDot);
}

 void particle::repel(vector<ofVec2f> repelPt)
{
	repelFrc.set(0, 0);

	// Access all the repel points
	for (int i = 0; i < repelPt.size(); i++) {
		// We need to know the distance from the repel point to the particle itself
		float dist = pos.distance(repelPt[i]);

		// If the distance is smaller to 150, this means:
		// the distance of our repel point to the position of the 
		// particle is quite small
		if (dist < 150) {
			ofVec2f newRepelPt;
			// We are repeling with the newRepelPt
			// With this technique, we are getting the right vector direction
			// to repel in the right direction
			newRepelPt = repelPt[i] - pos;
			// This will add a very strong force in the new direction and multiplying by
			// a small value, we get something not as strong
			repelFrc += newRepelPt * 0.0001;
		}
	}
}