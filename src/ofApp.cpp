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
	gui.add(brillanceSliderGui.setup("Brillance", 3.0f, 1.0f, 32.0f));
	gui.add(frequencesGui.setup("Frequence", 440.0f, 1.0f, 22050.0f));

	// initialisation of keyboard
	for (int i = 0; i < 12; i++){
		float laFreq = 440.0f;
		int demiTon = i - 9;
		float f = laFreq * std::pow(2.0f, demiTon / 12.0f);
		oscillators[i].setFrequency(f);
	}
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

	float morph = oscillators[0].getMorphingFactor();
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
	// float laFreq = 440.0f;
	// int demiTon = 0;
	// bool notePressed = false;

	// Gestion clavier musical
    if(key == 'w') { oscillators[0].setNoteOn(true); }
    if(key == 's') { oscillators[1].setNoteOn(true); }
    if(key == 'x') { oscillators[2].setNoteOn(true); }
    if(key == 'd') { oscillators[3].setNoteOn(true); }
    if(key == 'c') { oscillators[4].setNoteOn(true); }
    if(key == 'v') { oscillators[5].setNoteOn(true); }
    if(key == 'g') { oscillators[6].setNoteOn(true); }
    if(key == 'b') { oscillators[7].setNoteOn(true); }
    if(key == 'h') { oscillators[8].setNoteOn(true); }
    if(key == 'n') { oscillators[9].setNoteOn(true); }
    if(key == 'j') { oscillators[10].setNoteOn(true); }
    if(key == ',') { oscillators[11].setNoteOn(true); }
	updateCurrentNotes();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == 'w'){oscillators[0].setNoteOn(false); }
	if(key == 's') {oscillators[1].setNoteOn(false); }
	if(key == 'x') {oscillators[2].setNoteOn(false); }
	if(key == 'd') {oscillators[3].setNoteOn(false); }
	if(key == 'c') {oscillators[4].setNoteOn(false); }
	if(key == 'v') {oscillators[5].setNoteOn(false); }
	if(key == 'g') {oscillators[6].setNoteOn(false); }
	if(key == 'b') {oscillators[7].setNoteOn(false); }
	if(key == 'h') {oscillators[8].setNoteOn(false); }
	if(key == 'n') {oscillators[9].setNoteOn(false); }
	if(key == 'j') {oscillators[10].setNoteOn(false);}
	if(key == ',') {oscillators[11].setNoteOn(false);}
	updateCurrentNotes();
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    // Map mouseX (0 to ~900) to morphing factor (0 to 1)
    // Left side = sine (0), middle = square (0.5), right side = sawtooth (1)
    float morphFactor = ofMap(x, 0, 900, 0.0f, 1.0f, true);
	float A = ofMap(y,0,ofGetHeight(),1,0); //axe négatif

	for (auto & osc : oscillators){
		osc.setMorphingFactor(morphFactor);	//float f = ofMap(x,0,ofGetWidth(),100,1000);
		osc.setAmplitude(A);
	}
    
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer & buffer){
		
	// set Brillance based on BrillanceSliderGui

	for (auto & osc : oscillators){
		osc.setBrillance(brillanceSliderGui);
	}

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
	for (size_t i = 0; i < buffer.size(); i++){
		buffer[i] = 0.0f;
	}

	ofSoundBuffer temp;
	temp.allocate(buffer.getNumFrames(), buffer.getNumChannels());

	for (auto & osc : oscillators){
		osc.get_signal(temp, buffer.getNumFrames());

		for (size_t i = 0; i < buffer.size(); i++){
			buffer[i] += temp[i];
		}
	}

	// Calcul du nombre de touche actives
	int nbActiveNote = 0;
	for (auto & osc : oscillators){
		if (osc.getNoteOn()) {nbActiveNote ++;}
	}

	// éviter saturation
    for(size_t i = 0; i < buffer.size(); i++)
        buffer[i] /= std::max(nbActiveNote, 1);
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

//--------------------------------------------------------------
void ofApp::updateCurrentNotes(){

    currentNote = "";

    for(int i = 0; i < oscillators.size(); i++){
        if(oscillators[i].getNoteOn()){
            currentNote += noteNames[i] + " ";
        }
    }
}
