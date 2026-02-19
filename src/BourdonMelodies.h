#pragma once
#include <vector>
#include <string>

struct MelodyStep {
    int noteIndex;      // 0..11 (or -1 for rest)
    float durationSec;  // seconds
};

namespace BourdonMelodies {

    static std::vector<MelodyStep> happyAndYouKnowIt = {
        {0, 0.4f}, {0, 0.4f}, {2, 0.4f}, {0, 0.4f},
        {4, 0.4f}, {4, 0.8f},
        {-1, 0.4f}, // rest
    };

}