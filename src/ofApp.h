#pragma once

#include "ofMain.h"
#include "AudioAnalyser.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void mousePressed(int x, int y, int button);

		float circleAngle; //this would store sphere angle of rotation
		ofVec2f circlePos; //this would store position of circle

		//audio analyser
		ofFmodSoundPlayer	m_soundPlayer;
		ofSoundPlayer       m_soundPlayerOF;
		AudioAnalyser		m_audioAnalyser;
	
		//make audio reactive polyline 
		ofSoundPlayer sound;
		float * fftSmooth;
		float* fft;
		float* soundSpectrum;
		int bands;
		int bandsCircle;
		
		//
		float  m_circle1Brightness;
		float  m_circle2Brightness;
		float  m_circle3Brightness;
		float  m_circle4Brightness;

		bool m_toggleScreen;

		ofImage m_ghostImaage;
		ofVec2f m_ghostPos;
		ofVec2f m_mousePos;
		float   m_ghostAngle;
		ofVec2f m_targetMousePos;

		float lerp(float start, float end, float percent);
};
