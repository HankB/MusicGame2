#include <Tone.h>
#include "Button.h"

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
 
 
#include "EventFramework.h"
#include "Music.h"

//#define NO_PPROGMEM

#if defined NO_PPROGMEM
  #define PROGMEM
  #define pgm_read_word(x) (*(x))
#endif


class HeartbeatTimer:
  public efl::Timer  // periodic timer by default
{
  private:
    static const int heartbeatPin = 13;
    static const int heartbeatPeriod = 500;
    static const int onTime = 5;
    virtual bool callback(ulong late) {
      heartbeatState ^= 1;
      digitalWrite(heartbeatPin, heartbeatState);
      if( heartbeatState )
        setPeriod(onTime);
      else 
        setPeriod(heartbeatPeriod-onTime);
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


void setup() {
  Serial.begin(115200);
  speaker1.begin(11); // speaker 1 on pin 11
  hb.add();
  setupButtonHandling();
  Serial.println("setup()");

}


void loop() {

#if 0
  static int state=0;
  if ( !notePlayer.isPlaying() ) {
    delay(1000);
    efl::LL<efl::Timer>::doItems();
    if( state >= 5)
      state = 0;
    switch(state) {
#if defined ALL
    case 0:
      notePlayer.play(Up_on_the_Housetop, np);
      break;
    case 1:
      notePlayer.play(Ode_to_Joy, np);
      break;
#endif // defined ALL
    case 2:
      notePlayer.play(Jingle_Bells, np);
      break;
#if defined ALL
    case 3:
      notePlayer.play(Toms_Flourish, np);
      break;
    case 4:
      notePlayer.play(Seven_Nation_Army, np);
      break;
#endif defined ALL

    default:
      //state = 2;
	;
    }
    state++;
      
  }
#endif  

  efl::LL<efl::Timer>::doItems();
  efl::LL<efl::Digital>::doItems();

  delay(1);
}
