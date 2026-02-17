#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(34, 34, 34);
	
	bufferSize	= 512;
	sampleRate 	= 44100;
	volume		= 0.1f;

    // initialize a audio buffer
	audioBuffer.assign(bufferSize, 0.0);

    // Setup device and stream
    soundStream.printDeviceList();
    ofSoundStreamSettings settings;

    auto devices = soundStream.getMatchingDevices("default");
	if(!devices.empty()){
		settings.setOutDevice(devices[0]);
	}

	settings.setOutListener(this);
	settings.sampleRate = sampleRate;
	settings.numOutputChannels = 2;
	settings.numInputChannels = 0;
	settings.bufferSize = bufferSize;
	soundStream.setup(settings);

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    // Draw current state of audiobuffer on the screen (e.g. as a waveform)

    ofSetColor(225);
	ofDrawBitmapString("AUDIO OUTPUT", 32, 32);
	ofDrawBitmapString("press 'o' to change waveform\npress 'b' to change the brillance", 31, 92);
	
	ofNoFill();
	
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, 150, 0);
			
		ofSetColor(225);
		ofDrawBitmapString("Audio buffer", 4, 18);
		
		ofSetLineWidth(1);	
		ofDrawRectangle(0, 0, 900, 200);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);
					
			ofBeginShape();
			for (unsigned int i = 0; i < audioBuffer.size(); i++){
				float x =  ofMap(i, 0, audioBuffer.size(), 0, 900, true);
				ofVertex(x, 100 -audioBuffer[i]*180.0f);
			}
			ofEndShape(false);
			
		ofPopMatrix();
	ofPopStyle();

}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // Use the 'o' key to change the formOnde (0, 1, 2)
    // Use the 'b' key to change the brillance

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    // Update mouseX and mouseY variables with the current mouse position
    // Use these variables to control 'volume' and 'f' (frequency) of the sound

}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer & buffer){
    // Call cbAudioProcess to fill the buffer with sound data
    cbAudioProcess(buffer);

	// Copy the processed audio data to audioBuffer for visualization in the draw() function
	for (size_t i = 0; i < buffer.getNumFrames(); i++){
		audioBuffer[i] = buffer.getSample(i, 0); // Mono output, so we take the first channel
	}
}


//--------------------------------------------------------------
void ofApp::cbAudioProcess(ofSoundBuffer & buffer){
   // Use 'volume' to control the overall volume of the sound (-> Amplitude)
   // Use 'f' to control the frequency of the sine wave

   // if formeOnde == 0, call calc_sin to fill the buffer with a sine wave
   // if formeOnde == 1, call calcul_carre to fill the buffer with a square wave
   // if formeOnde == 2, call calcul_scie to fill the buffer
}


//--------------------------------------------------------------
void ofApp::calc_sin(float A, float f, float t){
    // Fill the buffer with a sine wave of amplitude A, frequency f, and time t
}

//--------------------------------------------------------------
void ofApp::calcul_carre(float A, float f, float t, float brillance){
    // Fill the buffer with a square wave of amplitude A, frequency f, time t, and brightness brillance
}

//--------------------------------------------------------------
void ofApp::calcul_scie(float A, float f, float t, float brillance){
    // Fill the buffer with a sawtooth wave of amplitude A, frequency f, time t, and brightness brillance
}
