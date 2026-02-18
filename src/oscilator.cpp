#include "oscilator.h"
#include "ofMain.h"
#include <cmath>


// Constructor to initialize the oscilator parameters
oscilator::oscilator() {
    
    // Audio parameters
    A = 1.0f; // Amplitude
    f = 440.0f; // Frequency (440 Hz is the standard A note
    t = 0.0f; // Time
    formeOnde = 0; // Start with sine wave
    b = 1.0f; // Medium brightness
    
    sampleRate = 44100.0f; // Standard sample rate for audio processing
    
    // Parameters for phase calculation
    targetFrequency = f; // Initialize target frequency to the current frequency
    phase = 0.0f; // Initialize phase to zero
    phaseAdder = 0.0f; // Initialize phase adder to zero
    phaseAdderTarget = (f / sampleRate) * TWO_PI; // Calculate initial phase adder target based on the frequency and sample rate
}

// Getters and Setters for the oscilator parameters 
float oscilator::getAmplitude() const { return A; }
void oscilator::setAmplitude(float amplitude) { A = amplitude; }
float oscilator::getFrequency() const { return f; }
void oscilator::setFrequency(float frequency) { f = frequency; }
int oscilator::getFormeOnde() const { return formeOnde; }
void oscilator::setFormeOnde(int forme) { formeOnde = forme; }
float oscilator::getBrillance() const { return b; }
void oscilator::setBrillance(float brillance) { b = brillance; }

//--------------------------------------------------------------
void  oscilator::get_signal(ofSoundBuffer & buffer, int n){
    // if formeOnde == 0, call calc_sin to fill the buffer with a sine wave
    // if formeOnde == 1, call calcul_carre to fill the buffer with a square wave
    // if formeOnde == 2, call calcul_scie to fill the buffer

    switch (formeOnde) {
        case 0:
            calc_sin(buffer, n);
            break;
        case 1:
            calcul_carre(buffer, n);
            break;
        case 2:
            calcul_scie(buffer, n);
            break;
        default:    
            for (int i = 0; i < n; i++) {
                buffer[i] = 0.0f;
            }
            break;
    };

}

//--------------------------------------------------------------
void oscilator::calc_sin(ofSoundBuffer & buffer, int n){
    // Calculate the sine wave values for the given amplitude A, frequency f, time t, and fill the buffer with the generated samples
    for (int i = 0; i < n; i++){
    	
        float sample;
	    sample = A * sin(2 * M_PI * f * t);
        
        // Fill the buffer with the generated sample (same for left and right channels for mono output)
        buffer[i*buffer.getNumChannels() + 0] = sample; // Left channel
	    buffer[i*buffer.getNumChannels() + 1] = sample; // Right channel (same as left for mono output)
        t += 1.0f / sampleRate;
   }

}

//--------------------------------------------------------------
void oscilator::calcul_carre(ofSoundBuffer & buffer, int n){
    // Calculate the square wave values for the given amplitude A, frequency f, time t, and fill the buffer with the generated samples
    for (int i = 0; i < n; i++){
    	
        float sample = 0;
	    for (int k=0; k <= b; k++){
            sample += sin((2 * k + 1) * 2 * PI * f * t ) / (2 * k + 1);
        }
        sample *= (4 / PI);
        sample *= A; // Scale by amplitude


        // Fill the buffer with the generated sample (same for left and right channels for mono output)
        buffer[i*buffer.getNumChannels() + 0] = sample; // Left channel
	    buffer[i*buffer.getNumChannels() + 1] = sample; // Right channel (same as left for mono output)
        t += 1.0f / sampleRate;
   }

}

//--------------------------------------------------------------
void oscilator::calcul_scie(ofSoundBuffer & buffer, int n){
    // Calculate the sawtooth wave values for the given amplitude A, frequency f, time t, and fill the buffer with the generated samples
    for (int i = 0; i < n; i++){
    	
        float sample = 0;
	    for (int k=1; k <= b+1; k++){
            sample += pow(-1, k) * sin(2 * k * PI * f * t ) / (k);
        }
        sample *= (2 / PI);
        sample *= A; // Scale by amplitude


        // Fill the buffer with the generated sample (same for left and right channels for mono output)
        buffer[i*buffer.getNumChannels() + 0] = sample; // Left channel
	    buffer[i*buffer.getNumChannels() + 1] = sample; // Right channel (same as left for mono output)
        t += 1.0f / sampleRate;
   }

}
