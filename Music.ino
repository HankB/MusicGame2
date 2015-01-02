/*
  was: Multiple tone player
  Now: Play some music!

 Plays a sequence of notes that make up a song

 circuit:
 * Some kind of speaker on pin 8

 created 6 December 2014
 by Hank Barta
 based on a snippet from Greg Borenstein later modified by Tom Igoe
 Original music arranged from a composition byu James Lord Pierpont.

This example code is in the public domain.

 */
#include "pitches.h"
#include "EventFramework.h"

#define REST 0
static const int interNoteDelay = 20;
static const int speaker1 = 10;
static const int measure = 800;
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

typedef unsigned long ulong; // unsigned long int gets a bit tedious

typedef struct {
  int  note;      // pitch or 0 for a rest
  int  duration;  // duration of the note
} Note;

// a song is a sequence of phrases. Many are repeated so instead of just
// slavishly copying them, we write them once and reuse them as needed.
typedef struct {
  const Note* notes;      // an array of notes
  int   noteCount;  // number of notes in the array
} Phrase;

// Some macros to make construction of songs a little easier
#define ARRAY_COUNT(X) (sizeof(X)/sizeof(X[0]))
#define PHRASE(X) {X, ARRAY_COUNT(X)}


static const Note  Jingle_Bells_melody_1[] = {
  { NOTE_D4, quarter }, { NOTE_B4, quarter }, { NOTE_A4, quarter }, { NOTE_G4, quarter },
  { NOTE_D4, half }, { REST, quarter }, { NOTE_D4, quarter },
  { NOTE_D4, quarter }, { NOTE_B4, quarter }, { NOTE_A4, quarter }, { NOTE_G4, quarter },
  { NOTE_E4, half }, { REST, quarter }, { NOTE_E4, quarter },
  { NOTE_E4, quarter }, { NOTE_C5, quarter }, { NOTE_B4, quarter }, { NOTE_A4, quarter },
};

static const Note  Jingle_Bells_melody_2[] = {
  { NOTE_FS4, half }, { REST, quarter }, { NOTE_FS4, quarter },
  { NOTE_D5, quarter }, { NOTE_D5, quarter }, { NOTE_C5, quarter }, { NOTE_A4, quarter },
  { NOTE_B4, half }, { REST, quarter }, { NOTE_D4, quarter },
};

static const Note  Jingle_Bells_melody_3[] = {
  { NOTE_D5, quarter }, { NOTE_D5, quarter }, { NOTE_D5, quarter }, { NOTE_D5, quarter },
  { NOTE_E5, quarter }, { NOTE_D5, quarter }, { NOTE_C5, quarter }, { NOTE_A4, quarter },
  { NOTE_G4, half}, { NOTE_D5, half },
};

static const Note  Jingle_Bells_chorus[] = {
  { NOTE_B4, quarter }, { NOTE_B4, quarter }, { NOTE_B4, half },
  { NOTE_B4, quarter }, { NOTE_B4, quarter }, { NOTE_B4, half },
  { NOTE_B4, quarter }, { NOTE_D5, quarter }, { NOTE_G4, dotted_quarter }, { NOTE_A4, eighth },
  { NOTE_B4, whole },
  { NOTE_C5, quarter }, { NOTE_C5, quarter }, { NOTE_C5, quarter }, { NOTE_C5, quarter },
  { NOTE_C5, quarter }, { NOTE_B4, quarter }, { NOTE_B4, quarter }, { NOTE_B4, eighth }, { NOTE_B4, eighth },
};

static const Note  Jingle_Bells_end1[] = {
  { NOTE_B4, quarter }, { NOTE_A4, quarter }, { NOTE_A4, quarter }, { NOTE_B4, quarter },
  { NOTE_A4, half }, { NOTE_D5, half },
};

static const Note  Jingle_Bells_end2[] = {
  { NOTE_D5, quarter }, { NOTE_D5, quarter }, { NOTE_C5, quarter }, { NOTE_A4, quarter },
  { NOTE_G4, whole },
};

static const Phrase Jingle_Bells[] = {
  PHRASE(Jingle_Bells_melody_1),
  PHRASE(Jingle_Bells_melody_2),
  PHRASE(Jingle_Bells_melody_1),
  PHRASE(Jingle_Bells_melody_3),
  PHRASE(Jingle_Bells_chorus),
  PHRASE(Jingle_Bells_end1),
  PHRASE(Jingle_Bells_chorus),
  PHRASE(Jingle_Bells_end2),
};

