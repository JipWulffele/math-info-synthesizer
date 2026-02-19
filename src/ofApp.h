#pragma once

#include "ofMain.h"
#include "oscilator.h"
#include "ofxGui.h"
#include "BourdonMelodies.h"

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
		
		// Bourdons
		oscilator bourdon;
		oscilator bourdonMelody;
		const std::vector<MelodyStep> melody = BourdonMelodies::song3;

		// Melody management
		bool bourdonMelodyPlaying = false;
		int bourdonStep = 0;
		float bourdonStepStartTime = 0.0f;
		void startBourdonMelody();
		void stopBourdonMelody();

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
		ofxToggle bourdonToggleGui;
		ofxFloatSlider bourdonFrequencesGui;

        // Melody
		ofxToggle playBourdonMelodyButton;

		ofxFloatSlider brillanceSliderGui;
		ofxFloatSlider amplitudeSliderGui;
		ofxToggle mouseToggleGui;
		
		// Waveform amplitude sliders
		ofxFloatSlider ampSineGui;
		ofxFloatSlider ampSquareGui;
		ofxFloatSlider ampSawtoothGui;
		ofxFloatSlider ampTriangleGui;
	

};
