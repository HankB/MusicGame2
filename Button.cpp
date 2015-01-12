/**
 * Module to monitor the four push button inputs on the 'game console'
 * and respond when pressed. It is stateful and responds according to
 * what the user is involve at the time. There is an idle state
 * and one used to decode the initial sequence required to initiate
 * various actions. In addition there is a state for every 'application'
 * which uses the buttons.
 *
 * The button manager consists of a Digital object as well as a Timer
 * which are used to manage the actions.
 */

#include "Button.h"
#include "Music.h"

// callback for the button handler for the four (or maybe someday more) buttons that are the main UI for this
// app. The app maintains state for whatever function is presently running and pases the button press to that
// function to do with as it chooses. The app returns a value that is the nest state. State can also be changed
// by the ButtonTimer in the case of inaction or from monitoring a function for completion
bool MyDigital::callback (ulong late, efl::Digital::States newState, efl::Digital::States oldState) {
        static ulong timer=0; 
      bool sense = getSense();
      //Serial.print("Button() "); Serial.print(newState); Serial.print(" "); Serial.println(sense); 
      if( sense )
        speaker1.play(NOTE_D3, 6 ); // button click
     digitalWrite(getID()+1, sense );               // turn on corresponding LED while button is pressed.

#if 0     
     switch (buttonState) {
         case   PATTERN_SEARCH:
             buttonTimer.restart(ButtonTimer::buttonPressTimeout);
             buttonState = ButtonSeqAction::searchPattern(getID(), newState, oldState); 
             break;
         case PLAY_MUSIC:
             // someday we'll terminate the music here
             if (notePlayer.isPlaying()) {
               buttonState = PATTERN_SEARCH;
               notePlayer.stop();
             }
             break;
         case HI_LO_GAME:
             buttonState = playHiLoButton( (efl::Digital::DigitalBit)getID() );
             break;
         case BUTTON_GAME:
             break;
         default:
             break;
     }
#endif
}

// let's instantiate some button handlers
MyDigital  topRightButton(TR, 5, efl::Digital::ACT_LO, (efl::Digital::INACTIVE|efl::Digital::ACTIVE), true);
efl::LL<efl::Digital> d1(&topRightButton);

MyDigital  topLeftButton(TL, 5, efl::Digital::ACT_LO, (efl::Digital::INACTIVE|efl::Digital::ACTIVE), true);
efl::LL<efl::Digital> d2(&topLeftButton);

MyDigital  bottomLeftButton(BL, 5, efl::Digital::ACT_LO, (efl::Digital::INACTIVE|efl::Digital::ACTIVE), true);
efl::LL<efl::Digital> d3(&bottomLeftButton);

MyDigital  bottomRightButton(BR, 5, efl::Digital::ACT_LO, (efl::Digital::INACTIVE|efl::Digital::ACTIVE), true);
efl::LL<efl::Digital> d4(&bottomRightButton);

void setupButtonHandling(void)
{
  d1.add();
  d2.add();
  d3.add();
  d4.add();
}  


