/*
 * PretorioDivisio Paintball TIMETAG for Capture the Flag + Search and Destroy
 * (C) Maximilian Tuftner 2021 (Wirbelwind)
 * Firmware Version 2.0
 * FW2.0 uses the 16-Bit Timer1 for exactly generating 1 s. +/- (<1ms) 
   instead of delay(), millis() or micros() which would block some features and functions.
 * If you have any questions or maybe improvements please contact me on:
   Instagram: cccp__86 | Facebook: Maxi Tüftler | Snapchat: DO2MBT
 * Please feel free to use and modify this code
*/

#include <rgb_lcd.h> //The project uses the RGB LCD from Grove with I2C-Communication

rgb_lcd lcd;

//define all inputs and outputs
 
#define LED 2
#define BUZZER 3
#define MODE_BUTTON 4
#define GBA_PB 5
#define RELAY 6
#define UP_PB 7
#define IUB_PB 8

//variables for UI and loopruntime

int cMode = 0;
int bUP = 0;
char bI = 0;
char lEnd = 0;
char hStop = 0;
int gBA = 0;
int iUB = 0;
char bBreak = 0;
char bButton = 0;

//startcolor of the Grove RGB-LCD (Value range from 0-255)

unsigned char colorR = 0;
unsigned char colorG = 127;
unsigned char colorB = 127;

//variables for time and calculating

short int cSec = 0;
short int hSec = 0;
short int dispSec = 0;
short int bSec = 0;
unsigned char dispMin = 0;
unsigned char bMin = 0;
unsigned int cMingba = 0;
unsigned int cHourgba = 0;
unsigned int cSecH = 0;
unsigned int cSecgba = 0;
unsigned int cSeciub = 0;
unsigned int cMiniub = 0;
unsigned int cHouriub = 0;



void setup() {

//define I/O
  
pinMode(MODE_BUTTON, INPUT);
digitalWrite(MODE_BUTTON, HIGH);
pinMode(UP_PB, INPUT);
digitalWrite(UP_PB, HIGH);
pinMode(GBA_PB, INPUT);
digitalWrite(GBA_PB, HIGH);
pinMode(IUB_PB, INPUT);
digitalWrite(IUB_PB, HIGH);
pinMode(LED, OUTPUT);
pinMode(RELAY, OUTPUT);
digitalWrite(RELAY, LOW);

//initialize RGB LCD and set it up

lcd.begin(16, 2);
lcd.clear();
lcd.setRGB(colorR, colorG, colorB);

lcd.print("Pretorio Divisio");
lcd.setCursor(0,1);
lcd.print("  TIMETAG V2.0");
delay(3000);
lcd.clear();

// Select gamemode (ChipSelect(CS))

lcd.print("Modus waehlen    ");
lcd.setCursor(0,1);
lcd.print("Bombe/Herrschaft");
delay(3000);
cMode = digitalRead(MODE_BUTTON);

//Timer 1 (16-Bit) configuration for generating 1 Hz

noInterrupts(); // Disables temporary interrupts
TCCR1A = 0;
TCCR1B = 0;
TCNT1 = 3036; //preload value for the counter register
TCCR1B |=(1<<CS12); // Selection of the prescaler value 256
TIMSK1|= (1<< TOIE1); //Timer1 overflow interrupt enable bit
interrupts(); //enable Interrupts



Serial.begin(9600); // Start serial communication with 9600 Baud FOR DEVELOPMENT PURPOSES
 

}

ISR(TIMER1_OVF_vect) // Interrupt Service Routine (ISR) every 1 Sec. when the Timer1 overflows the ISR is executed (f = 1 Hz). 
{
  TCNT1 = 3036;
  digitalWrite(LED,(digitalRead(LED) ^ 1)); //toggle LED
  cSec--;
  cSecH ++;
  }



