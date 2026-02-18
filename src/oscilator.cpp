#include "oscilator.h"
#include "ofMain.h"
#include <cmath>


// Constructor to initialize the oscilator parameters
oscilator::oscilator() {
    
    // Initialize basic attributes
    t = 0.0f; // Time (legacy, kept for compatibility)
    noteOn = false; // No active note (no sound to generate)
    sampleRate = 44100.0f; // Standard sample rate for audio processing
    phase = 0.0f; // Initialize phase to zero
    morphSmoothing = 0.05f; // Smoothing factor for morphing transitions
    smoothingFactor = 0.05f; // Smoothing factor for frequency transitions
    
    // Use setters for bounds enforcement
    setAmplitude(1.0f); // Clamps to [0, 1]
    setFrequency(440.0f); // Clamps to [20, 20000], updates target frequency and phaseAdderTarget
    setBrillance(4.0f); // Clamps to [1, 32]
    setMorphingFactor(0.0f); // Clamps to [0, 1], starts with sine wave
    
    // Initialize phase adder to match target
    phaseAdder = phaseAdderTarget;
    morphingFactor = morphTargetFactor;

}

// Getters and Setters with bounds enforcement
float oscilator::getAmplitude() const { return A; }
void oscilator::setAmplitude(float amplitude) { 
    // Clamp amplitude to [0, 1]
    A = (amplitude < 0.0f) ? 0.0f : (amplitude > 1.0f) ? 1.0f : amplitude; 
}

float oscilator::getFrequency() const { return f; }
void oscilator::setFrequency(float frequency) { 
    // Clamp frequency to [20, 20000] Hz
    float clamped = (frequency < 20.0f) ? 20.0f : (frequency > 20000.0f) ? 20000.0f : frequency;
    f = clamped; 
    targetFrequency = clamped;
    phaseAdderTarget = (clamped / sampleRate) * TWO_PI;
}

float oscilator::getMorphingFactor() const { return morphingFactor; }
void oscilator::setMorphingFactor(float morph) { 
    // Clamp morphing factor to [0, 1] and update target
    morphTargetFactor = (morph < 0.0f) ? 0.0f : (morph > 1.0f) ? 1.0f : morph; 
}

float oscilator::getBrillance() const { return b; }
void oscilator::setBrillance(float brillance) { 
    // Clamp brillance to [1, 32]
    b = (brillance < 1.0f) ? 1.0f : (brillance > 32.0f) ? 32.0f : brillance; 
}

void oscilator::setSmoothingFactor(float factor) { smoothingFactor = factor; }
void oscilator::setMorphSmoothing(float factor) { morphSmoothing = factor; }
bool oscilator::getNoteOn() const { return noteOn; }
void oscilator::setNoteOn(bool value) { noteOn = value; }

//--------------------------------------------------------------
void  oscilator::get_signal(ofSoundBuffer & buffer, int n){

    // Smooth phaseAdder towards phaseAdderTarget for frequency transitions
    phaseAdder = (1 - smoothingFactor) * phaseAdder + smoothingFactor * phaseAdderTarget;
    
    // Smooth morphingFactor towards morphTargetFactor for waveform morphing
    morphingFactor = (1 - morphSmoothing) * morphingFactor + morphSmoothing * morphTargetFactor;

    // Generate the blended signal and fill the buffer
    if (noteOn) {
        generateBlendedSamples(buffer, n);
    } 
    else {
        // Silence when noteOn is false
        for (int i = 0; i < n; i++) {
            buffer[i*buffer.getNumChannels() + 0] = 0.0f;
            buffer[i*buffer.getNumChannels() + 1] = 0.0f;
        }
    }
}

//--------------------------------------------------------------
float oscilator::calc_sin_sample(){
    // Calculate and return a single sine wave sample
    float sample = A * sin(phase);
    phase += phaseAdder;
    return sample;
}

