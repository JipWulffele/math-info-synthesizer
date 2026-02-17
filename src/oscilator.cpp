#include "oscilator.h"
#include <cmath>

//--------------------------------------------------------------
float oscilator::get_signal(float A, float f, float t, int formeOnde, float b){
    // if formeOnde == 0, call calc_sin to fill the buffer with a sine wave
    // if formeOnde == 1, call calcul_carre to fill the buffer with a square wave
    // if formeOnde == 2, call calcul_scie to fill the buffer

    float sample = 0.0;

    switch (formeOnde) {
        case 0:
            sample = calc_sin(A, f, t);
            break;
        case 1:
            sample = calcul_carre(A, f, t, b);
            break;
        case 2:
            sample = calcul_scie(A, f, t, b);
            break;
        default:    
            sample = 0.0f; // Default to silence if an invalid waveform type is provided
            break;
    };


    return sample;

}

//--------------------------------------------------------------
float oscilator::calc_sin(float A, float f, float t){
    return A * sin(2 * M_PI * f * t);

}

//--------------------------------------------------------------
float oscilator::calcul_carre(float A, float f, float t, float b){
    return 0.0f;
}

//--------------------------------------------------------------
float oscilator::calcul_scie(float A, float f, float t, float b){
    return 0.0f;
}