static const Note  Seven_Nation_Army_notes[] = {
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

static const Phrase Seven_Nation_Army[] = {
    PHRASE(Seven_Nation_Army_notes),
};

static const Note  Toms_Flourish_Notes[] = {
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

static const Phrase Toms_Flourish[] = {
    PHRASE(Toms_Flourish_Notes),
};


class NotePlayerTimer:
  public efl::Timer  // periodic timer by default
{
  private:
    virtual bool callback(ulong late) {
      char buff[30];
      if ( pSong == 0 )    // a null pointer would be bad! (should assert())
        return false;
      if ( !playing )
        return false;       // disable the timer
      if ( pSong[phraseIndex].noteCount > noteIndex ) { // still note(s) to play?
        setPeriod(pSong[phraseIndex].notes[noteIndex].duration + interNoteDelay); // timer to start the next note
        tone(speaker1, pSong[phraseIndex].notes[noteIndex].note, pSong[phraseIndex].notes[noteIndex].duration);
        noteIndex++;            // next note to play
        return true;
      }
      else {
        phraseIndex++;                          // try the next phrase
        if ( phraseIndex < phraseCount ) {      // another phrase?
          noteIndex = 0;                      // index the first note in the new phrase
          if ( pSong[phraseIndex].noteCount > noteIndex ) { // still note(s) to play?
            setPeriod(pSong[phraseIndex].notes[noteIndex].duration + interNoteDelay); // timer to start the next note
            tone(speaker1, pSong[phraseIndex].notes[noteIndex].note, pSong[phraseIndex].notes[noteIndex].duration);
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
    int     noteIndex;      // current position in the phrase
    int     phraseIndex;    // phrase we are currently working on
    int     phraseCount;    // how many phrases make up this song

    bool    playing;        // provide easy way to tell if we're playiing something
    Phrase const * pSong;          // pointer to the song we're playing right now.
  public:
    NotePlayerTimer(ulong c = 1):
      efl::Timer(c, 1) {  // start as tick periodic. We'll update the period based on the note duration.
    };
    void play( const Phrase *phrases, int sizeofPhrases, efl::LL<efl::Timer> &listElt) {
      char buff[30];
      pSong = phrases;
      phraseCount = sizeofPhrases;
      playing = true;
      phraseIndex = noteIndex = 0;
      listElt.add();
      return;
    }
    void stop() {
      playing = false;
    };
    bool isPlaying() {
      return playing;
    };

};


class HeartbeatTimer:
  public efl::Timer  // periodic timer by default
{
  private:
    static const int heartbeatPin = 13;
    virtual bool callback(ulong late) {
      heartbeatState ^= 1;
      digitalWrite(heartbeatPin, heartbeatState);
      return true;
    };
    bool        heartbeatState;
  public:
    HeartbeatTimer( int x = 0, int y = 0, int z = 0):
      efl::Timer(500, 250), heartbeatState(false) {
      pinMode(heartbeatPin, OUTPUT);      // should this be done in setup?
    };
};

HeartbeatTimer  heartBeat(0);          // not sure why but removing the first ctor arg causes compiler error
efl::LL<efl::Timer> hb(&heartBeat);

NotePlayerTimer notePlayer(0);
efl::LL<efl::Timer> np(&notePlayer);


void setup() {
  Serial.begin(115200);
  hb.add();
}

void loop() {
  static int state=0;
  if ( !notePlayer.isPlaying() ) {
    delay(1000);
    efl::LL<efl::Timer>::doItems();
    if( state >= 3)
      state = 0;
    switch(state) {
    case 0:
      notePlayer.play(Seven_Nation_Army, ARRAY_COUNT(Seven_Nation_Army), np);
      break;
    case 1:
      notePlayer.play(Jingle_Bells, ARRAY_COUNT(Jingle_Bells), np);
      break;
    case 2:
    notePlayer.play(Toms_Flourish, ARRAY_COUNT(Toms_Flourish), np);
      break;
    default:
      state = 0;
    }
    state++;
      
  }
  efl::LL<efl::Timer>::doItems();

  delay(1);
}
