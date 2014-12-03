/*
  was: Multiple tone player
  Now: Play some music!

 Plays multiple tones on multiple pins in sequence

 circuit:
 * 1 8-ohm speaker on digital pin 6

 created 8 March 2010
 by Tom Igoe
 based on a snippet from Greg Borenstein

This example code is in the public domain.

 http://arduino.cc/en/Tutorial/Tone4

 */
#include "pitches.h"

static const int interNoteDelay = 20;
static const int measure = 800;

static const int whole = measure-interNoteDelay;
static const int half = measure/2-interNoteDelay;
static const int quarter = measure/4-interNoteDelay;
static const int eighth = measure/8-interNoteDelay;
static const int sixteenth = measure/19-interNoteDelay;
static const int dotted_eighth = (measure*3)/16-interNoteDelay;
static const int dotted_quarter = (measure*3)/8-interNoteDelay;


typedef struct {
  int  note;      // pitch or 0 for a rest
  int  duration;  // duration of the note
} Note;

// a song is a sequence of phrases.
typedef struct {
  Note* notes;      // an array of notes
  int   noteCount;  // number of notes in the array
} Phrase;

Note  Jingle_Bells_refrain[] = {
  { NOTE_E4, quarter }, { NOTE_E4, quarter },{ NOTE_E4, half },
  { NOTE_E4, quarter }, { NOTE_E4, quarter }, { NOTE_E4, half },
  { NOTE_E4, quarter }, { NOTE_G4, quarter },{ NOTE_C4, dotted_quarter }, { NOTE_D4, eighth },
  { NOTE_E4, whole },
  { NOTE_F4, quarter }, { NOTE_F4, quarter }, { NOTE_F4, quarter }, { NOTE_F4, quarter },
  { NOTE_F4, quarter }, { NOTE_E4, quarter }, { NOTE_E4, quarter },{ NOTE_E4, eighth }, { NOTE_E4, eighth },
};

Note  Jingle_Bells_end2[] = {
  { NOTE_G4, quarter }, { NOTE_G4, quarter }, { NOTE_F4, quarter }, { NOTE_D4, quarter },
  { NOTE_C4, whole },
};
  
Note  Jingle_Bells_end1[] = {
  { NOTE_E4, quarter }, { NOTE_D4, quarter }, { NOTE_D4, quarter }, { NOTE_E4, quarter },  
  { NOTE_D4, half },{ NOTE_G4, half },
};

#define ARRAY_COUNT(X) (sizeof(X)/sizeof(X[0]))
#define PHRASE(X) {X, ARRAY_COUNT(X)}
  
Phrase  Jingle_Bells[] = {
    PHRASE(Jingle_Bells_refrain),
    PHRASE(Jingle_Bells_end1),
    PHRASE(Jingle_Bells_refrain),
    PHRASE(Jingle_Bells_end2),
};

void setup() {
}

void loop() {
  // turn off tone function for pin 6:
  //noTone(6);
  // play a note on pin 6 for 200 ms:
  for(int i=0; i<ARRAY_COUNT(Jingle_Bells); i++ ) // loop over the phrases
  {
    for( int j=0; j<Jingle_Bells[i].noteCount; j++ ) {
    tone(6, Jingle_Bells[i].notes[j].note, Jingle_Bells[i].notes[j].duration);
    delay(Jingle_Bells[i].notes[j].duration);
    //noTone(6);
    delay(interNoteDelay);
    }
//    delay(500);
  }
  delay(2000);
}
