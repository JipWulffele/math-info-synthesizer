#include "ofMain.h"

class oscilator {
    public:
        oscilator(); // Constructor to initialize the oscilator parameters

        // Main method: generate a signal based on the current parameters
        void get_signal(ofSoundBuffer & buffer, int n); 
        
        // Getters and Setters with bounds enforcement
        float getAmplitude() const;
        void setAmplitude(float amplitude); // Clamped to [0, 1]
        float getFrequency() const;
        void setFrequency(float frequency); // Clamped to [20, 20000] Hz
        float getMorphingFactor() const;
        void setMorphingFactor(float morph); // Clamped to [0, 1]; 0=sine, 0.5=square, 1=sawtooth
        float getBrillance() const;
        void setBrillance(float brillance); // Clamped to [1, 32]
        
        void setSmoothingFactor(float factor); // Setter for frequency smoothing (default 0.05)
        void setMorphSmoothing(float factor); // Setter for morphing smoothing (default 0.05)
        
        bool getNoteOn() const;
        void setNoteOn(bool value);

    private:
        // Attributes for the oscilator
        float A; // Amplitude [0, 1]
        float f; // Frequency [20, 20000] Hz
        float t; // Time (legacy, kept for compatibility)
        float morphingFactor; // Waveform morphing [0, 1]: 0=sine, 0.5=square, 1=sawtooth
        float morphTargetFactor; // Target morphing factor for smooth transitions
        float morphSmoothing; // Smoothing factor for morphing transitions (e.g., 0.05)
        float b; // Brightness/Brillance [1, 32]
        bool noteOn; // If there is an active note to play

        float sampleRate; // Sample rate for audio processing

        // Phase attributes
        float targetFrequency;
        float phase; // Current phase of the waveform
        float phaseAdder;
		float phaseAdderTarget;
        float smoothingFactor; // Smoothing factor for frequency transitions

        // Envelope attributes (attack/release)
        float volumeEnvelope; // Current envelope level [0, 1]
        float volumeTarget; // Target envelope level (0 for release, 1 for attack)
        bool prevNoteOn; // Previous noteOn state to detect transitions
        int attackSamples; // Number of samples for attack (1ms)
        int releaseSamples; // Number of samples for release (1ms)
        float envelopeIncrement; // Increment per sample (1.0 / attackSamples)

        // Methods to generate different waveforms (return sample value, not buffer)
        float  calc_sin_sample();
        float  calcul_carre_sample();
        float  calcul_scie_sample();
        float  calc_triangle_sample();
        
        // Helper method for blended waveform generation
        void generateBlendedSamples(ofSoundBuffer & buffer, int n);
};