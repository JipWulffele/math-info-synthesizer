#pragma once

#include "ofMain.h"
#include "oscilator.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:

	    // Setup, update, draw, and exit functions
		void setup() override;
		void update() override;
		void draw() override;
		void exit() override;

		// Event handling functions
		void keyPressed(int key) override;
		void keyReleased(int key) override;
		void mouseMoved(int x, int y ) override;

        // Main audio functions
		void audioOut(ofSoundBuffer & buffer) override;
		void cbAudioProcess(ofSoundBuffer & buffer);
		
		void computeFT(vector <float> & audio); // Function to compute Fourier transform 

		// Variables for sound processing
		vector <float> audioBuffer; // Buffer to hold audio samples
        int sampleRate; // Sample rate for audio processing
		int bufferSize; // Size of the audio buffer

		vector <float> audioFT; // Buffer to hold Fourier transform data for visualization

		// Sound stream and buffer
		ofSoundStream soundStream;
		ofSoundBuffer soundBuffer;

		// Gestion clavier
		bool noteOn = false ;
		std::string currentNote; // nom de la note actuellement pressée

		// setup keyboard -> 1 oscilator by note stored in oscillators
		std::array<oscilator, 12> oscillators;

		// GUI
    	ofxPanel gui;
    	// ofParameter<float> brillanceGui;
		ofxFloatSlider brillanceSliderGui;
		ofxFloatSlider amplitudeSliderGui;
		ofxFloatSlider frequencesGui;
		ofxToggle mouseToggleGui;
		
		// Waveform amplitude sliders
		ofxFloatSlider ampSineGui;
		ofxFloatSlider ampSquareGui;
		ofxFloatSlider ampSawtoothGui;
		ofxFloatSlider ampTriangleGui;
	

};
