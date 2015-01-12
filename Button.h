#if !defined BUTTON_H
#define BUTTON_H

#include <EventFramework.h>

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

// extern MyDigital  topRightButton;
// extern MyDigital  topLeftButton;
// extern MyDigital  bottomLeftButton;
// extern MyDigital  bottomRightButton;
// 
// extern efl::LL<efl::Digital> d1, d2, d3, d4;

void setupButtonHandling(void);

#endif // !defined BUTTON_H
