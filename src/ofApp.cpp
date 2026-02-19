#include "ofApp.h"
#include <iostream>

using namespace std;
#include "BourdonMelodies.h"
#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){
	cout << "[SETUP START] Application initialization beginning" << endl;

    ofBackground(34, 34, 34);
	
	bufferSize	= 512;
	sampleRate 	= 44100;

    // initialize a audio buffer
	audioBuffer.assign(bufferSize, 0.0);
	audioFT.assign(bufferSize, 0.0); // Placeholder for Fourier transform buffer
	cout << "[SETUP] Buffers allocated - audioBuffer size: " << audioBuffer.size() << ", audioFT size: " << audioFT.size() << endl;
    // Prepare audio settings but DON'T start the stream yet
    ofSoundStreamSettings settings;

    auto devices = soundStream.getMatchingDevices("default");
	if(!devices.empty()){
		settings.setOutDevice(devices[0]);
		cout << "[SETUP] Audio device found: " << devices[0].deviceID << endl;
	} else {
		cout << "[SETUP WARNING] No audio device found!" << endl;
	}

	settings.setOutListener(this);
	settings.sampleRate = sampleRate;
	settings.numOutputChannels = 2;
	settings.numInputChannels = 0;
	settings.bufferSize = bufferSize;

	// setup gui Bourdon
	gui.setup("Synth");

	bourdonGui.setup("Bourdon");
	bourdonGui.setPosition(ofGetWidth()-bourdonGui.getWidth(), 10.0f);

	// # Bourdon
	bourdonGui.add(bourdonToggleGui.setup("Activate Bourdon (p)",0));
	bourdonGui.add(playBourdonMelodyButton.setup("Play Bourdon Melody",0)); // Bourdon Melody
	bourdonGui.add(bourdonFrequencesGui.setup("Frequence Bourdon", 440.0f, 1.0f, 22050.0f));
	bourdonGui.add(bourdonAmplitudeGui.setup ("Amplitude Bourdon", 0.5f, 0.0f, 1.0f));
	bourdonGui.add(bourdonBrillanceGui.setup ("Brillance Bourdon", 3.0f, 1.0f, 32.0f));
    // ## Waveform amplitude sliders
	bourdonGui.add(bourdonAmpSineGui.setup("Sine", 0.0f, 0.0f, 1.0f));
	bourdonGui.add(bourdonAmpSquareGui.setup("Square   Bourdon", 1.0f, 0.0f, 1.0f));
	bourdonGui.add(bourdonAmpSawtoothGui.setup("Sawtooth Bourdon", 0.0f, 0.0f, 1.0f));
	bourdonGui.add(bourdonAmpTriangleGui.setup("Triangle Bourdon", 0.0f, 0.0f, 1.0f));
	
	bourdonGui.add(mouseToggleGui.setup("Mouse control (m)",0));
	
	// add listener
	bourdonAmplitudeGui.addListener(this, & ofApp::onBourdonAmplitudeChanged);
	bourdonFrequencesGui.addListener(this, & ofApp::onBourdonFrequencyChanged);

	// # all the other oscillators mouseToggleGui;
	gui.add(amplitudeSliderGui.setup("Amplitude", 0.5f, 0.0f, 1.0f));
	gui.add(brillanceSliderGui.setup("Brillance", 3.0f, 1.0f, 32.0f));

    // ## Waveform amplitude sliders
	gui.add(ampSineGui.setup    ("Sine",     1.0f, 0.0f, 1.0f));
	gui.add(ampSquareGui.setup  ("Square",   0.0f, 0.0f, 1.0f));
	gui.add(ampSawtoothGui.setup("Sawtooth", 0.0f, 0.0f, 1.0f));
	gui.add(ampTriangleGui.setup("Triangle", 0.0f, 0.0f, 1.0f));
	

	// for (auto osc : oscillators){
	// 	auto onAmplitudeChanged= [&osc](float & value){
	// 		osc.setAmplitude(value);
	// 	};

	// 	auto onBrillanceChanged= [&osc](float & value){
	// 		osc.setBrillance(value);
	// 	};
		
	// 	//osc.setBrillance(brillanceSliderGui);

	// 	amplitudeSliderGui.addListener(this, & onAmplitudeChanged);
	// 	brillanceSliderGui.addListener(this, & onBrillanceChanged);
	// }

	// initialisation of keyboard
	cout << "[SETUP] Initializing 12 oscillators" << endl;
	for (int i = 0; i < 12; i++){
		float laFreq = 440.0f;
		int demiTon = i - 9;
		float f = laFreq * std::pow(2.0f, demiTon / 12.0f);
		baseFrequencies[i] = f;  // Store base frequency
		oscillators[i].setFrequency(f);
	}
	
	// NOW start the audio stream after everything is initialized
	cout << "[SETUP] About to call soundStream.setup() with bufferSize=" << bufferSize << ", sampleRate=" << sampleRate << endl;
	soundStream.setup(settings);
	cout << "[SETUP] soundStream.setup() completed" << endl;
	cout << "[SETUP COMPLETE] All initialization finished, ready for audio" << endl;
}

