#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	//this creates the backgroud audio
	m_soundPlayer.load("song.mp3");
	m_soundPlayer.setLoop(true);
	m_soundPlayer.play();

	//Shpere manipulation to sound
	circleAngle = 0.0f;
	circlePos.set(500, 400);

	//Mkaes center circle reacte to volume
	fftSmooth = new float [8192];
	for (int i = 0; i < 8192; i++) {
		fftSmooth[i] = 0;
	}
	bandsCircle = 64;

	//Wave line manipulation to sound
	fft = new float[512];
	for (int i = 0; i < 512; i++) {
		fft[i] = 0;
	}
	bands = 512;
	m_toggleScreen = false;
	m_audioAnalyser.init(&m_soundPlayer, 20);

	//Image reference
	m_ghostImaage.load("ghost.png");
	m_ghostPos.set(500, 400);
	m_targetMousePos = m_ghostPos;
	m_ghostAngle = 0.0f;

	//mouse position tracker
	m_mousePos = ofVec2f::zero();

}

//--------------------------------------------------------------
void ofApp::update() {
	//get the vector pointing to mouse from ghost
	m_mousePos = ofVec2f((float)ofGetMouseX(), (float)ofGetMouseY());
	ofVec2f vecToMouse = m_mousePos - m_ghostPos;

	m_ghostAngle = (ofRadToDeg(atan2(vecToMouse.x, vecToMouse.y)) * -1.0f) + 90.0f;

	//manual lerping
	m_ghostPos.x = lerp(m_ghostPos.x, m_targetMousePos.x, 0.01f);
	m_ghostPos.y = lerp(m_ghostPos.y, m_targetMousePos.y, 0.01f);

	//speed of circle rotation
	circleAngle += 0.5;

	ofSoundUpdate();
	//wave
	soundSpectrum = ofSoundGetSpectrum(bands);
	for (int i = 0; i < bands; i++) {
		fft[i] *= 0.9;
		if(fft[i] < soundSpectrum[i]){
			fft[i] = soundSpectrum[i];
		}
	}

	//sphere
	float* value = ofSoundGetSpectrum(bandsCircle);
	for (int i = 0; i < bands; i++) {
		fftSmooth[i] *= 0.9f;
		if (fftSmooth[i] < value[i]) {
			fftSmooth[i] = value[i];
		}
	}

	//analyser
	m_audioAnalyser.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofPushMatrix();
	if (m_toggleScreen == true) {
		
		//draws fFT graph
		ofSetRectMode(OF_RECTMODE_CORNER);
		ofScale(0.5f, 0.5f);
		m_audioAnalyser.drawWaveform(40, 300, 1200, 90);
		m_audioAnalyser.drawSpectrum(40, 460, 1200, 128);
		m_audioAnalyser.drawLinearAverages(40, 650, 1200, 128);	
		// Frequency / FFT information
		ofDrawBitmapString("Peak", 40, 220);
		ofTranslate(40, 200);
		ofScale(2.0f, 2.0f);
		for (int i = 0; i < 1; i++) {
			ofPolyline polyline;
			for (int j = 0; j < bands; j++) {
				polyline.addVertex(j, i - fft[j] * 100.0);
			}
			polyline.draw();
		}
	}
	ofPopMatrix();
	
	ofPushMatrix();
	//pick out instruments
	float tone1 = m_audioAnalyser.getLinearAverage(0); //base
	float tone2 = m_audioAnalyser.getLinearAverage(15);//Hi-Hat
	float tone3 = m_audioAnalyser.getLinearAverage(3); //acoustic sounds
	float tone4 = m_audioAnalyser.getLinearAverage(7); // classic guiter
	
	 // Draw circles to indicate activity in the frequency ranges we are interested in
	// Must be remapped using the ranges of activity that we want
	m_circle1Brightness = ofMap(tone1, 143.0f, 200.0f, 0.0f, 1.0f, true);
	m_circle2Brightness = ofMap(tone2, 30.0f, 50.0f, 0.0f, 1.0f, true);
	m_circle3Brightness = ofMap(tone3, 20.0f, 30.0f, 0.0f, 1.0f, true);
	m_circle4Brightness = ofMap(tone4, 16.0f, 23.0f, 0.0f, 1.0f, true);
	ofPopMatrix();

	ofPushMatrix();
	if (m_toggleScreen == false ) {

		//draw sphere
		ofNoFill(); // remove color from shepere
		ofTranslate(circlePos);
		for (int i = 0; i < bandsCircle; i++) {
			ofDrawIcoSphere(0, 0, 0, -(fftSmooth[i] * 50));
		}

		ofRotateDeg(circleAngle); //rotate circles

		ofFill(); //fills color of cirlces
		ofSetColor(255);
		ofSetColor(ofFloatColor(m_circle1Brightness, 0.0f, 0.0f));
		ofDrawCircle(100, 0, 50);

		ofSetColor(ofFloatColor(0.0f, m_circle2Brightness, 0.0f));
		ofDrawCircle(0, 100, 50);

		ofSetColor(ofFloatColor(0.0f, 0.0f, m_circle3Brightness));
		ofDrawCircle(-100, 0, 50);

		ofSetColor(ofFloatColor(m_circle4Brightness, m_circle4Brightness, m_circle4Brightness));
		ofDrawCircle(0, -100, 50);
		ofPopMatrix();
	}ofPopMatrix();
	
		//Draw pillar rectangles loop
	ofPushMatrix();
		if (m_toggleScreen == false) {
			for (int i = 0; i < 200; i++) {
				ofSetRectMode(OF_RECTMODE_CENTER);
				ofDrawRectangle(100, i * 4, sin(i + ofGetElapsedTimef()) * 128, 2);

			}
			for (int i = 0; i < 200; i++) {
				ofSetRectMode(OF_RECTMODE_CENTER);
				ofDrawRectangle(900, i * 4, sin(i + ofGetElapsedTimef()) * 128, 2);

			}
		}
	ofPopMatrix();
	
	
	if (m_toggleScreen == false) {
		if (tone1 > tone3) {
			ofNoFill();
			ofTranslate(circlePos);
			ofRotateDeg(circleAngle); //rotate circles
			ofScale(10.0f, 10.0f);
			ofDrawBox(0, 0, 2, 64, 64, 64);

			
		}
	}
	ofPopMatrix();

	//Recommended task: Ghost moving around screen
	ofPushMatrix();
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofTranslate(m_ghostPos);
	ofRotate(m_ghostAngle);
	ofScale(0.2f, 0.2f);
	m_ghostImaage.draw(0, 0);
	ofPopMatrix();
}

void ofApp::keyPressed(int key) {
	switch (key) {
	case '1':
		m_soundPlayer.play();
		break;
	case '2':
		m_soundPlayer.stop();
		break;
	}

	if (key == 32) {
		m_toggleScreen = !m_toggleScreen;
	}

}

void ofApp::mousePressed(int x, int y, int button) {

	m_targetMousePos = ofVec2f((float)x, (float)y);


}

float ofApp::lerp(float start, float end, float percent) {
	return(start + percent * (end - start));
}
