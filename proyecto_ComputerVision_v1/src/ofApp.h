#pragma once

#include "ofMain.h"
#include "../particle.h"
#include "ofxGui.h"
#include "ofxOpenCV.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		vector<particle> p;
		
//#ifdef song
		ofSoundPlayer song;
//#endif //song

		ofxPanel gui;
		ofxFloatSlider overallSpeed;
		ofxFloatSlider noiseAmount;
		ofxFloatSlider trail;
		//ofxFloatSlider repelDistance;
		// Object to extract the blubs from the Grayscale Images
		//ofxCvContourFinder contourFinder;

#ifdef Video_Player
		ofVideoPlayer vidPlayer;
		ofxCvColorImage colorImg;
		ofxCvGrayscaleImage grayImg;
		ofxCvGrayscaleImage grayBg;
		ofxCvGrayscaleImage grayDiff;
		// Object to extract the blubs from the Grayscale Images
		ofxCvContourFinder contourFinder;

		ofxIntSlider threshold;
		ofxToggle bLearnBackground;
#endif //Video_Player

//#ifdef Webcam
		ofVideoGrabber webcam;
		ofxCvColorImage colorImg;
		ofxCvGrayscaleImage grayImg;
		ofxCvGrayscaleImage grayBg;
		ofxCvGrayscaleImage grayDiff;
		// Object to extract the blubs from the Graydiff Image. 
		//To find the contour of the silhouette that is being tracked
		ofxCvContourFinder contourFinder;
		//
		ofxIntSlider threshold;
		// Toggle button to capture the background
		ofxToggle bLearnBackground;
//#endif //Webcam

		int numParticles;
		
};
