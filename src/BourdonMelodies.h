#pragma once
#include <vector>
#include <string>

struct MelodyStep {
    int noteIndex;      // 0..11 (or -1 for rest)
    float durationSec;  // seconds
};

namespace BourdonMelodies {

    // ----------------------------
    // Song 1
    // ----------------------------
    static std::vector<MelodyStep> song1 = {
        // Old MacDonald had a farm, E-I-E-I-O
        {0,0.3f}, {0,0.3f}, {0,0.3f}, {7,0.3f}, {9,0.3f}, {9,0.3f}, {7,0.5f},
        {4,0.3f}, {4,0.3f}, {2,0.3f}, {2,0.3f}, {0,0.6f},

        // And on his farm he had a cow, E-I-E-I-O
        {7,0.3f}, {7,0.3f}, {4,0.3f}, {4,0.3f}, {2,0.3f}, {2,0.3f}, {0,0.5f},
        {9,0.3f}, {9,0.3f}, {7,0.3f}, {4,0.3f}, {2,0.6f},

        // With a moo moo here and a moo moo there
        {0,0.3f}, {0,0.3f}, {0,0.3f}, {0,0.3f}, {0,0.3f}, {0,0.3f},
        {0,0.3f}, {0,0.3f}, {0,0.5f},

        // Here a moo, there a moo, everywhere a moo moo
        {0,0.3f}, {0,0.3f}, {0,0.3f}, {0,0.3f}, {0,0.3f}, {0,0.3f},
        {0,0.3f}, {0,0.3f}, {0,0.5f},

        // Old MacDonald had a farm, E-I-E-I-O
        {0,0.3f}, {0,0.3f}, {0,0.3f}, {7,0.3f}, {9,0.3f}, {9,0.3f}, {7,0.5f},
        {4,0.3f}, {4,0.3f}, {2,0.3f}, {2,0.3f}, {0,0.75f}
    };

    // ----------------------------
    // Song 2
    // ----------------------------
    static std::vector<MelodyStep> song2 = {
        {0,0.3f}, {0,0.3f}, {7,0.3f}, {7,0.3f}, {9,0.3f}, {9,0.3f}, {7,0.5f},
        {5,0.3f}, {5,0.3f}, {4,0.3f}, {4,0.3f}, {2,0.3f}, {2,0.3f}, {0,0.5f},

        {7,0.3f}, {7,0.3f}, {5,0.3f}, {5,0.3f}, {4,0.3f}, {4,0.3f}, {2,0.5f},
        {7,0.3f}, {7,0.3f}, {5,0.3f}, {5,0.3f}, {4,0.3f}, {4,0.3f}, {2,0.5f},

        {0,0.3f}, {0,0.3f}, {7,0.3f}, {7,0.3f}, {9,0.3f}, {9,0.3f}, {7,0.5f},
        {5,0.3f}, {5,0.3f}, {4,0.3f}, {4,0.3f}, {2,0.3f}, {2,0.3f}, {0,0.6f}        
    };

    // ----------------------------
    // Song3
    // ----------------------------
    static std::vector<MelodyStep> song3 = {
        // The itsy bitsy spider climbed up the waterspout
        {7,0.3f}, {0,0.3f}, {0,0.3f}, {2,0.3f}, {4,0.6f},
        {4,0.3f}, {2,0.3f}, {0,0.3f}, {2,0.3f}, {4,0.6f},
        {7,0.3f}, {7,0.3f}, {7,0.6f},

        // Down came the rain and washed the spider out
        {4,0.3f}, {4,0.3f}, {4,0.6f},
        {7,0.3f}, {7,0.3f}, {7,0.6f},
        {4,0.3f}, {2,0.3f}, {0,0.9f}
    };

}