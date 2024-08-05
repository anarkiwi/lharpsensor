#include <MIDI.h>
#include "Adafruit_VL53L0X.h"

const float MAX_RANGE_MM = 1000;
const byte MIDI_NOTE = 64;
const byte MIDI_VELOCITY = 64;
const byte MIDI_CHANNEL = 1;

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
byte note_enabled = false;

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  Serial.begin(31250);
  while (!lox.begin()) {
    delay(10);
  }

  //lox.configSensor(1); // long range
  lox.startRangeContinuous();
}

void noteon() {
  if (!note_enabled) {
    MIDI.sendNoteOn(MIDI_NOTE, MIDI_VELOCITY, MIDI_CHANNEL);
    note_enabled = true;
  }
}

void noteoff() {
  if (note_enabled) {
    MIDI.sendNoteOff(MIDI_NOTE, 0, MIDI_CHANNEL);
    note_enabled = false;
  }
}

void loop() {
  if (lox.isRangeComplete()) {
    uint16_t r = lox.readRange();
    if (r > MAX_RANGE_MM) {
      noteoff();
    } else {
      if (r < MAX_RANGE_MM) {
        noteon();
        int pb = (r / MAX_RANGE_MM * 16384) - 8192;
        MIDI.sendPitchBend(pb, MIDI_CHANNEL);
      }
    }
  }
}