#include "Music.h"

#define REST 0
static const int interNoteDelay = 20;
//static const int speaker1 = 11;
static const int measure = 1000;
//static const int measure = 2400;

static const int whole = measure - interNoteDelay;
static const int half = measure / 2 - interNoteDelay;
static const int quarter = measure / 4 - interNoteDelay;
static const int quarter_triplet = (measure*2) / (3*3) - interNoteDelay;
static const int eighth = measure / 8 - interNoteDelay;
static const int sixteenth = measure / 19 - interNoteDelay;
static const int dotted_eighth = (measure * 3) / 16 - interNoteDelay;
static const int dotted_quarter = (measure * 3) / 8 - interNoteDelay;
static const int dotted_half = (measure * 3) / 4 - interNoteDelay;

Tone speaker1;

bool NotePlayerTimer::callback(ulong late)
{
      char buff[30];
      if ( pSong == 0 )    // a null pointer would be bad! (should assert())
        return false;
      if ( !playing )
        return false;       // disable the timer
      if ( pSong[phraseIndex].getNoteCount() > noteIndex ) { // still note(s) to play?
        setPeriod(pSong[phraseIndex].getNotePtr()[noteIndex].getDuration() + interNoteDelay); // timer to start the next note
	//Serial.print("play ");Serial.print(pSong[phraseIndex].notes[noteIndex].getNote());
	//Serial.print(" for ");Serial.println(pSong[phraseIndex].notes[noteIndex].getDuration());

        speaker1.play( pSong[phraseIndex].getNotePtr()[noteIndex].getNote(), pSong[phraseIndex].getNotePtr()[noteIndex].getDuration());
        noteIndex++;            // next note to play
        return true;
      }
      else {
        phraseIndex++;                          // try the next phrase
//        if ( phraseIndex < phraseCount ) {      // another phrase?
        if ( pSong[phraseIndex].getNoteCount() != 0 ) {      // another phrase?
          noteIndex = 0;                      // index the first note in the new phrase
          if ( pSong[phraseIndex].getNoteCount() > noteIndex ) { // still note(s) to play?
            setPeriod(pSong[phraseIndex].getNotePtr()[noteIndex].getDuration() + interNoteDelay); // timer to start the next note
            speaker1.play( pSong[phraseIndex].getNotePtr()[noteIndex].getNote(), pSong[phraseIndex].getNotePtr()[noteIndex].getDuration());
            noteIndex++;            // next note to play
            return true;
          }
          else { // odd situation - phrase with no notes
            playing = false;
            return 0;
          }
        }           // no more phrases
        else {
          playing = false;
          return 0;
        }

      }
      // unreachable? return true;
    };


static const Note  Jingle_Bells_melody_1[] PROGMEM = {
  { NOTE_D4, quarter }, { NOTE_B4, quarter }, { NOTE_A4, quarter }, { NOTE_G4, quarter },
  { NOTE_D4, half }, { REST, quarter }, { NOTE_D4, quarter },
  { NOTE_D4, quarter }, { NOTE_B4, quarter }, { NOTE_A4, quarter }, { NOTE_G4, quarter },
  { NOTE_E4, half }, { REST, quarter }, { NOTE_E4, quarter },
  { NOTE_E4, quarter }, { NOTE_C5, quarter }, { NOTE_B4, quarter }, { NOTE_A4, quarter },
 };

static const Note  Jingle_Bells_melody_2[] PROGMEM  = {
  { NOTE_FS4, half }, { REST, quarter }, { NOTE_FS4, quarter },
  { NOTE_D5, quarter }, { NOTE_D5, quarter }, { NOTE_C5, quarter }, { NOTE_A4, quarter },
  { NOTE_B4, half }, { REST, quarter }, { NOTE_D4, quarter },
};

static const Note  Jingle_Bells_melody_3[] PROGMEM  = {
  { NOTE_D5, quarter }, { NOTE_D5, quarter }, { NOTE_D5, quarter }, { NOTE_D5, quarter },
  { NOTE_E5, quarter }, { NOTE_D5, quarter }, { NOTE_C5, quarter }, { NOTE_A4, quarter },
  { NOTE_G4, half}, { NOTE_D5, half },
};

static const Note  Jingle_Bells_chorus[] PROGMEM  =  {
  { NOTE_B4, quarter }, { NOTE_B4, quarter }, { NOTE_B4, half },
  { NOTE_B4, quarter }, { NOTE_B4, quarter }, { NOTE_B4, half },
  { NOTE_B4, quarter }, { NOTE_D5, quarter }, { NOTE_G4, dotted_quarter }, { NOTE_A4, eighth },
  { NOTE_B4, whole },
  { NOTE_C5, quarter }, { NOTE_C5, quarter }, { NOTE_C5, quarter }, { NOTE_C5, quarter },
  { NOTE_C5, quarter }, { NOTE_B4, quarter }, { NOTE_B4, quarter }, { NOTE_B4, eighth }, { NOTE_B4, eighth },
};

