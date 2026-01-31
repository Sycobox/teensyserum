# Mini-Potion: Teensy Wavetable Synthesizer

**Mini-Potion** is a high-performance, miniature digital synthesis engine running purely on Teensy hardware. Inspired by Xfer Serum, it moves beyond standard oscillators by transforming raw audio data into dynamic, playable wavetables.

---

## 📺 Video Demonstration
[![Mini-Potion Demo](https://img.youtube.com/vi/nBVYrPePfq4/0.jpg)](https://youtu.be/nBVYrPePfq4)  
*Click the image above to watch the demonstration.*

---

## 🚀 Key Features

* **Audio-to-Wavetable Transformation:** Import external audio files and split them into small sections called "frames." Each frame is used as a single-cycle waveform, utilizing the complex harmonics from the original source.
* **Linear Interpolation:** To ensure smooth, high-fidelity audio at higher frequencies, the engine uses linear interpolation. This calculates the precise sample point between indices, providing accurate pitches and preventing artifacts when the playback speed exceeds the sample rate.
* **Custom Teensy Audio Library:** This project is powered by a **custom-built Teensy Audio Library** (specifically the `wavetable_processor` class), optimized for real-time wavetable scanning and memory management.
* **Envelope & Amplitude Control:** Full control over the overall amplitude and envelope of the sound, allowing you to shape the signal's character from the initial attack to the final release.
* **Interactive Visualization:** Utilizes geometric scaling to map 16-bit amplitude values onto a 128x64 OLED display, allowing for real-time visual feedback of the current wavetable frame.



---

## 🛠 Technical Overview

### Signal Flow & Pitching
The synthesizer uses a custom `getNextSample()` function to determine the exact data point to play back based on a calculated `pitchMult`.
* **Pitch Calculation:** Playback speed is determined by calculating frequency ratios relative to A4 (440Hz) based on incoming MIDI data.
* **Wavetable Navigation:** Users can scan through different frames of the wavetable using a hardware rotary encoder, triggering an `addOffset` function to shift positions smoothly.

### Current Status
* **Voice Mode:** Currently **Monophonic**. The engine is optimized for single-note precision and MIDI-in support from DAWs.
* **Hardware:** Built using the Teensy 4.0/4.1 ecosystem, an I2C OLED display, and a rotary encoder.

---

## 🗺 Roadmap

The project is actively evolving. Upcoming milestones include:

* **Polyphony:** Refactoring voice allocation and update logic to support multi-note playback.
* **Custom FX Suite:** A dedicated signal chain featuring:
    * **Distortion:** Waveshaping and saturation algorithms.
    * **Filtering:** Multi-mode resonant filters (LPF, HPF, BPF).
    * **Compression:** Dynamics processing for signal leveling.
    * **Reverb:** Custom spatial and time-based effects.

---

## ⚡ Getting Started

1.  **Hardware:** Requires a Teensy 4.0/4.1, an I2C 128x64 OLED Display, and a Rotary Encoder.
2.  **Library:** Use the custom `wavetable_processor` files provided in this repository.
3.  **Wavetables:** Audio should be converted to compatible 16-bit integer arrays (e.g., using `wav-to-sketch`).

---

*Project by Rishi Krishnan*
