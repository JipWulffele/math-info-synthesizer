#pragma once
#include "ofMain.h"

class BiquadFilter {
    public:
        BiquadFilter();

        void setSampleRate(float sampleRate);   // pour transférer l'info du sampleRate
        void reset();                           // reset mémoire interne

        void setBPF(float q, float fo);         // calcule les coeffs avec alpha et wo pour un filtre passe bande
        void setLPF(float q, float fo);         // calcule les coeffs avec alpha et wo pour un filtre passe bas

        void setFo(float newFo);
        void setQ(float newQ);

        float getFo() const;
        float getQ() const;

        float process(float sample);            // applique le filtre sur un échantillon
        float getMagnitudeResponse(float freq); // pour voir reponse du filtre théorique passe bande

    private:

        void updateLPF();
        void updateBPF();

        float fs = 44100.0f; // valeur par defaut

        // stockage et valeur par defaut des propriétes du filtre
        float fo = 1000.0f; 
        float Q  = 0.707f;

        float x1, x2, y1, y2; // mémoire du filtre
        float b0, b1, b2, a0, a1, a2; // coefficients

        enum Type {LPF, BPF};
        Type currentType = LPF;
};