//--------------------------------------------------------------
float oscilator::calcul_carre_sample(){
    // Calculate and return a single square wave sample with harmonic interpolation
    int harmonics = (int)b; // Integer part: number of harmonics
    float blend = b - harmonics; // Fractional part: interpolation factor [0, 1]
    
    // Generate sample with 'harmonics' harmonic terms
    float sampleLow = 0;
    for (int k = 0; k <= harmonics; k++){
        sampleLow += sin((2 * k + 1) * phase) / (2 * k + 1);
    }
    sampleLow *= (4 / PI);
    sampleLow *= A; // Scale by amplitude
    
    // Generate sample with 'harmonics+1' harmonic terms
    float sampleHigh = 0;
    for (int k = 0; k <= harmonics + 1; k++){
        sampleHigh += sin((2 * k + 1) * phase) / (2 * k + 1);
    }
    sampleHigh *= (4 / PI);
    sampleHigh *= A; // Scale by amplitude
    
    // Linear interpolation between the two
    float sample = (1.0f - blend) * sampleLow + blend * sampleHigh;
    
    phase += phaseAdder;
    return sample;
}

//--------------------------------------------------------------
float oscilator::calcul_scie_sample(){
    // Calculate and return a single sawtooth wave sample with harmonic interpolation
    int harmonics = (int)b; // Integer part: number of harmonics
    float blend = b - harmonics; // Fractional part: interpolation factor [0, 1]
    
    // Generate sample with 'harmonics+1' harmonic terms
    float sampleLow = 0;
    for (int k = 1; k <= harmonics + 1; k++){
        sampleLow += pow(-1, k) * sin(k * phase) / k;
    }
    sampleLow *= (2 / PI);
    sampleLow *= A; // Scale by amplitude
    
    // Generate sample with 'harmonics+2' harmonic terms
    float sampleHigh = 0;
    for (int k = 1; k <= harmonics + 2; k++){
        sampleHigh += pow(-1, k) * sin(k * phase) / k;
    }
    sampleHigh *= (2 / PI);
    sampleHigh *= A; // Scale by amplitude
    
    // Linear interpolation between the two
    float sample = (1.0f - blend) * sampleLow + blend * sampleHigh;
    
    phase += phaseAdder;
    return sample;
}

//--------------------------------------------------------------
void oscilator::generateBlendedSamples(ofSoundBuffer & buffer, int n){
    // Generate blended waveform samples based on morphingFactor
    // 0.0 = sine, 0.5 = square, 1.0 = sawtooth
    for (int i = 0; i < n; i++) {
        float sample = 0.0f;
        
        // Save phase state to recompute waveforms independently
        float phaseSnapshot = phase;
        
        // Determine which waveforms to blend
        if (morphingFactor <= 0.5f) {
            // Blend between sine (0.0) and square (0.5)
            float alpha = morphingFactor * 2.0f; // Scale to [0, 1]
            
            // Reset phase and compute sine sample
            phase = phaseSnapshot;
            float sinSample = calc_sin_sample();
            
            // Reset phase and compute square sample
            phase = phaseSnapshot;
            float sqSample = calcul_carre_sample();
            
            // Linear blend
            sample = (1.0f - alpha) * sinSample + alpha * sqSample;
        } else {
            // Blend between square (0.5) and sawtooth (1.0)
            float alpha = (morphingFactor - 0.5f) * 2.0f; // Scale to [0, 1]
            
            // Reset phase and compute square sample
            phase = phaseSnapshot;
            float sqSample = calcul_carre_sample();
            
            // Reset phase and compute sawtooth sample
            phase = phaseSnapshot;
            float scieSample = calcul_scie_sample();
            
            // Linear blend
            sample = (1.0f - alpha) * sqSample + alpha * scieSample;
        }
        
        // Fill both channels with the blended sample
        buffer[i*buffer.getNumChannels() + 0] = sample; // Left channel
        buffer[i*buffer.getNumChannels() + 1] = sample; // Right channel
    }
}
