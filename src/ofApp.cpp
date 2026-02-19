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
	gui.add(bourdonToggleGui.setup("Activate Bourdon",0));
	gui.add(bourdonFrequencesGui.setup("Frequence", 440.0f, 1.0f, 22050.0f));
	gui.add(amplitudeSliderGui.setup("Amplitude", 0.5f, 0.0f, 1.0f));
	gui.add(brillanceSliderGui.setup("Brillance", 3.0f, 1.0f, 10.0f));

    // Waveform amplitude sliders
	gui.add(ampSineGui.setup("Sine", 1.0f, 0.0f, 1.0f));
	gui.add(ampSquareGui.setup("Square", 0.0f, 0.0f, 1.0f));
	gui.add(ampSawtoothGui.setup("Sawtooth", 0.0f, 0.0f, 1.0f));
	gui.add(ampTriangleGui.setup("Triangle", 0.0f, 0.0f, 1.0f));


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
			for (unsigned int i = 0; i < audioBuffer.size(); i++){
				float x =  ofMap(i, 0, audioBuffer.size(), 0, 900, true);
				ofVertex(x, 100 -audioBuffer[i]*40.0f);
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
				ofVertex(x, 180 - audioFT[i]*200.0f);
			}
			ofEndShape(false);
			
		ofPopMatrix();
	ofPopStyle();

	gui.draw();
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

}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer & buffer){
		
	// set Brillance based on BrillanceSliderGui

	for (auto & osc : oscillators){
		osc.setBrillance(brillanceSliderGui);
		osc.setAmplitude(amplitudeSliderGui);
		osc.setAmpSine(ampSineGui);
		osc.setAmpSquare(ampSquareGui);
		osc.setAmpSawtooth(ampSawtoothGui);
		osc.setAmpTriangle(ampTriangleGui);
	}

	// manage bourdon
	bourdon.setFrequency(bourdonFrequencesGui);
	bourdon.setNoteOn(bourdonToggleGui);
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
	// for (size_t i = 0; i < buffer.size(); i++){
	// 	buffer[i] = 0.0f;
	// }
	
	// init signal with bourdon
	bourdon.get_signal(buffer, buffer.getNumFrames());

	// add other oscillator signal
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
	// Compute the Fourier transform of the aumyOscilator.setFormeOnde(0);dio buffer and store the result in fourierBuffer for visualization
	
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

    for(long unsigned int i = 0; i < oscillators.size(); i++){
        if(oscillators[i].getNoteOn()){
            currentNote += noteNames[i] + " ";
        }
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