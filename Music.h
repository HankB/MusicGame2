#if !defined MUSIC_H
#define MUSIC_H
#include "pitches.h"
#include "EventFramework.h"
#include <Tone.h>

extern Tone speaker1;

#define ALL 1

typedef unsigned long ulong; // unsigned long int gets a bit tedious

typedef struct {
  int  note_;      // pitch or 0 for a rest
  int  duration_;  // duration of the note
  int getNote() const { return pgm_read_word(&note_); };
  int getDuration() const { return pgm_read_word(&duration_); };
} Note;

// a song is a sequence of phrases. Many are repeated so instead of just
// slavishly copying them, we write them once and reuse them as needed.
typedef struct {
  const Note* notes_;      // an array of notes
  int   noteCount_;  // number of notes in the array
  const PROGMEM Note * getNotePtr() const { return (const PROGMEM Note *) pgm_read_word(&notes_); }
  const int getNoteCount() const { return pgm_read_word(&noteCount_); }
} Phrase;

// Some macros to make construction of songs a little easier
#define ARRAY_COUNT(X) (sizeof(X)/sizeof(X[0]))
#define PHRASE(X) {X, ARRAY_COUNT(X)}


class NotePlayerTimer:
  public efl::Timer  // periodic timer by default
{
  private:
    virtual bool callback(ulong late); 
    int     noteIndex;      // current position in the phrase
    int     phraseIndex;    // phrase we are currently working on
    //int     phraseCount;    // how many phrases make up this song

    bool    playing;        // provide easy way to tell if we're playiing something
    Phrase const * pSong;          // pointer to the song we're playing right now.
  public:
    NotePlayerTimer(ulong c = 1):
      efl::Timer(c, 1) {  // start as tick periodic. We'll update the period based on the note duration.
    };
    void play( const Phrase *phrases, efl::LL<efl::Timer> &listElt) {
      char buff[30];
      pSong = phrases;
      //phraseCount = sizeofPhrases;
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

extern const Phrase Jingle_Bells[];
extern const Phrase Seven_Nation_Army[];
extern const Phrase Toms_Flourish[];
extern const Phrase Ode_to_Joy[];
extern const Phrase Up_on_the_Housetop[];

#endif //!defined MUSIC_H
