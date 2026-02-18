#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(34, 34, 34);
	
	bufferSize	= 512;
	sampleRate 	= 44100;

    // initialize a audio buffer
	audioBuffer.assign(bufferSize, 0.0);
	audioFT.assign(bufferSize, 0.0); // Placeholder for Fourier transform buffer

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

	// setup gui IHM
	gui.setup("Synth");
	gui.add(brillanceSliderGui.setup("Brillance", 3.0f, 0.0f, 20.0f));
	gui.add(frequencesGui.setup("Frequence", 440.0f, 1.0f, 22050.0f));

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    // Draw current state of audiobuffer on the screen

    ofSetColor(225);
	ofDrawBitmapString("AUDIO OUTPUT", 32, 32);
	ofDrawBitmapString("Move mouse left/right to morph waveform (sine → square → sawtooth)", 31, 62);

	// Indiquer la forme d'onde actuelle and morphing factor
	std::string waveName;
	float morph = myOscilator.getMorphingFactor();
	if(morph < 0.25f) waveName = "Sine to Square";
	else if(morph < 0.5f) waveName = "Square (blend from Sine)";
	else if(morph < 0.75f) waveName = "Square to Sawtooth";
	else waveName = "Sawtooth (blend from Square)";

	ofDrawBitmapString("Current waveform: " + waveName, 32, 92);
	ofDrawBitmapString("Morphing factor: " + std::to_string(morph).substr(0, 4), 32, 107);
	ofDrawBitmapString("Current musical note: " + currentNote, 32, 122);

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
				ofVertex(x, 100 -audioBuffer[i]*90.0f);
			}
			ofEndShape(false);
			
		ofPopMatrix();
	ofPopStyle();

	// Add Fourier transform visualization here (optional)
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, 350, 0);
			
		ofSetColor(225);
		ofDrawBitmapString("Fourier Transform", 4, 18);
		
		ofSetLineWidth(1);	
		ofDrawRectangle(0, 0, 900, 200);

		ofSetColor(58, 135, 245);
		ofSetLineWidth(3);
					
			ofBeginShape();
			for (unsigned int i = 0; i < audioFT.size()/2; i++){
				float x =  ofMap(i, 0, audioFT.size()/2, 0, 900, true);
				ofVertex(x, 180 - audioFT[i]*300.0f);
			}
			ofEndShape(false);
			
		ofPopMatrix();
	ofPopStyle();

	gui.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // Use the 'o' key to change the formOnde (0, 1, 2)
    // Use the 'b' key to change the brillance

	float laFreq = 440.0f;
	int demiTon = 0;
	bool notePressed = false;

	// Gestion clavier musical
    if(key == 'w')      { demiTon = -9; notePressed = true; currentNote = "Do"; }
    else if(key == 's') { demiTon = -8; notePressed = true; currentNote = "Do#"; }
    else if(key == 'x') { demiTon = -7; notePressed = true; currentNote = "Re"; }
    else if(key == 'd') { demiTon = -6; notePressed = true; currentNote = "Re#"; }
    else if(key == 'c') { demiTon = -5; notePressed = true; currentNote = "Mi"; }
    else if(key == 'v') { demiTon = -4; notePressed = true; currentNote = "Fa"; }
    else if(key == 'g') { demiTon = -3; notePressed = true; currentNote = "Fa#"; }
    else if(key == 'b') { demiTon = -2; notePressed = true; currentNote = "Sol"; }
    else if(key == 'h') { demiTon = -1; notePressed = true; currentNote = "Sol#"; }
    else if(key == 'n') { demiTon = 0;  notePressed = true; currentNote = "La"; }
    else if(key == 'j') { demiTon = 1;  notePressed = true; currentNote = "La#"; }
    else if(key == ',') { demiTon = 2;  notePressed = true; currentNote = "Si"; }

	if (notePressed) {
		myOscilator.setNoteOn(true);
		float f = laFreq * std::pow(2.0f, demiTon / 12.0f);
		myOscilator.setFrequency(f);
		frequencesGui= f;

	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == 'w')     {myOscilator.setNoteOn(false); currentNote = "";}
	else if(key == 's') {myOscilator.setNoteOn(false); currentNote = "";}
	else if(key == 'x') {myOscilator.setNoteOn(false); currentNote = "";}
	else if(key == 'd') {myOscilator.setNoteOn(false); currentNote = "";}
	else if(key == 'c') {myOscilator.setNoteOn(false); currentNote = "";}
	else if(key == 'v') {myOscilator.setNoteOn(false); currentNote = "";}
	else if(key == 'g') {myOscilator.setNoteOn(false); currentNote = "";}
	else if(key == 'b') {myOscilator.setNoteOn(false); currentNote = "";}
	else if(key == 'h') {myOscilator.setNoteOn(false); currentNote = "";}
	else if(key == 'n') {myOscilator.setNoteOn(false); currentNote = "";}
	else if(key == 'j') {myOscilator.setNoteOn(false); currentNote = "";}
	else if(key == ',') {myOscilator.setNoteOn(false); currentNote = "";}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    // Map mouseX (0 to ~900) to morphing factor (0 to 1)
    // Left side = sine (0), middle = square (0.5), right side = sawtooth (1)
    float morphFactor = ofMap(x, 0, 900, 0.0f, 1.0f, true);
    myOscilator.setMorphingFactor(morphFactor);	//float f = ofMap(x,0,ofGetWidth(),100,1000);
	
	float A = ofMap(y,0,ofGetHeight(),1,0); //axe négatif
	myOscilator.setAmplitude(A);
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer & buffer){
		
	// set Brillance based on BrillanceSliderGui
	myOscilator.setBrillance(brillanceSliderGui);
	//myOscilator.setFrequency(frequencesGui);

    // Call cbAudioProcess to fill the buffer with sound data
    cbAudioProcess(buffer);

	// Compute the Fourier transform of the audio buffer for visualization (optional)
	computeFT(audioBuffer);

	// Copy the processed audio data to audioBuffer for visualization in the draw() function
	for (unsigned int i = 0; i < buffer.getNumFrames(); i++){ // buffer.getNumFrames() is unsigned
		audioBuffer[i] = buffer.getSample(i, 0); // Mono output, so we take the first channel
	}
}

//--------------------------------------------------------------
void ofApp::cbAudioProcess(ofSoundBuffer & buffer){
    
	// Use the oscilator instance to generate the sound signal based on the current parameters
	myOscilator.get_signal(buffer, buffer.getNumFrames());
}

//--------------------------------------------------------------
// Add fourier transform function here (optional)
void ofApp::computeFT(vector <float> & audio){
	// Compute the Fourier transform of the audio buffer and store the result in fourierBuffer for visualization
	
	int n = audio.size(); // Number of samples in the audio buffer

	// Compute the real and imaginary parts of the fourier transform
	float realPart[n], imagPart[n]; // Array to hold the real and imaginary parts of the fourier transform

	// Compute the fourier transform using the DFT formula
	for (int k = 0; k < n; k++) { // For each output
		realPart[k] = 0;
		imagPart[k] = 0;
		for (int t = 0; t < n; t++) { // For each input
			float angle = (2 * PI * t * k) / n;
			realPart[k] += audio[t] * cos(angle);
			imagPart[k] -= audio[t] * sin(angle); // Negative sign for the imaginary part
		}
		// Calculate FT magnitude and normalize by n
		audioFT[k] = sqrt(realPart[k] * realPart[k] + imagPart[k] * imagPart[k]) / n;
	}
}
