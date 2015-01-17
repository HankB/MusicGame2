/*
 * This is going to be a simple game that varies the tone on the speaker based on button presses. Plan is to:
 *  - ramp tone up if a button is pressed (perhaps faster if different or more than one buitton is pressed.)
 *  - ramp tone down if no button is pressed, exiting the game if it hits 'bottom'
 *  - ramp down faster if other buttons are pressed.
 */

#include <Tone.h>

#include "Music.h"
#include "Button.h"

static uint pitch;
static const uint pitchIncrement = 8;           // how much we'll vary pitch at button press/held or decay
static const ulong timingPeriod = 50;

static const uint maxPitch = NOTE_G6-2*pitchIncrement;
static const uint minPitch = NOTE_B1+2*pitchIncrement;

static const ulong timeout = 10000/timingPeriod; // 10K msec

// perform any initialization for this game
void startHiLo(void)
{
    //setButtonProcessingState(HI_LO_GAME);
    buttonTimer.setPeriod(timingPeriod);
    buttonTimer.setCounter(timingPeriod);
    pitch = NOTE_A4;
}

// respond to timer events in the game
ButtonState playHiLoTimer(void)
{
    uint pitchStep = topLeftButton.getSense()?(3*pitchIncrement):pitchIncrement;

    if( topRightButton.getSense()  // button pressed?
        &&  pitch < (maxPitch) )
            pitch += pitchStep;
    else if ( !topRightButton.getSense() && pitch > minPitch )
        pitch -= pitchStep;
    else if( !topRightButton.getSense() )
        return PATTERN_SEARCH;

    speaker1.play( pitch, timingPeriod+2 );
    return HI_LO_GAME;
}

// respond to button press in the game
ButtonState playHiLoButton( efl::Digital::DigitalBit button )
{
    return HI_LO_GAME;
}