static const Note  Jingle_Bells_end1[] PROGMEM =  {
  { NOTE_B4, quarter }, { NOTE_A4, quarter }, { NOTE_A4, quarter }, { NOTE_B4, quarter },
  { NOTE_A4, half }, { NOTE_D5, half },
};

static const Note  Jingle_Bells_end2[] PROGMEM =  {
  { NOTE_D5, quarter }, { NOTE_D5, quarter }, { NOTE_C5, quarter }, { NOTE_A4, quarter },
  { NOTE_G4, whole },
};

const Phrase Jingle_Bells[] PROGMEM = {
  PHRASE(Jingle_Bells_melody_1),
  PHRASE(Jingle_Bells_melody_2),
  PHRASE(Jingle_Bells_melody_1),
  PHRASE(Jingle_Bells_melody_3),
  PHRASE(Jingle_Bells_chorus),
  PHRASE(Jingle_Bells_end1),
  PHRASE(Jingle_Bells_chorus),
  PHRASE(Jingle_Bells_end2),
  { 0,0} // equivalent to a null terminator  
};

#if defined ALL
static const Note  Seven_Nation_Army_notes[] PROGMEM  = {
  { NOTE_E3, 730 },
  { NOTE_E3, 280 },
  { NOTE_G3, 310 },
  { NOTE_E3, 370 },
  { NOTE_D3, 340 },
  { NOTE_C3, 900 },
  { NOTE_B2, 835 },
  { REST,    300 },
  { NOTE_E3, 730 },
  { NOTE_E3, 310 },
  { NOTE_G3, 310 },
  { NOTE_E3, 390 },
  { NOTE_D3, 320 },
  { NOTE_C3, 320 },
  { NOTE_D3, 320 },
  { NOTE_C3, 330 },
  { NOTE_B2, 700 },
};

const Phrase Seven_Nation_Army[]  PROGMEM  = {
    PHRASE(Seven_Nation_Army_notes),
    { 0,0} // equivalent to a null terminator
};

static const Note  Toms_Flourish_Notes[]  PROGMEM = {
  { NOTE_C3, 50 }, { REST, 200 }, { NOTE_E3, 50 }, { REST, 200 }, { NOTE_G3, 50 }, { REST, 200 },
  { NOTE_C3, 50 }, { REST, 200 }, { NOTE_F3, 50 }, { REST, 200 }, { NOTE_A3, 50 }, { REST, 200 },
  { REST, 30 },
  { NOTE_C3, 75 },  { NOTE_E3, 75 }, { NOTE_G3, 75 },
  { NOTE_C3, 60 },  { NOTE_F3, 60 }, { NOTE_A3, 60 },
  { NOTE_C3, 50 },  { NOTE_E3, 50 }, { NOTE_G3, 50 },
  { NOTE_C4, 50 },  { NOTE_E4, 50 }, { NOTE_G4, 50 },
  { NOTE_C3, 50 },  { NOTE_F3, 50 }, { NOTE_A3, 50 },
  { NOTE_C4, 50 },  { NOTE_F4, 50 }, { NOTE_A4, 50 },
  { NOTE_C3, 50 },  { NOTE_E3, 50 }, { NOTE_G3, 50 },
  { NOTE_C3, 50 },  { NOTE_F3, 50 }, { NOTE_A3, 50 },
  { NOTE_C3, 40 },  { NOTE_F3, 40 }, { NOTE_A3, 40 },
  { NOTE_C4, 40 },  { NOTE_F4, 40 }, { NOTE_A4, 40 },
  { NOTE_C5, 30 },  { NOTE_F5, 30 }, { NOTE_A5, 30 },
  { NOTE_C4, 25 },  { NOTE_F4, 25 }, { NOTE_A4, 25 },
  { NOTE_C6, 20 },  { NOTE_F6, 20 }, { NOTE_A6, 20 },
  { NOTE_C7, 15 },  { NOTE_C6, 15 },  { NOTE_C5, 15 },  { NOTE_C4, 15 },  { NOTE_C3, 15 },  { NOTE_C6, 120 },
  { REST, 3*1000 },
};

const Phrase Toms_Flourish[]  PROGMEM  = {
    PHRASE(Toms_Flourish_Notes),
    { 0,0} // equivalent to a null terminator
};

#define S1(n) (n*8/6)
static const Note Ode_to_Joy_Notes_1[]  PROGMEM  = {
    { NOTE_B3, S1(quarter) }, { NOTE_B3, S1(quarter) }, { NOTE_C4, S1(quarter) }, { NOTE_D4, S1(quarter) }, 
    { NOTE_D4, S1(quarter) }, { NOTE_C4, S1(quarter) }, { NOTE_B3, S1(quarter) }, { NOTE_A3, S1(quarter) }, 
    { NOTE_G3, S1(quarter) }, { NOTE_G3, S1(quarter) }, { NOTE_A3, S1(quarter) }, { NOTE_B3, S1(quarter) }, 
};

static const Note Ode_to_Joy_Notes_2[]  PROGMEM = {
    { NOTE_B3, S1(dotted_quarter) }, { NOTE_A3, S1(eighth) }, { NOTE_A3, S1(half) }, 
};

