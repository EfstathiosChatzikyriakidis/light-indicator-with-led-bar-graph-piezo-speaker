/*
 *  Light Indicator (LED Bar Graph & Piezo Speaker).
 *
 *  Copyright (C) 2010 Efstathios Chatzikyriakidis (stathis.chatzikyriakidis@gmail.com)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// include notes' frequencies.
#include "pitches.h"

const int piezoPin = 9;  // the pin number of the piezo speaker.
const int sensorPin = 0; // the pin number of the input sensor.

// array with the pin numbers of the leds.
const int ledsPins[] = { 2, 3, 4, 5, 6, 7 }; 

// calculate the number of the leds in the array.
const int NUM_LEDS = (int) (sizeof (ledsPins) / sizeof (const int));

// the two bounds of the input light sensor (analog input).
const int sensorMin = 0;
const int sensorMax = 1023;

// default value for leds bar graph.
int ledsRange = -1;

// threshold value for the music to play.
const int ledsThreshold = NUM_LEDS-1;

// notes of the melody.
const int notesMelody[] = {
  NOTE_G4, NOTE_C4, NOTE_G3, NOTE_G3, NOTE_C4, NOTE_G3, NOTE_C4
};

// calculate the number of the notes in the melody in the array.
const int NUM_NOTES = (int) (sizeof (notesMelody) / sizeof (const int));

// note durations: 4 = quarter note, 8 = eighth note, etc.
const int noteDurations[] = {
  4, 8, 8, 2, 4, 2, 4
};

// startup point entry (runs once).
void setup () {
  // set sensor as input.
  pinMode(sensorPin, INPUT);

  // set piezo as output.
  pinMode(piezoPin, OUTPUT);

  // set bar graph leds as output.
  for (int i = 0; i < NUM_LEDS; i++)
    pinMode(ledsPins[i], OUTPUT);
}

// loop the main sketch.
void loop () {
  // get the value from the input sensor.
  ledsRange = analogRead(sensorPin);

  // map the value for the leds bar graph.
  ledsRange = map(ledsRange, sensorMin, sensorMax, -1, NUM_LEDS-1);

  // light or dark the appropriate leds from the bar graph.
  for (int thisLed = 0; thisLed < NUM_LEDS; thisLed++) {
    if (thisLed <= ledsRange) {
      // light the led.
      digitalWrite(ledsPins[thisLed], HIGH);
    }
    else {
      // dark the led.
      digitalWrite(ledsPins[thisLed], LOW); 
    }
  }

  // if the bar graph leds is completed play a melody.
  if(ledsRange >= ledsThreshold)
    playMelody ();
}

// play a melody and return immediately.
void playMelody () {
  // iterate over the notes of the melody.
  for (int thisNote = 0; thisNote < NUM_NOTES; thisNote++) {
    // to calculate the note duration, take one second divided by the note type.
    // e.g. quarter note = 1000/4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];

    // play the tone.
    tone(piezoPin, notesMelody[thisNote], noteDuration);

    // to distinguish notes, set a minimum time between them.
    // the note's duration plus 30% seems to work well enough.
    int pauseBetweenNotes = noteDuration * 1.30;

    // delay some time.
    delay(pauseBetweenNotes);
  }
}
