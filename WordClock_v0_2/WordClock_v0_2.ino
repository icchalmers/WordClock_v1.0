#include <Adafruit_NeoPixel.h>
#include <DS3232RTC.h>
#include <Time.h>
#include <Wire.h>
#include <Streaming.h> 

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel clockLEDs = Adafruit_NeoPixel(122, PIN, NEO_GRB + NEO_KHZ800);

uint32_t wordColor;

#define WORDON(__WORDTOTURN__) wordOn(__WORDTOTURN__, sizeof(__WORDTOTURN__));

const uint8_t IT[] = {10,11};
const uint8_t IS[] = {7,8};
const uint8_t HAPPY[] = {1,2,3,4,5};
const uint8_t MQUARTER[] = {16,17,18,19,20,21,22};
const uint8_t MTEN[] = {12,13,14};
const uint8_t MTWENTY[] = {28,29,30,31,32,33};
const uint8_t MFIVE[] = {23,24,25,26};
const uint8_t MHALF[] = {41,42,43,44};
const uint8_t MINUTES[] = {34,35,36,37,38,39,40};
const uint8_t TO[] = {54,55};
const uint8_t BIRTHDAY[] = {46,47,48,49,50,51,52,53};
const uint8_t PAST[] = {66,65,64,63};
const uint8_t TWELVE[] = {61,60,59,58,57,56};
const uint8_t ELEVEN[] = {77,76,75,74,73,72};
const uint8_t THREE[] = {71,70,69,68,67};
const uint8_t NINE[] = {88,87,86,85};
const uint8_t MUM[] = {84,83,82};
const uint8_t FIVE[] = {81,80,79,78};
const uint8_t EIGHT[] = {99,98,97,96,95};
const uint8_t ONE[] = {94,93,92};
const uint8_t TEN[] = {91,90,89};
const uint8_t TWO[] = {110,109,108};
const uint8_t SEVEN[] = {107,106,105,104,103};
const uint8_t SIX[] = {102,101,100};
const uint8_t FOUR[] = {121,120,119,118};
const uint8_t OCLOCK[] = {116,115,114,113,112,111};

void setup() {
  // RTC Setup
  Serial.begin(9600);
    setSyncProvider(RTC.get);   // the function to get the time from the RTC
    if(timeStatus() != timeSet) {
        Serial.println("Unable to sync with the RTC");
    }
    else {
        Serial.println("RTC has set the system time"); 
    }  
  clockLEDs.begin();
  clockLEDs.show(); // Initialize all pixels to 'off'
  rainbowCycle(5);
  clockLEDs.clear();
  clockLEDs.show();
}

void loop() {
  setTimeFromTerminal();
  static time_t tLast;
  time_t t = now();
  wordColor = clockLEDs.Color(255,255,255);
//    WORDON(IT);
//    WORDON(IS);
//    WORDON(MQUARTER);
//    WORDON(PAST);
//    WORDON(THREE);
//    WORDON(IT);
//    WORDON(IS);
//    WORDON(MTWENTY);
//    WORDON(MFIVE);
//    WORDON(MINUTES);
//    WORDON(PAST);
//    WORDON(TWELVE);

// Make the words rainbow coloured
//    for(uint16_t i=0; i<256; i++){
//      wordColor = Wheel(i);
//      DisplayTime();
//      delay(20);
//    }

// Make the words all on colour
    if (t != tLast) {
        tLast = t;
        DisplayTime();
    }

}

void setTimeFromTerminal(){
  time_t t;
  tmElements_t tm;
  if(Serial.findUntil("t","\n\r")){
      while(Serial.available() < 19){};
      int y = Serial.parseInt();
      tm.Year = CalendarYrToTm(y);
      tm.Month = Serial.parseInt();
      tm.Day = Serial.parseInt();
      tm.Hour = Serial.parseInt();
      tm.Minute = Serial.parseInt();
      tm.Second = Serial.parseInt();
      t = makeTime(tm);
      RTC.set(t);        //use the time_t value to ensure correct weekday is set
            setTime(t);        
            Serial << F("RTC set to: ");
            printTime(t);
            Serial << endl;
    }
}

//print time to Serial
void printTime(time_t t)
{
    printI00(hour(t), ':');
    printI00(minute(t), ':');
    printI00(second(t), ' ');
}
void printI00(int val, char delim)
{
    if (val < 10) Serial << '0';
    Serial << _DEC(val);
    if (delim > 0) Serial << delim;
    return;
}

// Turn on a word. Used with macro WORDON
void wordOn(const uint8_t *wordToSet, uint8_t length){
  uint8_t i;
    for(i=0; i<length; i++){
        clockLEDs.setPixelColor(wordToSet[i], wordColor);
    }
}
  
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=clockLEDs.numPixels(); i>0; i--) {
      clockLEDs.setPixelColor(i, c);
      clockLEDs.show();
      delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*1; j++) { // 1 cycles of all colors on wheel
    for(i=0; i< clockLEDs.numPixels(); i++) {
      clockLEDs.setPixelColor(i, Wheel(((i * 256 / clockLEDs.numPixels()) + j) & 255));
    }
    clockLEDs.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return clockLEDs.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return clockLEDs.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return clockLEDs.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}


