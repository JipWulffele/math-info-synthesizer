#include "oscilator.h"
#include "ofMain.h"
#include <cmath>


// Constructor to initialize the oscilator parameters
oscilator::oscilator() {
    
    // Initialize basic attributes
    t = 0.0f; // Time (legacy, kept for compatibility)
    noteOn = false; // No active note (no sound to generate)
    prevNoteOn = false; // Previous noteOn state
    sampleRate = 44100.0f; // Standard sample rate for audio processing
    phase = 0.0f; // Initialize phase to zero
    waveformSmoothing = 0.05f; // Smoothing factor for waveform amplitude transitions
    smoothingFactor = 0.05f; // Smoothing factor for frequency transitions
    
    // Initialize envelope attributes: smoothing volume
    attackSamples = (int)(sampleRate / 100.0f); // 10ms attack
    releaseSamples = (int)(sampleRate / 100.0f); // 10ms release
    envelopeIncrement = 1.0f / attackSamples; // Increment per sample
    volumeEnvelope = 0.0f; // Start silent
    volumeTarget = 0.0f; // Target is OFF initially
    
    // Use setters for bounds enforcement
    setAmplitude(1.0f); // Clamps to [0, 1]
    setFrequency(440.0f); // Clamps to [20, 20000], updates target frequency and phaseAdderTarget
    setBrillance(4.0f); // Clamps to [1, 32]
    
    // Initialize waveform amplitudes: sine by default
    setAmpSine(1.0f); // Sine wave
    setAmpSquare(0.0f); // No square
    setAmpSawtooth(0.0f); // No sawtooth
    setAmpTriangle(0.0f); // No triangle
    
    // Initialize phase adder to match target
    phaseAdder = phaseAdderTarget;

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
    phaseAdder = phaseAdderTarget;
}

// Waveform amplitude getters and setters
float oscilator::getAmpSine() const { return ampSine; }
void oscilator::setAmpSine(float amp) { 
    ampSineTarget = (amp < 0.0f) ? 0.0f : (amp > 1.0f) ? 1.0f : amp; 
}

float oscilator::getAmpSquare() const { return ampSquare; }
void oscilator::setAmpSquare(float amp) { 
    ampSquareTarget = (amp < 0.0f) ? 0.0f : (amp > 1.0f) ? 1.0f : amp; 
}

float oscilator::getAmpSawtooth() const { return ampSawtooth; }
void oscilator::setAmpSawtooth(float amp) { 
    ampSawtoothTarget = (amp < 0.0f) ? 0.0f : (amp > 1.0f) ? 1.0f : amp; 
}

float oscilator::getAmpTriangle() const { return ampTriangle; }
void oscilator::setAmpTriangle(float amp) { 
    ampTriangleTarget = (amp < 0.0f) ? 0.0f : (amp > 1.0f) ? 1.0f : amp; 
}

float oscilator::getBrillance() const { return b; }
void oscilator::setBrillance(float brillance) { 
    // Clamp brillance to [1, 10]
    b = (brillance < 1.0f) ? 1.0f : (brillance > 10.0f) ? 10.0f : brillance; 
}

void oscilator::setSmoothingFactor(float factor) { smoothingFactor = factor; }
void oscilator::setWaveformSmoothing(float factor) { waveformSmoothing = factor; }
bool oscilator::getNoteOn() const { return noteOn; }
void oscilator::setNoteOn(bool value) { noteOn = value; }

//--------------------------------------------------------------
void  oscilator::get_signal(ofSoundBuffer & buffer, int n){

    // Smooth phaseAdder towards phaseAdderTarget for frequency transitions
    phaseAdder = (1 - smoothingFactor) * phaseAdder + smoothingFactor * phaseAdderTarget;
    
    // Smooth waveform amplitudes towards their targets
    ampSine = (1 - waveformSmoothing) * ampSine + waveformSmoothing * ampSineTarget;
    ampSquare = (1 - waveformSmoothing) * ampSquare + waveformSmoothing * ampSquareTarget;
    ampSawtooth = (1 - waveformSmoothing) * ampSawtooth + waveformSmoothing * ampSawtoothTarget;
    ampTriangle = (1 - waveformSmoothing) * ampTriangle + waveformSmoothing * ampTriangleTarget;

    // Handle noteOn/Off transitions for envelope
    if (noteOn != prevNoteOn) {
        if (noteOn) {
            volumeTarget = 1.0f; // Attack: ramp to full volume
        } 
    else {
            volumeTarget = 0.0f; // Release: ramp to silence
        }
        prevNoteOn = noteOn;
    }

    // Always generate blended samples (envelope handles silence)
    generateBlendedSamples(buffer, n);
}

