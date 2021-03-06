#include "SevSeg.h"

// Constructors
SevSeg::SevSeg( byte a, byte b, byte c, byte d, byte e, byte f, byte g, boolean _commonAnode ) {
    commonAnode = _commonAnode;
    dotpoint = false;
    shiftRegister = false;

    pinArray = new byte[7];
    pinArray[0] = a;
    pinArray[1] = b;
    pinArray[2] = c;
    pinArray[3] = d;
    pinArray[4] = e;
    pinArray[5] = f;
    pinArray[6] = g;
    
    for (byte set=0; set<7; set++){
        pinMode(pinArray[set], OUTPUT);
    }
}

SevSeg::SevSeg( byte a, byte b, byte c, byte d, byte e, byte f, byte g, byte dp, boolean _commonAnode ) {
    commonAnode = _commonAnode;
    dotpoint = true;
    shiftRegister = false;

    pinArray = new byte[8];
    pinArray[0] = a;
    pinArray[1] = b;
    pinArray[2] = c;
    pinArray[3] = d;
    pinArray[4] = e;
    pinArray[5] = f;
    pinArray[6] = g;
    pinArray[7] = dp;
    
    for (byte set=0; set<8; set++){
        pinMode(pinArray[set], OUTPUT);
    }
}

SevSeg::SevSeg( byte clock, byte latch, byte data, boolean _commonAnode) {
    commonAnode = _commonAnode;
    shiftRegister = true;
    // When you use shift register, DP is true
    
    pinArray = new byte[3];
    pinArray[_CLOCK_PIN] = clock;
    pinArray[_LATCH_PIN] = latch;
    pinArray[_DATA_PIN] = data;
    
    for (byte set=0; set<3; set++){
        pinMode(pinArray[set], OUTPUT);
    }

}

// Prints the number on the display
void SevSeg::print(byte number, boolean dotpoint_on) {
	if (! shiftRegister) {
        for (byte c = 0, dataTmp = dataArray[number]; c < 7; c++, dataTmp = dataTmp >> 1) {
            digitalWrite(pinArray[c], commonAnode ? !(dataTmp & 0x01) : dataTmp & 0x01);
        }
        
        // Dotpoint PIN must be HIGH if display is COMMON CATHODE AND DOTPOINT is REQUIRED
        if (dotpoint == true)
            digitalWrite(pinArray[7], !commonAnode ^ dotpoint_on);
    }
    else {
        digitalWrite(pinArray[_LATCH_PIN], LOW);
        shiftOut(pinArray[_DATA_PIN], pinArray[_CLOCK_PIN], MSBFIRST, commonAnode ? ~( dataArray[number] | dotpoint << 7 ) : ( dataArray[number] | dotpoint << 7 ));  
        digitalWrite(pinArray[_LATCH_PIN], HIGH);
    }
}

// Clear the display
void SevSeg::clear() {
	if (! shiftRegister) {
		for (byte c = 0; c < 7; c++)
            digitalWrite(pinArray[c], commonAnode);
        
        // Dotpoint PIN must be HIGH if display is COMMON CATHODE AND DOTPOINT is REQUIRED
        if (dotpoint == true)
            digitalWrite(pinArray[7], commonAnode);
	}
	else {
		digitalWrite(pinArray[_LATCH_PIN], LOW);
        shiftOut(pinArray[_DATA_PIN], pinArray[_CLOCK_PIN], MSBFIRST, commonAnode ? 0xFF : 0x00 );  
        digitalWrite(pinArray[_LATCH_PIN], HIGH);
	}
}

// Deallocates the dynamic data
SevSeg::~SevSeg() {
    delete[] pinArray;
}