void loop() {



if (cMode == 0)
  {
 
   lcd.clear();
   lcd.print("Modi: Bombe");
   delay(3000);
   lcd.clear();
   lcd.print("   Bitte Zeit   ");
   lcd.setCursor(0,1);
   lcd.print("  einstellen !  ");
   cSec = 0;
   while(1)
    {

     while ( bSec == (-cSec) && (bSec != 0) && (bI == 1)  || (lEnd == 1))
    {
       colorR = 127;
       colorG = 0;
       colorB = 0;
       lcd.setRGB(colorR, colorG, colorB);
       lcd.clear();
       lcd.print("!! DETONATION !!");
       lcd.setCursor(0,1);
       lcd.print("!! DETONATION !!");
       digitalWrite(RELAY, HIGH);
       delay(50);
       lEnd = 1;
    }
      while (bSec != (-cSec) && (bSec != 0) && (bBreak == 0))
      {
        bButton = 1;
        colorR = 127;
        colorG = 0;
        colorB = 0;
        lcd.setRGB(colorR, colorG, colorB);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("DETONATION IN:");
        lcd.setCursor(2,1);
        lcd.print(":");
        dispMin = ((bSec + cSec) / 60);
        dispSec = ((bSec + cSec) % 60);
        
        if (dispMin < 10)
        {
          lcd.setCursor(0,1);
          lcd.print("0");
          lcd.setCursor(1,1);
          lcd.print(dispMin);
        }
        if (dispSec < 10)
        {
          lcd.setCursor(3,1);
          lcd.print("0");
          lcd.setCursor(4,1);
          lcd.print(dispSec);
        }
        
         if (dispMin > 9)
         {
          lcd.setCursor(0,1);
          lcd.print(dispMin);

         }

        if (dispSec > 9)
         {
          lcd.setCursor(3,1);
          lcd.print(dispSec);
         }
          delay(50);
        
        
        bBreak = digitalRead(GBA_PB);
        hSec = cSec;
      }
      colorR = 0;
      colorG = 127;
      colorB = 0;
      cSec = hSec;
      lcd.setRGB(colorR, colorG, colorB);
      bUP = digitalRead(MODE_BUTTON);
      while(bUP == 1 && bButton == 0)
      {
        delay(300);
        bI = 1;
        bMin = bMin + 1;
        bSec = bMin * 60;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Zeit einstellen:");
        lcd.setCursor(0,1);
        lcd.print(bMin);
        lcd.setCursor(2,1);
        //lcd.print(nSec);
        bUP = digitalRead(MODE_BUTTON);
        cSec = 0;
      }
      
    bBreak = !digitalRead(IUB_PB);
    if (bBreak == 1 && bButton == 1)
     {
      lcd.setCursor(0,0);
      lcd.print("   DISARMED !   ");
     }
    }
    }
  
if (cMode == 1)
    {
      lcd.clear();
      lcd.print("Modi: Herrschaft");
      delay(3000);
      lcd.clear();
      hStop = digitalRead(MODE_BUTTON);
 
        while(1) 
        {
        lcd.setCursor(0,0);
        lcd.print("GBA:");
        lcd.setCursor(0,1);
        lcd.print("IUB:");
        iUB = digitalRead(IUB_PB);
        delay(100);
        gBA = digitalRead(GBA_PB);
        delay(100);
        hStop = digitalRead(MODE_BUTTON);
        if (iUB == 1)
        {
          iUB = 1;
          cSecH = 0;
          colorR = 127;
          colorG = 0;
          colorB = 0;
          lcd.setRGB(colorR, colorG, colorB);
        }
        
        if (gBA == 1)
        {
          gBA = 1;
          cSecH = 0;
          colorR = 0;
          colorG = 0;
          colorB = 127;
          lcd.setRGB(colorR, colorG, colorB);
          }
      

        while (gBA == 1)
        {
          hStop = digitalRead(MODE_BUTTON);
          if (hStop == 1)
          {
            gBA = 0;
            iUB = 0;
            colorR = 0;
            colorG = 127;
            colorB = 127;
            lcd.setRGB(colorR, colorG, colorB);
          }
         cSecgba = cSecgba + cSecH;
          if(cSecH > 0)
          {
            cSecH = 0;
          }
          
          if (cSecgba > 59)
          {
            cSecgba = 0;
            cMingba ++;
          }
          if (cMingba > 59)
          {
            cMingba = 0;
            cHourgba ++;
          }
          if (cSecgba < 10)
          {
            lcd.setCursor(11,0);
            lcd.print("0");
            lcd.setCursor(12,0);
            lcd.print(cSecgba);
          }
          if (cMingba < 10)
          {
            lcd.setCursor(8,0);
            lcd.print("0");
            lcd.setCursor(9,0);
            lcd.print(cMingba);
          }
          if (cHourgba < 10)
          {
            lcd.setCursor(5,0);
            lcd.print("0");
            lcd.setCursor(6,0);
            lcd.print(cHourgba);
          }
          
          lcd.setCursor(5,0);
          if (cHourgba > 9)
          {
          lcd.print(cHourgba);
          }
          lcd.setCursor(7,0);
          lcd.print(":");
          lcd.setCursor(8,0);
          if (cMingba > 9)
          {
          lcd.print(cMingba);
          }
          lcd.setCursor(10,0);
          lcd.print(":");
          lcd.setCursor(11,0);
          if (cSecgba > 9)
          {
          lcd.print(cSecgba);
          }
          iUB = digitalRead(IUB_PB); 
          
          if (iUB == 1)
          {
            colorR = 200;
            colorG = 0;
            colorB = 0;
            lcd.setRGB(colorR, colorG, colorB);
            gBA = 0;
            iUB = 1;
            cSecH = 0;
          }
          }
          
          

          while (iUB == 1)
        {
          hStop = digitalRead(MODE_BUTTON);
          if (hStop == 1)
          {
            gBA = 0;
            iUB = 0;
            colorR = 0;
            colorG = 127;
            colorB = 127;
            lcd.setRGB(colorR, colorG, colorB);
          }
          cSeciub = cSeciub + cSecH;
          if(cSecH > 0)
          {
            cSecH = 0;
          }
          
          if(cSeciub > 59)
          {
            cSeciub = 0;
            cMiniub ++; 
          }
          
          if (cMiniub > 59)
          {
            cMiniub = 0;
            cHouriub ++;
          }
          
          if (cSeciub < 10)
          {
            lcd.setCursor(11,1);
            lcd.print("0");
            lcd.setCursor(12,1);
            lcd.print(cSeciub);
          }
          if (cMiniub < 10)
          {
            lcd.setCursor(8,1);
            lcd.print("0");
            lcd.setCursor(9,1);
            lcd.print(cMiniub);
          }
          if (cHouriub < 10)
          {
            lcd.setCursor(5,1);
            lcd.print("0");
            lcd.setCursor(6,1);
            lcd.print(cHouriub);
          }
 

          lcd.setCursor(5,1);
          if (cHouriub > 9)
          {
          lcd.print(cHouriub);
          }
          lcd.setCursor(7,1);
          lcd.print(":");
          lcd.setCursor(8,1);
          if (cMiniub > 9)
          {
          lcd.print(cMiniub);
          }
          lcd.setCursor(10,1);
          lcd.print(":");
          lcd.setCursor(11,1);
          if (cSeciub > 9)
          {
          lcd.print(cSeciub);
          }
          gBA = digitalRead(GBA_PB);
          if (gBA == 1)
          {
            colorR = 0;
            colorG = 0;
            colorB = 200;
            gBA = 1;
            lcd.setRGB(colorR, colorG, colorB);
            iUB = 0;
            cSecH = 0;
          }
          
  
          }
          
    }
    }
    }
    
    
