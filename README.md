# MusicGame2

## TL;DR

Button sequences are found in `Button.cpp` starting at line 82 with `jingleBellSeq` and an additional one at 145 (`hiLoSeq`). `hiLoSeq` starts an interactive "game."

## History and motivation

This is an ancient project from a previous age. About 8 years ago I developed something on an Arduino that interfaced with buttons and matching LEDs arranged as four corners un a plastic box; (upper left, upper right, lower left and lower right.) It also drove a little speaker and when the correct button sequence was pressed, it would play a little snippet of music. (Hopefully not long enough to trigger a copyright violation, since this is now public.)

I gave the assembly programmed with this sketch to my grandson and that was the last I heard of it until he brought it over during a recent visit. He showed me how he hjad figured out a couple of the sequences. I told him there were more and he confidently explained to me that there were not. My reaction was to dig up this project to find the sequences I coded and try them out.

At that time I did not put the project up on Github but instead kept it in a local file based repo. Now it's up on Github. I have no idea if it will even build. I did have a project `MusicGame` with a note that it would not build and that I was starting from scratch and adding components until it broke and I could fix it. Apparently this built 8 years ago using the Arduino IDE. I have no idea if it will build today. I'm pretty sure it depends on another project <https://github.com/HankB/EventFramework> And I think it is just necessary to copy the header to this project directory. It looks like it also uses `A Tone Generator Library` by Brett Hagman.

## Plans

My initial plans were to simply find the coded key sequences and see if they still work. If I decide to continue working with this, I will probably migrate the build tools to PlatformIO. I also should decide on a license.

## Notes & Errata

### Tone Library

toneAC - requires 2 digital pins, hard coded and uses timer 1
toneAC2 - requires two pins, configurable pins and timer 2
tone (standard lib)

Arduino Tone Library https://code.google.com/p/rogue-code/wiki/ToneLibraryDocumentation - Three timers (two if you want to use millis()) Does PWM in S/W. (That link is now archived.)

### I/O assignments

|Pin| Name|Function|
|---|---|---|
D0|reserved
D1|reserved
D2/INT0|in|Button – Upper Left
D3|out|LED – Upper Left
D4|in|Button – Upper Right
D5|out|LED – Upper Right
D6|in|Button – Lower Left
D7|out|LED – Lower Left 
D11|out|Speaker 2 (future)
D12|	
D13|out|heartbeat
GND		
AREF		
nc		
IOREF		
RESET		
3.3V		
5V		
GND		
GND		
Vin		
A0		
A1		
A2		
A3		
A4		
A5		

 