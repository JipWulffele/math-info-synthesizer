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

		// listeners
		void onBourdonAmplitudeChanged(float & value);
		void onBourdonFrequencyChanged(float & value);

		// Variables for sound processing
		vector <float> audioBuffer; // Buffer to hold audio samples
        int sampleRate; // Sample rate for audio processing
		int bufferSize; // Size of the audio buffer

		vector <float> audioFT; // Buffer to hold Fourier transform data for visualization

		
		// Gestion clavier
		bool noteOn = false ;
		std::string currentNote; // nom de la note actuellement pressée
		oscilator bourdon;

		// setup keyboard -> 1 oscilator by note stored in oscillators
		std::array<oscilator, 12> oscillators;
		std::array<float, 12> baseFrequencies;  // Store base frequencies for octave shifting
		int octaveShift = 0;  // 0 = default, 1 = +1 octave, 2 = +2 octaves, etc.
		const std::array<std::string, 12> noteNames = {
			"Do","Do#","Re","Re#","Mi","Fa",
			"Fa#","Sol","Sol#","La","La#","Si"
		};
		void updateOctaveShift();  // Recalculate oscillator frequencies based on octaveShift
		void drawKeyboard(float x, float y, float width, float height); // affichage clavier
		void updateCurrentNotes(); // pour recuperer la liste des notes actives

		// GUI
    	ofxPanel gui;
    	ofxPanel bourdonGui;
		// Bourdon
		ofxToggle bourdonToggleGui;
		ofxFloatSlider bourdonFrequencesGui;
		ofxFloatSlider bourdonAmplitudeGui;
		ofxFloatSlider bourdonBrillanceGui;
		// Waveform amplitude for Bourdon
		ofxFloatSlider bourdonAmpSineGui;
		ofxFloatSlider bourdonAmpSquareGui;
		ofxFloatSlider bourdonAmpSawtoothGui;
		ofxFloatSlider bourdonAmpTriangleGui;
		
		// all the other oscillators
		ofxToggle mouseToggleGui;
		ofxFloatSlider brillanceSliderGui;
		ofxFloatSlider amplitudeSliderGui;
		
		// Waveform amplitude sliders
		ofxFloatSlider ampSineGui;
		ofxFloatSlider ampSquareGui;
		ofxFloatSlider ampSawtoothGui;
		ofxFloatSlider ampTriangleGui;
		
		// Sound stream and buffer
		ofSoundStream soundStream;
		ofSoundBuffer soundBuffer;

};
