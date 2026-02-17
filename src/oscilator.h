
class oscilator {
    public:

        // Main method: generate a signal based on the current parameters
        // A is amplitude
        // f is frequency
        // t is time
        // formeOnde is the waveform type (0 = sine, 1 = square, 2 = sawtooth)
        // b is brightness
        static float get_signal(float A, float f, float t, int formeOnde, float b); 

    private:
        // Methods to generate different waveforms
        static float calc_sin(float A, float f, float t);
        static float calcul_carre(float A, float f, float t, float b);
        static float calcul_scie(float A, float f, float t, float b);
};