#include "ofMain.h"

class oscilator {
    public:
        oscilator(); // Constructor to initialize the oscilator parameters

        // Main method: generate a signal based on the current parameters
        void get_signal(ofSoundBuffer & buffer, int n); 
        
        // Getters and Setters
        float getAmplitude() const;
        void setAmplitude(float amplitude);
        float getFrequency() const;
        void setFrequency(float frequency);
        int getFormeOnde() const;
        void setFormeOnde(int forme);
        float getBrillance() const;
        void setBrillance(float brillance);
        
        void setSmoothingFactor(float factor); // Setter for smoothing factor (controls how quickly frequency changes; e.g., 0.05 for fast smoothing)
        
        bool getNoteOn() const;
        void setNoteOn(bool value);

    private:
        // Attributes for the oscilator
        float A; // Amplitude
        float f; // Frequency
        float t; // Time
        int formeOnde; // Waveform type (0, 1, 2)
        float b; // Brightness of the sound
        bool noteOn; // If there is an active note to play

        float sampleRate; // Sample rate for audio processing

        // Phase attributes
        float targetFrequency;
        float phase; // Current phase of the waveform
        float phaseAdder;
		float phaseAdderTarget;
        float smoothingFactor; // Smoothing factor for frequency transitions (e.g., 0.05 means 5% towards target per get_signal call)

        // Methods to generate different waveforms
        void  calc_sin(ofSoundBuffer & buffer, int n);
        void  calcul_carre(ofSoundBuffer & buffer, int n);
        void  calcul_scie(ofSoundBuffer & buffer, int n);
};