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