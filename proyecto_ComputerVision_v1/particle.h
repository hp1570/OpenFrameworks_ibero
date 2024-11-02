#pragma once

#include "ofMain.h"

class particle
{
public:
	particle(); // Constructor

	/* Métodos de la clase */
	void setup();
	void update(float speed, float noise);
	void draw(float sizeDot);
	void repel(vector<ofVec2f> repelPt);

	/* Atributos */
	ofVec2f pos, vel, frc, repelFrc; // Position, velocity, forces and repelForces
	float drag; // Creates nice balance of speed

	ofVec2f uniqueVal;
	float size;
	ofColor color;
};