//--------------------------------------------------------------
void ofApp::update(){

    // Update bourdonMelody if it's playing
	if(bourdonMelodyPlaying && !melody.empty()){

		float now = ofGetElapsedTimef();
		float elapsed = now - bourdonStepStartTime;

		if(elapsed >= melody[bourdonStep].durationSec){

			// next step
			bourdonStep++;
			if(bourdonStep >= melody.size()){
				bourdonStep = 0; // loop
			}

			int idx = melody[bourdonStep].noteIndex;

			if(idx == -1){
				bourdonMelody.setNoteOn(false); // rest
			} else {
				bourdonMelody.setNoteOn(true);
				bourdonMelody.setFrequency(baseFrequencies[idx]);
			}

			bourdonStepStartTime = now;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Draw current state of audiobuffer on the screen
    static bool firstDraw = true;
    if (firstDraw) {
        cout << "[DRAW] First draw call - audioBuffer.size()=" << audioBuffer.size() << ", audioFT.size()=" << audioFT.size() << endl;
        firstDraw = false;
    }

    ofSetColor(225);
	ofDrawBitmapString("AUDIO OUTPUT", 32, 32);
	ofDrawBitmapString("Use sliders to mix waveforms (Sine, Square, Sawtooth, Triangle)", 31, 62);

	// Display current waveform mix
	ofDrawBitmapString("Waveform mix - Sine: " + std::to_string(ampSineGui).substr(0, 4) + 
	                    " Square: " + std::to_string(ampSquareGui).substr(0, 4) + 
	                    " Sawtooth: " + std::to_string(ampSawtoothGui).substr(0, 4) + 
	                    " Triangle: " + std::to_string(ampTriangleGui).substr(0, 4), 32, 92);
	ofDrawBitmapString("Current musical note: " + currentNote, 32, 107);

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
	if (audioBuffer.empty()) {
		ofLogWarning("ofApp") << "[DRAW WARNING] audioBuffer is empty!";
	} else {
		for (unsigned int i = 0; i < audioBuffer.size(); i++){
			float x =  ofMap(i, 0, audioBuffer.size(), 0, 900, true);
			ofVertex(x, 100 -audioBuffer[i]*40.0f);
		}
		ofEndShape(false);
	}
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
	if (audioFT.empty()) {
		ofLogWarning("ofApp") << "[DRAW WARNING] audioFT is empty!";
	} else {
		for (unsigned int i = 0; i < audioFT.size()/2; i++){
			float x =  ofMap(i, 0, audioFT.size()/2, 0, 900, true);
			ofVertex(x, 180 - audioFT[i]*200.0f);
		}
	}
	
	ofEndShape(false);
			
	ofPopMatrix();
	ofPopStyle();

	gui.draw();
	bourdonGui.draw();
	ofFill();
	drawKeyboard(32, 600, 900, 180);
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// float laFreq = 440.0f;
	// int demiTon = 0;
	// bool notePressed = false;

	// toggle mouse toggle
	if (key == 'm'){
		mouseToggleGui=  (mouseToggleGui?false:true);
	}

	// toggle bourdon
	if (key == 'p'){
		bourdonToggleGui=  (bourdonToggleGui?false:true);
	}
	
	// Numpad 0-9 for octave shifting
	if (key >= '0' && key <= '9') {
		octaveShift = key - '0';
		updateOctaveShift();
		//return;
	}

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
	if (mouseToggleGui){
		// Update mouseX and mouseY variables with the current mouse position
    	// Use these variables to control 'volume' and 'f' (frequency) of the sound
		bourdonFrequencesGui = ofMap(x,0,ofGetWidth(),100,1000);
		bourdonAmplitudeGui = ofMap(y,0,ofGetHeight(),1,0); //axe négatif
	}
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer & buffer){
	static bool firstAudioCallback = true;
	if (firstAudioCallback) {
		cout << "[AUDIO CALLBACK] First audioOut() call - buffer size=" << buffer.size() << ", audioBuffer.size()=" << audioBuffer.size() << endl;
		firstAudioCallback = false;
	}
		
	if (audioBuffer.empty()) {
		cout << "[AUDIO CALLBACK ERROR] audioBuffer is empty in audioOut()!" << endl;
	}
	
	// set Brillance based on BrillanceSliderGui

	for (auto & osc : oscillators){
		osc.setBrillance(brillanceSliderGui);
		// osc.setAmplitude(amplitudeSliderGui);
		osc.setAmpSine(ampSineGui);
		osc.setAmpSquare(ampSquareGui);
		osc.setAmpSawtooth(ampSawtoothGui);
		osc.setAmpTriangle(ampTriangleGui);
	}

	// manage bourdon
	// Note: Amplitude & Frequency are updated with listeners
	// see ofApp::onBourdonAmplitudeChanged & ofApp::onBourdonFrequencyChanged(float & value){
	bourdon.setNoteOn(bourdonToggleGui);
	bourdon.setBrillance  (bourdonBrillanceGui);
	bourdon.setAmpSine    (bourdonAmpSineGui);
	bourdon.setAmpSquare  (bourdonAmpSquareGui);
	bourdon.setAmpSawtooth(bourdonAmpSawtoothGui);
	bourdon.setAmpTriangle(bourdonAmpTriangleGui);

	// manage bourdonMelody
	bourdonMelody.setBrillance  (bourdonBrillanceGui);
	bourdonMelody.setAmpSine    (bourdonAmpSineGui);
	bourdonMelody.setAmpSquare  (bourdonAmpSquareGui);
	bourdonMelody.setAmpSawtooth(bourdonAmpSawtoothGui);
	bourdonMelody.setAmpTriangle(bourdonAmpTriangleGui);

    // Manage melody bourdon
	if(playBourdonMelodyButton){
		if(!bourdonMelodyPlaying){
			startBourdonMelody();
		}
	} else {
		if(bourdonMelodyPlaying){
			stopBourdonMelody();
		}
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
	static int callCount = 0;
	callCount++;
	if (callCount % 100 == 0) {
		cout << "[AUDIO PROCESS] Call #" << callCount << " - buffer.size()=" << buffer.size() << ", frames=" << buffer.getNumFrames() << endl;
	}
 	
	// Use the oscilator instance to generate the sound signal based on the current parameters
	if (buffer.size() == 0) {
		cout << "[AUDIO PROCESS ERROR] Buffer size is 0!" << endl;
		return;
	}
	
	for (size_t i = 0; i < buffer.size(); i++){
		buffer[i] = 0.0f;
	}
	
	// init signal with bourdon
	if (!bourdonMelodyPlaying){
		bourdon.get_signal(buffer, buffer.getNumFrames());
	} else {
		bourdonMelody.get_signal(buffer, buffer.getNumFrames());
	}

	// add other oscillator signal
	ofSoundBuffer temp;
	temp.allocate(buffer.getNumFrames(), buffer.getNumChannels());
	if (temp.size() == 0) {
		cout << "[AUDIO PROCESS ERROR] Temp buffer allocation failed!" << endl;
		return;
	}

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
	if (callCount % 100 == 0) {
		cout << "[AUDIO PROCESS] Active notes: " << nbActiveNote << endl;
	}

	// éviter saturation
    for(size_t i = 0; i < buffer.size(); i++)
        buffer[i] /= std::max(nbActiveNote, 1);
}

//--------------------------------------------------------------
// Add fourier transform function here (optional)
void ofApp::computeFT(vector <float> & audio){
	// Compute the Fourier transform of the aumyOscilator.setFormeOnde(0);dio buffer and store the result in fourierBuffer for visualization
	
	int n = audio.size(); // Number of samples in the audio buffer
	
	if (n <= 0 || n != (int)audioFT.size()) {
		cout << "[COMPUTE FT ERROR] Size mismatch - audio.size()=" << n << ", audioFT.size()=" << audioFT.size() << endl;
		return;
	}

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

    for(long unsigned int i = 0; i < oscillators.size(); i++){
        if(oscillators[i].getNoteOn()){
            currentNote += noteNames[i] + " ";
        }
    }
}

//--------------------------------------------------------------
void ofApp::updateOctaveShift(){
    // Apply octave shift to all oscillators
    for (int i = 0; i < 12; i++){
        float shiftedFreq = baseFrequencies[i] * std::pow(2.0f, octaveShift);
        oscillators[i].setFrequency(shiftedFreq);
    }
}

//--------------------------------------------------------------
void ofApp::drawKeyboard(float x, float y, float width, float height)
{
    float whiteWidth = width / 7.0f;
    float blackWidth = whiteWidth * 0.6f;
    float blackHeight = height * 0.6f;
	std::array<std::string, 12> keyLabels = {
		"W","S","X","D","C","V",
		"G","B","H","N","J",","
	};

    // Indique si la note est noire (12 demi-tons)
    std::array<bool, 12> isBlack = {
        false,true,false,true,false,
        false,true,false,true,false,true,false
    };

    // TOUCHES BLANCHES
    int whiteIndex = 0;

    for(int i = 0; i < 12; i++)
    {
        if(!isBlack[i])
        {
            float keyX = x + whiteIndex * whiteWidth;

            if(oscillators[i].getNoteOn())
                ofSetColor(0, 255, 120);   // couleur active
            else
                ofSetColor(255);           // blanc normal

            ofDrawRectangle(keyX, y, whiteWidth, height);

            // contour
            ofSetColor(0);
            ofNoFill();
            ofDrawRectangle(keyX, y, whiteWidth, height);
            ofFill();

            whiteIndex++;
			// Dessin du label
			ofSetColor(0); // noir pour texte sur blanc
			ofDrawBitmapString(keyLabels[i], keyX + whiteWidth/2 - 4, y + height - 20);
        }
    }

    // TOUCHES NOIRES
    whiteIndex = 0;

    for(int i = 0; i < 12; i++)
    {
        if(!isBlack[i])
        {
            whiteIndex++;
        }
        else
        {
            float keyX = x + whiteIndex * whiteWidth - blackWidth/2;

            if(oscillators[i].getNoteOn())
                ofSetColor(0, 255, 120);   // active
            else
                ofSetColor(0);             // noire normale

            ofDrawRectangle(keyX, y, blackWidth, blackHeight);
			// Dessin du label
			ofSetColor(255); // blanc sur noir
			ofDrawBitmapString(keyLabels[i], keyX + blackWidth/2 - 4, y + blackHeight - 8);
        }
    }
}

// callback listener
void ofApp::onBourdonAmplitudeChanged(float & value){
	bourdon.setAmplitude(value);
	bourdonMelody.setAmplitude(value);
}

void ofApp::onBourdonFrequencyChanged(float & value){
	bourdon.setFrequency(value);
	// Do not change the frequency of the bourdonMelody, as it is controlled by the melody steps 
}


void ofApp::startBourdonMelody(){
    if(melody.empty()) return;

    bourdonMelodyPlaying = true;
    bourdonStep = 0;
    bourdonStepStartTime = ofGetElapsedTimef();

    int idx = melody[bourdonStep].noteIndex;
	std::cout << "Starting bourdon melody, first note index: " << idx << std::endl;

    if(idx == -1){
        bourdonMelody.setNoteOn(false);
    } else {
        bourdonMelody.setNoteOn(true);
        bourdonMelody.setFrequency(baseFrequencies[idx]);
    }
}

void ofApp::stopBourdonMelody(){
	bourdonMelodyPlaying = false;
	bourdonMelody.setNoteOn(false);
}

