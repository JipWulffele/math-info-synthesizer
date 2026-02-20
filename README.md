# 🎛️ The Best Synthesizer

A real-time polyphonic synthesizer built with **openFrameworks**.

This project implements:

- A custom `oscilator` class (additive synthesis)
- A main `ofApp` synthesizer controller
- Keyboard-controlled notes (12 semitones)
- Octave shifting using numpad
- Waveform blending (Sine / Square / Sawtooth / Triangle)
- Constant drone mode (Bourdon)
- Predefined Bourdon melodies
- Real-time Fourier Transform visualization
- Band-pass filter for the Bourdon

---

## 🧠 Architecture

### 1️⃣ Class `oscilator`

The `oscilator` class is responsible for generating sound.

#### Features

- Additive synthesis
- Multiple waveforms:
  - Sine
  - Square (odd harmonics)
  - Sawtooth (full harmonics)
  - Triangle (odd harmonics squared)
- Smooth frequency transitions
- Smooth waveform amplitude transitions
- Anti-aliasing via harmonic limiting
- ADSR-style envelope (Attack / Release)
- Optional band-pass filter
- Stereo output
- Real-time safe signal generation

#### Envelope

- Attack ramp
- Release ramp
- Smooth note on/off transitions

#### Methods

- `setFrequency(float f)`
- `setAmplitude(float A)`
- `setBrillance(float b)`
- `setAmpSine(float)`
- `setAmpSquare(float)`
- `setAmpSawtooth(float)`
- `setAmpTriangle(float)`
- `setNoteOn(bool)`
- `get_signal(ofSoundBuffer&, int n)`
- Filter controls:
  - `setFilterActive(bool)`
  - `setFilterBPF(freq, Q)`

---

### 2️⃣ Class `ofApp`

Main application controller.

Handles:

- Audio stream setup
- GUI
- Keyboard interaction
- Mixing multiple oscillators
- Bourdon management
- Melody playback
- Fourier Transform visualization
- Drawing piano keyboard

---

## 🎹 Keyboard Controls

### Musical Keys (12 semitones)

| Key | Note |
|------|------|
| W | C |
| S | C# |
| X | D |
| D | D# |
| C | E |
| V | F |
| G | F# |
| B | G |
| H | G# |
| N | A |
| J | A# |
| , | B |

Press → note ON  
Release → note OFF  

Polyphonic playback supported.

---

### 🔢 Octave Shift

Use number keys:

```
0–9
```

Each number shifts the octave:

```
newFreq = baseFrequency * 2^octaveShift
```

---

## 🎚️ Waveform Mixing

You can blend waveforms in real time using GUI sliders:

- Sine
- Square
- Sawtooth
- Triangle

Each waveform has independent amplitude (0–1).

This allows:
- Pure sine tones
- Classic subtractive-style square
- Bright saw synth
- Soft triangle
- Hybrid blends

---

## 🎼 Bourdon (Drone Mode)

The Bourdon is a constant sustained oscillator.

#### Controls

- Activate / deactivate (`p`)
- Frequency
- Amplitude
- Brillance (harmonics)
- Waveform mix
- Optional band-pass filter
- Mouse control (optional)

Mouse mode:
- X → Frequency
- Y → Amplitude

---

## 🎵 Predefined Bourdon Melody

You can trigger a predefined melody:

- Toggle: "Play Bourdon Melody"
- Uses `BourdonMelodies.h`
- Steps defined as:
  - Note index
  - Duration (seconds)
- Supports rests (`noteIndex = -1`)
- Loops automatically

Melody uses a dedicated oscillator.

---


## Setup
``` sh
``` sh
git clone git@github.com:JipWulffele/math-info-synthesizer.git
~/of_v0.12.1_linux64_gcc6_release/apps/projectGenerator/commandLine/bin/projectGenerator  -o"~/of_v0.12.1_linux64_gcc6_release" -a"ofxGui" -p"linux64"  -t"" "~/of_v0.12.1_linux64_gcc6_release/apps/myApps/math-info-synthesizer
``` 

addons: ofxGui


