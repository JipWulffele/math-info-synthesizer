#include "biquadFilter.h"
#include <cmath>
#include <complex>

void BiquadFilter::setSampleRate(float sampleRate) { 
    fs = sampleRate; 
}

// reset la mémoire
void BiquadFilter::reset() {
    x1 = x2 = y1 = y2 = 0.0f;
}

// reset memoire et initialise les coeff à 0
BiquadFilter::BiquadFilter() {
    reset();
    b0 = b1 = b2 = a1 = a2 = 0.0f;
}

//---------------------------------------------------------
// fixe les propriétés des filtres
void BiquadFilter::setBPF(float newFo, float newQ) {
    fo = newFo;
    Q  = newQ;
    currentType = BPF;
    updateBPF();
}

void BiquadFilter::setLPF(float newFo, float newQ){
    fo = newFo;
    Q  = newQ;
    currentType = LPF;
    updateLPF();
}

//---------------------------------------------------------
// met à jour les coeff du filtre selon les paramètres du filtre
void BiquadFilter::updateBPF() {

    float wo    = (TWO_PI * fo) / fs;
    float alpha = sin(wo) / (2.0*Q);

    b0 =  alpha ;
    b1 =  0.0f;
    b2 = -alpha;

    a0 = 1.0f + alpha;
    a1 = -2.0f * cos(wo);
    a2 = 1.0f - alpha;
}

void BiquadFilter::updateLPF(){

    float wo    = (TWO_PI * fo) / fs;
    float alpha = sin(wo) / (2.0*Q);

    b0 = (1 - cos(wo))/2.0 ;
    b1 = 1 - cos(wo);
    b2 = (1 - cos(wo))/2.0 ;

    a0 = 1.0f + alpha;
    a1 = -2.0f * cos(wo);
    a2 = 1.0f - alpha;
}

//---------------------------------------------------------
// setter
void BiquadFilter::setFo(float newFo) {
    fo = newFo;

    if(currentType == LPF)
        updateLPF();
    else
        updateBPF();
}

void BiquadFilter::setQ(float newQ) {
    Q = newQ;

    if(currentType == LPF)
        updateLPF();
    else
        updateBPF();
}
//---------------------------------------------------------
// getter
float BiquadFilter::getFo() const { return fo; }
float BiquadFilter::getQ()  const { return Q;  }

//---------------------------------------------------------
float BiquadFilter::process(float x) {
    float y = (b0/a0)*x + (b1/a0)*x1 + (b2/a0)*x2 - (a1/a0)*y1 - (a2/a0)*y2;

    x2 = x1;
    x1 = x;

    y2 = y1;
    y1 = y;

    return y;
}

float BiquadFilter::getMagnitudeResponse(float freq)
{
    float omega = 2.0f * PI * freq / fs;

    std::complex<float> z1 = std::exp(std::complex<float>(0, -omega));
    std::complex<float> z2 = std::exp(std::complex<float>(0, -2.0f * omega));

    std::complex<float> numerator = b0 + b1 * z1 + b2 * z2;
    std::complex<float> denominator = 1.0f + a1 * z1 + a2 * z2;

    std::complex<float> H = numerator / denominator;

    return std::abs(H);
}

