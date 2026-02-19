#include "ofMain.h"

class BiquadFilter {
    public:
        BiquadFilter();
        void setSampleRate(float sampleRate); // pour transférer l'info du sampleRate
        void reset();                         // reset mémoire interne
        void setBPF(float q, float fo);       // calcule les coeffs avec alpha et wo
        float process(float sample);          // applique le filtre sur un échantillon

    private:
        float x1, x2, y1, y2; // mémoire du filtre
        float b0, b1, b2, a0, a1, a2; // coefficients
        float fs = 44100.0f; // valeur par defaut
};
