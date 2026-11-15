/*
 * @file ofApp.cpp
 * @brief Este programa utiliza Computer Vision (Procesamiento de imágenes)
 *		  Se utilizará una técnica estándar de Computer Vision denominada Background Substraction
 *		  Este programa sigue estos 4 pasos fundamentales:
 *		  1) Capturar la imagen del escenario vacío (IMPORTANTE)
 *		  2) Tomar video desde una cámara
 *		  3) Comparar las diferencias entre la imagen capturada y la imagen de la cámara en vivo.
 *		  4) De la silueta sustraida anteriormente por las diferencias, definir los blobs (puntos resultantes de la silueta),
 *        para los contours
 * @author Hernán Nuño
 * @date 10/oct/2024
 */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	numParticles = 1000;

#ifdef Video_Player
	vidPlayer.load("finger.mov");
	vidPlayer.play();
	vidPlayer.setLoopState(OF_LOOP_NORMAL);
#endif // Video_Player
	
//#ifdef Webcam
	// Inicializar la cámara
	webcam.setVerbose(true);
	webcam.setup(320, 240);
//#endif // Webcam
	/*
#ifdef Kinect
	kinect.open();
#endif // Kinect
	*/

	// Asignación de pixeles para cada imagen a probar
	colorImg.allocate(320, 240);
	grayImg.allocate(320, 240);
	grayBg.allocate(320, 240);
	grayDiff.allocate(320, 240);

	// Setup del GUI
	gui.setup();
	gui.add(overallSpeed.setup("Speed", 1, 0, 25));
	gui.add(noiseAmount.setup("Noise", 1, 0, 10));
	gui.add(trail.setup("Trail", 10.0, 0, 200));

//#ifdef Webcam
	/*
	In digital image processing, thresholding is the simplest method of segmenting images. 
	From a grayscale image, thresholding can be used to create binary images.
	*/
	gui.add(threshold.setup("Threshold", 80, 0, 255));
	gui.add(bLearnBackground.setup("Capture bg", false));
//#endif // Webcam

//#ifdef song
	song.load("Viva La Vida.mp3");
	song.play();
	song.setLoop(true);
	song.setVolume(10);
//#endif // song

	// Creación de partículas
	p.assign(numParticles, particle());

	// Recurrimos al método setup() de cada part�cula para proporcionarle un comportamiento
	for (int i = 0; i < p.size(); i++) {
		p[i].setup();
	}

	// No refrescará la pantalla mientras est� en ejecucci�n el programa
	ofSetBackgroundAuto(false);

}

//--------------------------------------------------------------
void ofApp::update(){
	//Se puede modificar el FrameRate() para las part�culas porque eso cambiar� el control del sistema a trabajar
	ofSetFrameRate(60);

#ifdef Video_Player
	bool bNewFrame = false;
	vidPlayer.update();
	bNewFrame = vidPlayer.isFrameNew();

	if (bNewFrame) {
		colorImg.setFromPixels(vidPlayer.getPixels());
		grayImg = colorImg;

		if (bLearnBackground == true) {
			grayBg = grayImg;
			bLearnBackground = false;
		}

		grayDiff.absDiff(grayBg, grayImg);
		grayDiff.threshold(threshold);

		contourFinder.findContours(grayDiff, 20, (340 * 240) / 3, 10, true);
	}
#endif // Video_Player

//#ifdef Webcam
	bool bNewFrame = false;
	webcam.update();
	bNewFrame = webcam.isFrameNew();

	if (bNewFrame) {
		// Step 1: Set the color image equals to the webcam image
		colorImg.setFromPixels(webcam.getPixels());
		// Step 2: Equals gray image to color image
		grayImg = colorImg;

		// Step 3: Enable toggle button, so that we can copy to the background image the current image
		if (bLearnBackground == true) {
			grayBg = grayImg;
			bLearnBackground = false;
		}
		//Step 4: Do the background substraction using grayDiff
		grayDiff.absDiff(grayBg, grayImg);
		// Threshold operation is important because we would have a very noisy outcome
		// and not to good background subtraction
		grayDiff.threshold(threshold);

		// Extract the contour of the silhouette that is being tracked
		// grayDiff is the one that we use to extract the silhouette
		// minArea = 20, is the minimum size of the blob
		// maxArea = (340 * 20) / 2, is the maximum size of the blob (relative to our screen)
		// nCosidered = 10, is the number of blobs we want max
		// bFindHoles = true, if we want to find holes within the blobs
		//				false, to test 
		contourFinder.findContours(grayDiff, 20, (340 * 20) / 2, 50, true);
	}
//#endif // Webcam

	// Create a vector to get those repel points from the contour
	vector<ofVec2f> blobPts;
	for (int i = 0; i < contourFinder.nBlobs; i++) {
		for (int j = 0; j < contourFinder.blobs[i].pts.size(); j++) {
			// We need to scale the blobs to scale it to the size of our screen
			// That's why we multiply by ( ofGetWidth() / 320 )
			blobPts.push_back(contourFinder.blobs[i].pts[j] * (ofGetWidth() / 320) );
		}
	}

	for (int i = 0; i < p.size(); i++) {
		p[i].repel(blobPts);
		p[i].update(overallSpeed, noiseAmount);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	// Dependiendo del valor del trail, se actualizará el valor de la opacacidad del fondo para mostrar el trazo del objeto
	ofSetColor(0, 0, 0, trail);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	// Solución temporal a dibujar cada partícula en blanco
	//ofSetColor(255, 255, 255, 255);
	for (int i = 0; i < p.size(); i++) {
		p[i].draw(1);
	}

//#ifdef Webcam
	// Draw the incoming, the grayscale, the bg and the thresholded difference
	// Dibujar las operaciones del CV resultantes
	
	/*
	ofSetHexColor(0xffffff);
	colorImg.draw(20, 20);
	grayImg.draw(360, 20);
	grayBg.draw(20, 280);
	grayDiff.draw(20, 20);
	*/
	
	// Then draw the contours:
	ofFill();
	ofSetHexColor(0x333333);
	//ofDrawRectangle(360, 540, 320, 240);
	ofDrawRectangle(50, 20, 320, 240);
	ofSetHexColor(0xffffff);

	contourFinder.draw(20, 20);

	for (int i = 0; i < contourFinder.nBlobs; i++) {
		contourFinder.blobs[i].draw(50, 20);

		// Draw over the centroid if the blob is a hole
		ofSetColor(255);
		if (contourFinder.blobs[i].hole) {
			ofDrawBitmapString("hole",
				contourFinder.blobs[i].boundingRect.getCenter().x + 50,
				contourFinder.blobs[i].boundingRect.getCenter().y + 20);
		}
	}
//#endif // Webcam

	gui.draw();
}

/*
#ifdef Kinect
//--------------------------------------------------------------
void ofApp::exit() {
	kinect.close();
}
#endif // Kinect
*/

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
