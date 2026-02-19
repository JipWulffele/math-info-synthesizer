#include "biquadFilter.h"
#include <cmath>

void BiquadFilter::setSampleRate(float sampleRate) { 
    fs = sampleRate; 
}

// reset memoire et initialise les coeff à 0
BiquadFilter::BiquadFilter() {
    reset();
    b0 = b1 = b2 = a1 = a2 = 0.0f;
}

// reset la mémoire
void BiquadFilter::reset() {
    x1 = x2 = y1 = y2 = 0.0f;
}

void BiquadFilter::setBPF(float fo, float q) {
    
    float wo    = (TWO_PI * fo) / fs;
    float alpha = sin(wo) / (2*q);

    b0 =  alpha ;
    b1 =  0.0f;
    b2 = -alpha;

    a0 = 1.0f + alpha;
    a1 = -2.0f * cos(wo);
    a2 = 1.0f - alpha;
}

float BiquadFilter::process(float x) {
    float y = (b0/a0)*x + (b1/a0)*x1 + (b2/a0)*x2 - (a1/a0)*y1 - (a2/a0)*y2;

    x2 = x1;
    x1 = x;

    y2 = y1;
    y1 = y;

    return y;
}