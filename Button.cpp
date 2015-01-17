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
  


// state variable for tracking what state button processing is in.
char buttonState = PATTERN_SEARCH;

static int buttonPressCount = 0; // which potential button in the sequence are we working on?

// callback for the button handler for the four (or maybe someday more) buttons that are the main UI for this
// app. The app maintains state for whatever function is presently running and pases the button press to that
// function to do with as it chooses. The app returns a value that is the nest state. State can also be changed
// by the ButtonTimer in the case of inaction or from monitoring a function for completion
bool MyDigital::callback (ulong late, efl::Digital::States newState, efl::Digital::States oldState) {
        static ulong timer=0; 
      bool sense = getSense();
//      Serial.print(") seq:"); Serial.print(buttonPressCount); Serial.print(" Button:"); Serial.print(getID()); Serial.print(" sense:"); Serial.println(sense); 
      if( sense )
        speaker1.play(NOTE_D3, 6 ); // button click
     digitalWrite(getID()+1, sense );               // turn on corresponding LED while button is pressed.

     switch (buttonState) {
         case   PATTERN_SEARCH:
             buttonTimer.restart(ButtonTimer::buttonPressTimeout);             
             buttonState = ButtonSeqAction::searchPattern(getID(), newState, oldState); 
             break;
         case PLAY_MUSIC:
             if (notePlayer.isPlaying()) { // if playing, cancel and use this key press to styart the next search
               //buttonState = PATTERN_SEARCH;
               buttonState = ButtonSeqAction::searchPattern(getID(), newState, oldState); 
               notePlayer.stop();
             }
             break;
#if HI_LO
         case HI_LO_GAME:
             buttonState = playHiLoButton( (efl::Digital::DigitalBit)getID() );
             break;
#endif             
         case BUTTON_GAME:
             break;
         default:
             break;
     }
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
  bt.add();
  resetButtonSequenceStates();
}  

static const char jingleBellSeq[] = { BR, TL, BR, 0};// a button sequence tied to the Jingle Bells composition
const MusicSeqAction playJingleBells( Jingle_Bells, jingleBellSeq);

static const char sevenNationArmySeq[] = { TR, BL, 0 };// button sequence that will be used to some other composition
const MusicSeqAction playSevenNationArmy( Seven_Nation_Army, sevenNationArmySeq);

static const char TomsFlourishSeq[] = { TR, TL, 0 };// button sequence that will be used to some other composition
const MusicSeqAction playTomsFlourish( Toms_Flourish, TomsFlourishSeq);

static const char OdeToJoySeq[] = { TR, BR, 0 };// button sequence that will be used to some other composition
const MusicSeqAction playOdeToJoy( Ode_to_Joy, OdeToJoySeq);

static const char UpOnTheHousetopSeq[] = { BL, TR, 0 };// button sequence that will be used to some other composition
const MusicSeqAction playUpOnTheHousetop( Up_on_the_Housetop, UpOnTheHousetopSeq);

/* 
 * Timer tied to Button inputs serves timing functions for buttons such as timing out a button press and so on.
 */
bool ButtonTimer::callback(ulong late)
{
//      Serial.print("analog: ");Serial.print(sensorValue);Serial.print(" volume: "); Serial.println(vol);
     switch (buttonState) {
        case   PATTERN_SEARCH:
            if( buttonPressCount ) {
                resetButtonSequenceStates();
 //               Serial.println("reset on timeout");
            }
            break;
         case PLAY_MUSIC:
             //Serial.println("PLAY_MUSIC:");
             // every 100 ms check to see if music is still playing
             if (!notePlayer.isPlaying()) {
                     buttonState = PATTERN_SEARCH;
                     //Serial.println(F("Song done, returning to PATTERN_SEARCH"));
                     resetButtonSequenceStates();
             }
             break;
#if HI_LO
         case HI_LO_GAME:
             buttonState = playHiLoTimer();
             break;
#endif             
         case BUTTON_GAME:
             break;
         default:
             break;
     }
      return true;
};


// an array of button sequences that the pattern matcher will search
// to see if some action should be initiated.
static const ButtonSeqAction*  sequences[] = {
  &playJingleBells, 
  &playSevenNationArmy,  
#if HI_LO
  &playHiLoGame,
#endif
  &playTomsFlourish,
  &playOdeToJoy,
  &playUpOnTheHousetop,
};


// array of button sequence states. Ordinarily these would be part of the ButtonSeqAction class
// but since the RAM/ROM footprint of the micro is so small, these will be kept separate so the
// rest of the class can remain in ROM.
static bool buttonSequenceStates[ARRAY_COUNT(sequences)];
void resetButtonSequenceStates() 
{
    for( int i=0; i<ARRAY_COUNT(sequences); i++ )
        buttonSequenceStates[i] = true; // they all match before we start checking
    buttonPressCount = 0; // start over again
}
            
// Search all button sequences for a match. Searching is done on button/active transition
// along with a short beep (like a click) and actions are executed on button/inactive
ButtonState ButtonSeqAction::searchPattern(int pin, efl::Digital::States newState, efl::Digital::States oldState)
{
    if (newState == efl::Digital::INACTIVE) {// button released?
        for (int i=0; i<ARRAY_COUNT(sequences); i++) { // iterate over all sequences
            if (buttonSequenceStates[i]) { // this one still a candidate?
//                Serial.print(F("Still a candidate ")); Serial.print(buttonPressCount); Serial.print(" ");Serial.println(strlen(sequences[i]->sequence)-1);
                if (buttonPressCount==strlen(sequences[i]->sequence)-1) { // match entire string?
//                    Serial.println("Entire seq matched");
                    resetButtonSequenceStates();
                    return sequences[i]->action();
                }
            }
        }
        buttonPressCount++;
    } 
    else if (newState == efl::Digital::ACTIVE)  // active transition?
    {
        for (int i=0; i<ARRAY_COUNT(sequences); i++)  // iterate over all sequences
        {
            if (buttonSequenceStates[i] &&  buttonPressCount<strlen(sequences[i]->sequence))  // still in the running and still have buttons to test
            {
//                Serial.print(F("Still in the running"));
                if (sequences[i]->sequence[buttonPressCount]!=pin)  // not a match?
                {
//                    Serial.println(F(" no pin match"));
                    buttonSequenceStates[i] = false; // strike this one from the list
                } 
//                else 
//                {
//                    Serial.println(F(" pin match"));
//                }
            }
//            else
//            {
//                Serial.println(F("Not in the running"));
//            }
        }
    }
                    
    return PATTERN_SEARCH; // nothing (yet) on button release
}


ButtonTimer     buttonTimer(0);
efl::LL<efl::Timer> bt(&buttonTimer);

