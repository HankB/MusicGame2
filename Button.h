#if !defined BUTTON_H
#define BUTTON_H

#include <EventFramework.h>
#include "Music.h"

// first some defines to ID the four buttons used
#define TR efl::Digital::BIT_8
#define TL efl::Digital::BIT_6
#define BL efl::Digital::BIT_4
#define BR efl::Digital::BIT_2

class MyDigital;

extern MyDigital  topRightButton;
extern MyDigital  topLeftButton;
extern MyDigital  bottomLeftButton;
extern MyDigital  bottomRightButton;

// possible states for button processing. Each represents a particular activity 
// available with the device
enum ButtonState {
    PATTERN_SEARCH,
    PLAY_MUSIC,
    BUTTON_GAME,
    HI_LO_GAME,
};


class MyDigital:
public efl::Digital
{
private:
  
public:
  MyDigital(  DigitalBit pin, int dbnc, Polarity pol=ACT_HI, uchar interest = (INACTIVE|ACTIVE), bool pullup=false)
  : efl::Digital( pin, dbnc, pol, interest, pullup) {
    pinMode(pin+1, OUTPUT);
    //pinMode(pin, INPUT_PULLUP);
//    Serial.print(" Pin: "); Serial.print(getID()); Serial.print(" setup ");
    };
   bool callback (ulong late, efl::Digital::States newState, efl::Digital::States oldState);
};

void setupButtonHandling(void);

class ButtonTimer:
  public efl::Timer  // periodic timer by default
{
  private:
    virtual bool callback(ulong late);
  public:
    ButtonTimer(int x):
      efl::Timer(10, 10) {
    };
    void restart(ulong x) { setCounter(x);};
    static const ulong buttonPressTimeout = 1000;
};


// Struct to tie a button sequence to a particular action
class ButtonSeqAction {
    const char *sequence;       // null terminated sequence of buttons
public:
    virtual ButtonState action()const = 0; // action to invoke when sequence met
    ButtonSeqAction(const char* seq):sequence(seq) {};
    static ButtonState searchPattern(int pin, efl::Digital::States newState, efl::Digital::States oldState);
};

// a ButtonSeqAction to start a particular song
class MusicSeqAction :
public ButtonSeqAction {
    const Phrase *       phrases;    // null terminated arrray of phrases
    ButtonState action() const {
        notePlayer.play(phrases, np);
        return PLAY_MUSIC;
    }
public:
    MusicSeqAction( const Phrase * p, const char* seq)
        :ButtonSeqAction( seq ),phrases(p) {}
};


extern efl::LL<efl::Timer> bt;
extern void resetButtonSequenceStates();
extern ButtonTimer     buttonTimer;

ButtonState playHiLoTimer(void);
ButtonState playHiLoButton( efl::Digital::DigitalBit button );
void startHiLo(void);


#endif // !defined BUTTON_H