//--------------------------------------------------------------
void oscilator::update_phase(){
    // Update phase and wrap to [0, 2π] to prevent overflow
    phase += phaseAdder;
    phase = fmod(phase, TWO_PI);
}

//--------------------------------------------------------------
float oscilator::calc_sin_sample(){
    // Calculate and return a single sine wave sample
    float sample = A * sin(phase);
    update_phase();
    return sample;
}

//--------------------------------------------------------------
float oscilator::calcul_carre_sample(float brillance){
    // Simplified square wave: sum odd harmonics with an optional partial last harmonic
    int N = (int)brillance;            // integer part
    float blend = brillance - N;       // fractional part [0..1]

    float s = 0.0f;
    // Add N harmonics fully
    for (int k = 0; k < N; k++) {
        int n = 2 * k + 1;
        s += sin(n * phase) / (float)n;
    }
    // Add the next harmonic partially
    int k = N;
    int n = 2 * k + 1;
    s += blend * (sin(n * phase) / (float)n);

    // Scale
    s *= (4.0f / PI);
    s *= A;

    update_phase();
    return s;
}

//--------------------------------------------------------------
float oscilator::calcul_scie_sample(float brillance){
    // Simplified sawtooth: sum harmonics with optional partial last harmonic
    int N = (int)brillance;            // integer part
    float blend = brillance - N;       // fractional part [0..1]

    float s = 0.0f;
    // Add N harmonics fully (k from 1..N)
    for (int k = 1; k <= N; k++) {
        s += pow(-1.0f, k) * sin(k * phase) / (float)k;
    }
    // Add the next harmonic partially
    int k = N + 1;
    s += blend * (pow(-1.0f, k) * sin(k * phase) / (float)k);

    // Scale
    s *= (2.0f / PI);
    s *= A;

    update_phase();
    return s;
}

//--------------------------------------------------------------
float oscilator::calc_triangle_sample(float brillance) {
    int N = (int)brillance;            // integer part
    float blend = brillance - N;       // fractional part [0..1]

    float s = 0.0f;

    // Add N harmonics fully
    for (int k = 0; k < N; k++) {
        int n = 2 * k + 1;
        float sign = (k & 1) ? -1.0f : 1.0f;
        s += sign * sin(n * phase) / (float)(n * n);
    }

    // Add the next harmonic partially
    int k = N;
    int n = 2 * k + 1;
    float sign = (k & 1) ? -1.0f : 1.0f;
    s += blend * (sign * sin(n * phase) / (float)(n * n));

    // Scale
    s *= (8.0f / (PI * PI));
    s *= A;

    update_phase();
    return s;
}

//--------------------------------------------------------------
void oscilator::generateBlendedSamples(ofSoundBuffer & buffer, int n){
    // Compute capped brillance to prevent aliasing
    float actualFrequency = (phaseAdder / TWO_PI) * sampleRate;
    float maxHarmonics = floor((sampleRate / 2.0f) / actualFrequency);
    float cappedB = std::min(b, maxHarmonics);

    // Generate blended waveform samples mixing sine, square, sawtooth, and triangle
    for (int i = 0; i < n; i++) {
        float sample = 0.0f;

        // Save phase state to recompute waveforms independently
        float phaseSnapshot = phase;

        // Generate all 4 waveforms
        phase = phaseSnapshot;
        float sinSample = calc_sin_sample();

        phase = phaseSnapshot;
        float sqSample = calcul_carre_sample(cappedB);

        phase = phaseSnapshot;
        float scieSample = calcul_scie_sample(cappedB);

        phase = phaseSnapshot;
        float triSample = calc_triangle_sample(cappedB);

        // Mix waveforms using their (already relative) amplitudes
        sample = ampSine * sinSample + ampSquare * sqSample + ampSawtooth * scieSample + ampTriangle * triSample;

        // Apply envelope (attack/release)
        if (volumeEnvelope < volumeTarget) {
            // Attack: ramp up
            volumeEnvelope += envelopeIncrement;
            if (volumeEnvelope > volumeTarget) {
                volumeEnvelope = volumeTarget;
            }
        } else if (volumeEnvelope > volumeTarget) {
            // Release: ramp down
            volumeEnvelope -= envelopeIncrement;
            if (volumeEnvelope < volumeTarget) {
                volumeEnvelope = volumeTarget;
            }
        }

        // Scale sample by envelope
        sample *= volumeEnvelope;

        // Fill both channels with the blended sample
        buffer[i*buffer.getNumChannels() + 0] = sample; // Left channel
        buffer[i*buffer.getNumChannels() + 1] = sample; // Right channel
    }
}