static const Note Ode_to_Joy_Notes_3[]  PROGMEM = {
    { NOTE_A3, S1(dotted_quarter) }, { NOTE_G3, S1(eighth) }, { NOTE_G3, S1(half) }, 
};

static const Note Ode_to_Joy_Notes_4[]  PROGMEM = {
    { NOTE_A3, S1(quarter) }, { NOTE_A3, S1(quarter) }, { NOTE_B3, S1(quarter) }, { NOTE_G3, S1(quarter) }, 
    { NOTE_A3, S1(quarter) }, { NOTE_B3, S1(eighth) }, { NOTE_C4, S1(eighth) }, { NOTE_B3, S1(quarter) }, { NOTE_G3, S1(quarter) }, 
    { NOTE_A3, S1(quarter) }, { NOTE_B3, S1(eighth) }, { NOTE_C4, S1(eighth) }, { NOTE_B3, S1(quarter) }, { NOTE_A3, S1(quarter) }, 
    { NOTE_G3, S1(quarter) }, { NOTE_A3, S1(quarter) }, { NOTE_D3, S1(half) }, 
};


const Phrase Ode_to_Joy[]  PROGMEM = {
    PHRASE(Ode_to_Joy_Notes_1),
    PHRASE(Ode_to_Joy_Notes_2),
    PHRASE(Ode_to_Joy_Notes_1),
    PHRASE(Ode_to_Joy_Notes_3),
    PHRASE(Ode_to_Joy_Notes_4),
    PHRASE(Ode_to_Joy_Notes_1),
    PHRASE(Ode_to_Joy_Notes_3),
    { 0,0} // equivalent to a null terminator
};

#define S2(n) (n*5/3)
static const Note Up_on_the_Housetop_Notes_1[]  PROGMEM  = {
  { NOTE_F4, S2(quarter) },  { NOTE_F4, S2(eighth) },  { NOTE_G4, S2(eighth) },  { NOTE_F4, S2(quarter) },  { NOTE_D4, S2(quarter) },  
  { NOTE_AS3, S2(quarter) },  { NOTE_D4, S2(quarter) },  { NOTE_F4, S2(half) },  
  { NOTE_G4, S2(quarter) },  { NOTE_G4, S2(quarter) },  { NOTE_F4, S2(quarter) },  { NOTE_D4, S2(quarter) },  
  { NOTE_C4, S2(quarter) },  { NOTE_F4, S2(quarter) },  { NOTE_F4, S2(half) },  
  { NOTE_F4, S2(quarter) },  { NOTE_F4, S2(eighth) },  { NOTE_G4, S2(eighth) },  { NOTE_F4, S2(quarter) },  { NOTE_D4, S2(eighth) },  { NOTE_D4, S2(eighth) },  
  { NOTE_AS3, S2(quarter) },  { NOTE_D4, S2(quarter) },  { NOTE_F4, S2(half) },  
  { NOTE_G4, S2(quarter) },  { NOTE_G4, S2(eighth) },  { NOTE_G4, S2(eighth) },  { NOTE_F4, S2(eighth) },  { NOTE_F4, S2(eighth) },  { NOTE_D4, S2(quarter) },  
  { NOTE_C4, S2(quarter) },  { NOTE_F4, S2(quarter) },  { NOTE_AS3, S2(half) },  
  { NOTE_DS4, S2(quarter) },  { NOTE_DS4, S2(quarter) },  { NOTE_G4, S2(half) },  
  { NOTE_F4, S2(quarter) },  { NOTE_F4, S2(eighth) },  { NOTE_F4, S2(eighth) },  { NOTE_D4, S2(half) },  
  { NOTE_C4, S2(quarter) },  { NOTE_C4, S2(quarter) },  { NOTE_DS4, S2(half) },  
  { NOTE_D4, S2(quarter) },  { NOTE_F4, S2(eighth) },  { NOTE_F4, S2(eighth) },  { NOTE_AS3, S2(quarter) },  { NOTE_D4, S2(quarter) },  
  { NOTE_F4, S2(quarter) },  { NOTE_F4, S2(eighth) },  { NOTE_G4, S2(eighth) },  { NOTE_F4, S2(quarter) },  { NOTE_D4, S2(quarter) },  
  { NOTE_DS4, S2(quarter) },  { NOTE_F4, S2(quarter) },  { NOTE_G4, S2(half) },  
  { NOTE_F4, S2(quarter) },  { NOTE_F4, S2(eighth) },  { NOTE_G4, S2(eighth) },  { NOTE_F4, S2(quarter) },  { NOTE_D4, S2(eighth) },  { NOTE_D4, S2(eighth) },  
  { NOTE_C4, S2(quarter) },  { NOTE_F4, S2(quarter) },  { NOTE_AS3, S2(half) },  
};

const Phrase Up_on_the_Housetop[]  PROGMEM = {
  PHRASE(Up_on_the_Housetop_Notes_1),
  { 0,0} // equivalent to a null terminator
};

NotePlayerTimer notePlayer(0);
efl::LL<efl::Timer> np(&notePlayer);


#endif // defined ALL
