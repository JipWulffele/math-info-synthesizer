#pragma once

#include "ofMain.h"
#include "oscilator.h"

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
		//void mouseDragged(int x, int y, int button) override;
		//void mousePressed(int x, int y, int button) override;
		//void mouseReleased(int x, int y, int button) override;
		//void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
		//void mouseEntered(int x, int y) override;
		//void mouseExited(int x, int y) override;
		//void windowResized(int w, int h) override;
		//void dragEvent(ofDragInfo dragInfo) override;
		//void gotMessage(ofMessage msg) override;
        

        // Main audio functions
		void audioOut(ofSoundBuffer & buffer) override;
		void cbAudioProcess(ofSoundBuffer & buffer);
		
		void computeFT(vector <float> & audio); // Function to compute Fourier transform 

		// Functions to create and modify sound
		
		// Functions create and modify sound
		float calc_sin(float A, float f, float t);
		float calcul_carre(float A, float f, float t, float brillance);
		float calcul_scie(float A, float f, float t, float brillance);
        

		// Variables for sound processing
		vector <float> audioBuffer; // Buffer to hold audio samples
        int sampleRate; // Sample rate for audio processing
		int bufferSize; // Size of the audio buffer

		vector <float> audioFT; // Buffer to hold Fourier transform data for visualization

		float A; // Amplitude
		float f; // Frequency
		float t; // Time
		// float volume; // Just use amplitude ?

		int formeOnde; // Waveform type (0, 1, 2)
		float brillance; // Brightness of the sound

		// Sound stream and buffer
		ofSoundStream soundStream;
		ofSoundBuffer soundBuffer;
		
};
