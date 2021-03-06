/*
Basic RTTY Arduino audio Beacon

See README in https://github.com/lu1aat/rtty-arduino-beacon-audio for instructions.

TL;DR:

- Connect OUT_PIN to speaker "+" and speaker "-" to GND, should output RTTY audio.
- Change MESSAGE for the beacon message and SLEEP_SEC for beacon internal

*/
#define MARK 1
#define SPACE 0

// Settings
const int OUT_PIN = 13;
const int SLEEP_SEC = 60;
char MESSAGE[] = "CQ CQ CQ DE N0CALL N0CALL N0CALL";
// char MESSAGE[] = "CQ CQ CQ DE ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789";

//
// TONES, mark and space
//
const int markFreqHz = 2125;
const int shiftFreqHz = 170;
const int spaceFreqHz = markFreqHz - shiftFreqHz;
const int toneDurationMs = 22;

// Send mark/space tones to a pin
void toneSwitch(bool mark) {
  if (mark) {
    tone(OUT_PIN, markFreqHz);
  } else {
    tone(OUT_PIN, spaceFreqHz);
  }
}

//
// ASCII + custom to Baudot symbols
//
// 0-9 group is ASCII 48-57 located fom 25 to 35
// A-Z group is ASCII 65-90 located from 0 to 25
int symbolTable[][5] = {
  {1, 1, 0, 0, 0},           // 0  A
  {1, 0, 0, 1, 1},           // 1  B
  {0, 1, 1, 1, 0},          // 2  C
  {1, 0, 0, 1, 0},          // 3  D
  {1, 0, 0, 0, 0},              // 4  E
  {1, 0, 1, 1, 0},           // 5  F
  {0, 1, 0, 1, 1},           // 6  G
  {0, 0, 1, 0, 1},           // 7  H
  {0, 1, 1, 0, 0},           // 8  I
  {1, 1, 0, 1, 0},           // 9  J
  {1, 1, 1, 1, 0},           // 10 K
  {0, 1, 0, 0, 1},          // 11 L
  {0, 0, 1, 1, 1},           // 12 M
  {0, 0, 1, 1, 0},           // 13 N
  {0, 0, 0, 1, 1},           // 14 O
  {0, 1, 1, 0, 1},           // 15 P
  {1, 1, 1, 0, 1},          // 16 Q
  {0, 1, 0, 1, 0},           // 17 R
  {1, 0, 1, 0, 0},           // 18 S
  {0, 0, 0, 0, 1},           // 19 T
  {1, 1, 1, 0, 0},          // 20 U
  {0, 1, 1, 1, 1},            // 21 V
  {1, 1, 0, 0, 1},            // 22 W
  {1, 0, 1, 1, 1},            // 23 X
  {1, 0, 1, 0, 1},            // 24 Y
  {1, 0, 0, 0, 1},            // 25 Z
  {0, 1, 1, 0, 1},   // 26 0
  {1, 1, 1, 0, 1},   // 27 1
  {1, 1, 0, 0, 1},   // 28 2
  {1, 0, 0, 0, 0},   // 29 3
  {0, 1, 0, 1, 0},   // 30 4
  {0, 0, 0, 0, 1},   // 31 5
  {1, 0, 1, 0, 1},   // 32 6
  {1, 1, 1, 0, 0},   // 33 7
  {0, 1, 1, 0, 0},   // 34 8
  {0, 0, 0, 1, 1},   // 35 9
  {0, 0, 1, 0, 0},   // 36 SPACE
  {0, 0, 0, 1, 0},   // 37 CR
  {0, 1, 0, 0, 0},   // 38 LF
  {1, 1, 0, 1, 1},   // 39 NUMBERS
  {1, 1, 1, 1, 1}    // 40 LETTERS
};

void rttyTxBits(int bits[]) {
  // Start bit
  rttyStartBit();

  // 5 data bits
  for(byte i = 0; i < 5; i++) {
    int symbol = 0;
    symbol = bits[i];

    toneSwitch(symbol);
    delay(toneDurationMs);
  }

  // Stop bit
  rttyStopBit();
}

// Switch RTTY mode from numbers to letters, true for letters
void rttyMode(bool letters) {
  if(letters) {
    rttyTxBits(symbolTable[40]);
  } else {
    rttyTxBits(symbolTable[39]);
  }
}

void rttyStartBit() {
  // Start bit
  toneSwitch(SPACE);
  delay(toneDurationMs);
}

void rttyStopBit() {
  // Stop bit
  toneSwitch(MARK);
  delay(toneDurationMs * 1.5);
}

void rttyTx(char message[]) {
  // Set MARK for 200ms
  toneSwitch(MARK);
  delay(200);
  
  // Letters of numbers mode and previous mode, letters by default
  bool numbersMode = false;
  bool numbersModePrev = false;

  // Iter over message characters
  for(byte i = 0; i < strlen(message); i++) {
    // Calculate charcter position in symbol table
    int symbolIndex = 0;
    int symbolShift = 65;

    // Numbers shift
    if (message[i] < 58 && message[i] != 32) {
        numbersMode = true;
        symbolShift = 22;
        if(!numbersModePrev) {
          rttyMode(0);
        }
    } else {
        // Letters
        numbersMode = false;
        if(numbersModePrev) {
          rttyMode(1);
        }
    }
    numbersModePrev = numbersMode;

    symbolIndex = message[i] - symbolShift;

    // Check for SPACE, DEC 32 in ASCII force to "SPACE" symbol
    if(message[i] == 32) {
      symbolIndex = 36;
    }

    // TX Character
    rttyTxBits(symbolTable[symbolIndex]);
  }

  // End of message
  rttyTxBits(symbolTable[37]); // CR
  rttyTxBits(symbolTable[37]); // LF
  noTone(OUT_PIN);
}

//
// Setup
//
void setup() {
  // Set output PIN as OUTPUT 
  pinMode(OUT_PIN, OUTPUT);
}


//
// Loop
//
void loop() {
  // TX
  rttyTx(MESSAGE);
  
  // Sleep
  delay(SLEEP_SEC * 1000UL);
}