void DisplayTime(void){

  // start by clearing the display to a known state
  clockLEDs.clear();
  int minutes = minute();
  int hours = hour();
  if(hours>12){
    hours = hours-12;
  }
  if (hours==0){
    hours = 12;
  }
  
  Serial.print("It is ");
  WORDON(IT);
  WORDON(IS);
  // now we display the appropriate minute counter
  if ((minutes>4) && (minutes<10)) { 
    WORDON(MFIVE); 
    WORDON(MINUTES); 
    Serial.print("Five Minutes ");
  } 
  if ((minutes>9) && (minutes<15)) { 
    WORDON(MTEN); 
    WORDON(MINUTES); 
    Serial.print("Ten Minutes ");
  }
  if ((minutes>14) && (minutes<20)) {
    WORDON(MQUARTER); 
      Serial.print("Quarter ");
  }
  if ((minutes>19) && (minutes<25)) { 
    WORDON(MTWENTY); 
    WORDON(MINUTES); 
    Serial.print("Twenty Minutes ");
  }
  if ((minutes>24) && (minutes<30)) { 
    WORDON(MTWENTY); 
    WORDON(MFIVE); 
    WORDON(MINUTES);
    Serial.print("Twenty Five Minutes ");
  }  
  if ((minutes>29) && (minutes<35)) {
    WORDON(MHALF);
    Serial.print("Half ");
  }
  if ((minutes>34) && (minutes<40)) { 
    WORDON(MTWENTY); 
    WORDON(MFIVE); 
    WORDON(MINUTES);
    Serial.print("Twenty Five Minutes ");
  }  
  if ((minutes>39) && (minutes<45)) { 
    WORDON(MTWENTY); 
    WORDON(MINUTES); 
    Serial.print("Twenty Minutes ");
  }
  if ((minutes>44) && (minutes<50)) {
    WORDON(MQUARTER); 
    Serial.print("Quarter ");
  }
  if ((minutes>49) && (minutes<55)) { 
    WORDON(MTEN); 
    WORDON(MINUTES); 
    Serial.print("Ten Minutes ");
  } 
  if (minutes>54) { 
    WORDON(MFIVE); 
    WORDON(MINUTES); 
    Serial.print("Five Minutes ");
  }

  if ((minutes <5))
  {
    switch (hours) {
    case 1: 
      WORDON(ONE); 
      Serial.print("One ");
      break;
    case 2: 
      WORDON(TWO); 
      Serial.print("Two ");
      break;
    case 3: 
      WORDON(THREE); 
      Serial.print("Three ");
      break;
    case 4: 
      WORDON(FOUR); 
      Serial.print("Four ");
      break;
    case 5: 
      WORDON(FIVE); 
      Serial.print("Five ");
      break;
    case 6: 
      WORDON(SIX); 
      Serial.print("Six ");
      break;
    case 7: 
      WORDON(SEVEN); 
      Serial.print("Seven ");
      break;
    case 8: 
      WORDON(EIGHT); 
      Serial.print("Eight ");
      break;
    case 9: 
      WORDON(NINE); 
      Serial.print("Nine ");
      break;
    case 10: 
      WORDON(TEN); 
      Serial.print("Ten ");
      break;
    case 11: 
      WORDON(ELEVEN); 
      Serial.print("Eleven ");
      break;
    case 12: 
      WORDON(TWELVE); 
      Serial.print("Twelve ");
      break;
    }
  WORDON(OCLOCK);
  Serial.println("O'Clock");
  }
  else
    if ((minutes < 35) && (minutes >4))
    {
      WORDON(PAST);
      Serial.print("Past ");
      switch (hours) {
    case 1: 
      WORDON(ONE); 
      Serial.println("One ");
      break;
    case 2: 
      WORDON(TWO); 
      Serial.println("Two ");
      break;
    case 3: 
      WORDON(THREE); 
      Serial.println("Three ");
      break;
    case 4: 
      WORDON(FOUR); 
      Serial.println("Four ");
      break;
    case 5: 
      WORDON(FIVE); 
      Serial.println("Five ");
      break;
    case 6: 
      WORDON(SIX); 
      Serial.println("Six ");
      break;
    case 7: 
      WORDON(SEVEN); 
      Serial.println("Seven ");
      break;
    case 8: 
      WORDON(EIGHT); 
      Serial.println("Eight ");
      break;
    case 9: 
      WORDON(NINE); 
      Serial.println("Nine ");
      break;
    case 10: 
      WORDON(TEN); 
      Serial.println("Ten ");
      break;
    case 11: 
      WORDON(ELEVEN); 
      Serial.println("Eleven ");
      break;
    case 12: 
      WORDON(TWELVE); 
      Serial.println("Twelve ");
      break;
      }
    }
    else
    {
      // if we are greater than 34 minutes past the hour then display
      // the next hour, as we will be displaying a 'to' sign
      WORDON(TO);
      Serial.print("To ");
      switch (hours) {
      case 1: 
        WORDON(TWO); 
       Serial.println("Two ");
       break;
      case 2: 
        WORDON(THREE); 
      Serial.println("Three ");
        break;
      case 3: 
        WORDON(FOUR); 
      Serial.println("Four ");
        break;
      case 4: 
        WORDON(FIVE); 
      Serial.println("Five ");
        break;
      case 5: 
        WORDON(SIX); 
      Serial.println("Six ");
        break;
      case 6: 
        WORDON(SEVEN); 
      Serial.println("Seven ");
        break;
      case 7: 
        WORDON(EIGHT); 
      Serial.println("Eight ");
        break;
      case 8: 
        WORDON(NINE); 
      Serial.println("Nine ");
        break;
      case 9: 
        WORDON(TEN); 
      Serial.println("Ten ");
        break;
      case 10: 
        WORDON(ELEVEN); 
      Serial.println("Eleven ");
        break;
      case 11: 
        WORDON(TWELVE); 
      Serial.println("Twelve ");
        break;
      case 12: 
        WORDON(ONE); 
      Serial.println("One ");
        break;
      }
    }
    clockLEDs.show();
}
