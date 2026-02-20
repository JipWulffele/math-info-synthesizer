#include "ofMain.h"
#include "biquadFilter.h"

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
        float getBrillance() const;
        void setBrillance(float brillance); // Clamped to [1, 32]
        
        // Waveform amplitude getters/setters
        float getAmpSine() const;
        void setAmpSine(float amp); // Clamped to [0, 1]
        float getAmpSquare() const;
        void setAmpSquare(float amp); // Clamped to [0, 1]
        float getAmpSawtooth() const;
        void setAmpSawtooth(float amp); // Clamped to [0, 1]
        float getAmpTriangle() const;
        void setAmpTriangle(float amp); // Clamped to [0, 1]
        
        void setSmoothingFactor(float factor); // Setter for frequency smoothing (default 0.05)
        void setWaveformSmoothing(float factor); // Setter for waveform amplitude smoothing (default 0.05)
        
        bool getNoteOn() const;
        void setNoteOn(bool value);

        // Filter
        BiquadFilter filter;
        bool filterActive; // option pour activer/desactiver le filtre
        void setFilterActive(bool active);
        bool getFilterActive() const;
        void setFilterBPF(float freq, float Q);
        void setFilterLPF(float freq, float Q);
        void setFilterFo(float fo);
        void setFilterQ(float Q);
        float getFilterFo() const;
        float getFilterQ() const;
        void setFilterSampleRate(float fs);


    private:
        // Attributes for the oscilator
        float A; // Amplitude [0, 1]
        float f; // Frequency [20, 20000] Hz
        float t; // Time (legacy, kept for compatibility)
        float b; // Brightness/Brillance [1, 32]
        bool noteOn; // If there is an active note to play
        
        // Waveform amplitude attributes
        float ampSine, ampSineTarget; // Sine amplitude [0, 1]
        float ampSquare, ampSquareTarget; // Square amplitude [0, 1]
        float ampSawtooth, ampSawtoothTarget; // Sawtooth amplitude [0, 1]
        float ampTriangle, ampTriangleTarget; // Triangle amplitude [0, 1]
        float waveformSmoothing; // Smoothing factor for waveform amplitude transitions (default 0.05)

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
        float envelopeIncrementDown; // Increment per sample for release (1.0 / releaseSamples)

        // Methods to generate different waveforms (return sample value, not buffer)
        float  calc_sin_sample();
        float  calcul_carre_sample(float brillance);
        float  calcul_scie_sample(float brillance);
        float  calc_triangle_sample(float brillance);
        
        // Helper method to update phase and wrap to [0, 2π]
        void update_phase();
        
        // Helper method for blended waveform generation
        void generateBlendedSamples(ofSoundBuffer & buffer, int n);
};