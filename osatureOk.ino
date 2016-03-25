//initialisation lcd-------------------------------------------------------------

//#include "LedControl.h"
//
///*
// Now we need a LedControl to work with.
// ***** These pin numbers will probably not work with your hardware *****
// pin 50 is connected to the DataIn
// pin 52 is connected to the CLK
// pin 53 is connected to LOAD
// We have only a single MAX72XX.
// */
//LedControl lc = LedControl(50, 52, 53, 1);

// Enable one of these two #includes and comment out the other.
// Conditional #include doesn't work due to Arduino IDE shenanigans.
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
//#include <TinyWireM.h> // Enable this line if using Adafruit Trinket, Gemma, etc.

#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_7segment matrix = Adafruit_7segment();


int keyLed = 0;

//initialisation du clavier ----------------------------------------------------------------------↓

#include <Keypad.h>      

int postKey = 0;
const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {22, 24, 26, 28}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {23, 25, 27}; //connect to the column pinouts of the keypad
//-------------------------------------------------------------------------------------------------↑̛

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

boolean codetest[9]={false,false,false,false,false,false,false,false,false}; // flag determining the state of the lock
boolean isValid=false; // flag determining the validity of an input
char entryCode[9][4]={    // The code you need 
    {'1','1','1','1'},
    {'2','2','2','2'},
    {'7','8','9','9'},
    {'3','3','2','1'},
    {'6','6','5','4'},
    {'9','9','8','7'},
    {'2','5','8','0'},
    {'3','6','9','#'},
    {'#','9','6','3'},
};

char inputB[4]={' ',' ',' ',' '}; // the keypad input buffer
int incremenKeys =0; // incemen keys
//-------------------------------------------------------------------------------------------------

//initialize global increment

int codetestI=0; //incremen global
//-----------------------------------------------------------------------------------------------------------

//initialize mp3 DFPlayer Mini

#include <SoftwareSerial.h>
#include "DFPlayer_Mini_Mp3.h"

//----------------------------------------------------------------------------------------------------------

boolean mp=true; //mp3 frist plai

//------------------------------------------------------------------------------------------------------------

//initialize cnc ax

boolean cncAxisX=false;
boolean cncAxisY=false;
boolean cncAxisY2=false;
int cncAxisY2F=0;
//-----------------------------------------------------------------------------------------------------------

//initialize nixie tub

int brightness = 0;    // not rectifed bright the  nixie 
unsigned long tim =0;   //timer
int bright=0;  // how bright the  nixie is
int flag=0; //step increment
int miniNixie=0; //mini bright of nixie
boolean nixieFlag=true;
boolean nixieFlag2=false;
//-------------------------------------------------------------------------------------------------------

//initalize led cort de chaufe

int brightLed = 0;  // how bright the LED is
int fadeLed = 5;    // how many points to fade the LED by
unsigned long timLed =0;   //timer
boolean ledCort=false;

//-------------------------------------------------------------------------------------------------------

unsigned long timFumee =0;   //timer fumee
boolean fumee=false;

//-------------------------------------------------------------------------------------------------------

unsigned long timVentilo =0;   //timer ventilo
boolean ventilo=false;

//---------------------------------------------------------------------------------------------------------

unsigned long timerLock1=0;   //timer lock 1
boolean lock1=false;

//---------------------------------------------------------------------------------------------------------

unsigned long timerLock2=0;   //timer lock 2
boolean lock2=false;

//------------------------------------------------------------------------------------------------------

unsigned long timerLock3=0;   //timer lock 3
boolean lock3=false;

//------------------------------------------------------------------------------------------------------

unsigned long timerAlarme=0;   //timer alarme
boolean alarme=false;

//------------------------------------------------------------------------------------------------------
boolean laserDouble=false;
boolean laserDoubleStart=true;

//-------------------------------------------------------------------------------------------------------
//step serpentin

#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

Stepper myStepper(stepsPerRevolution, 14, 15, 16, 17);

//-------------------------------------------------------------------------------------------
void setup(){
 Serial.begin(9600); // this is added for debugging - allows you to echo the keys to the computer

 pinMode(29, INPUT); //on off               
 digitalWrite(29,HIGH); // active pull-upp

 pinMode(44,OUTPUT); //nixie tub
 digitalWrite(44,LOW); //nixie

 pinMode(45, OUTPUT); //led cort de chofe
 digitalWrite(44,LOW);
 

 pinMode(30,OUTPUT); //relay serpentin
 pinMode(31,OUTPUT); //relay fumer
 pinMode(32,OUTPUT); //relay plasme
 pinMode(33,OUTPUT); //relay ventilo
 pinMode(34,OUTPUT); //SOLENOID
 pinMode(35,OUTPUT); //SOLENOID
 pinMode(36,OUTPUT); //SOLENOID
 pinMode(37,OUTPUT); //SOLENOID
 pinMode(39,OUTPUT);
 pinMode(40,OUTPUT);
 pinMode(41,OUTPUT);
 pinMode(42,OUTPUT);
 pinMode(43,OUTPUT);
 digitalWrite(30,HIGH); // SOLENOID OFF
 digitalWrite(31,HIGH); // SOLENOID OFF
 digitalWrite(32,HIGH); // SOLENOID OFF
 digitalWrite(33,HIGH); // SOLENOID OFF
 digitalWrite(34,HIGH); // SOLENOID OFF
 digitalWrite(35,HIGH); // SOLENOID OFF
 digitalWrite(36,HIGH); // SOLENOID OFF
 digitalWrite(37,HIGH); // SOLENOID OFF
 digitalWrite(39,HIGH);
 digitalWrite(40,HIGH);
 digitalWrite(41,HIGH);
 digitalWrite(42,HIGH);
 digitalWrite(43,HIGH);
//----------------------------------------------------
 Serial1.begin (9600);
 mp3_set_serial (Serial1); //set Serial for DFPlayer-mini mp3 module 
 mp3_set_volume (15);
//-----------------------------------------------------
  pinMode(8,OUTPUT); // Enable cnc schide
  pinMode(2,OUTPUT); //step puls x-axis
  pinMode(3,OUTPUT); //step puls y-axis
  pinMode(5,OUTPUT); // direction x-axis
  pinMode(6,OUTPUT); // direction y-axis
  
  pinMode(38,INPUT); //suitch cnc
  digitalWrite(38,HIGH); // active pull-upp
  
//-----------------------------------------------------

//  /*
//   The MAX72XX is in power-saving mode on startup,
//   we have to do a wakeup call
//   */
//  lc.shutdown(0, false);
//  // Définir la luminosité à une valeur moyenne
//  lc.setIntensity(0, 15);
//  // effacer l'affichage
//  lc.clearDisplay(0);
//  // Afficher 00:00
//  lc.setChar(0, 0, ' ', false);
//  lc.setChar(0, 1, ' ', false);
//  lc.setChar(0, 4, ' ', false);
//  lc.setChar(0, 2, ' ', false);
//  lc.setChar(0, 3, ' ', false);

  matrix.begin(0x70);
  
  matrix.writeDigitNum(0,'.');
  matrix.writeDisplay();

//-------------------------------------------------
//setp serpenti
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);

//-------------------------------


}

void loop(){
  
 if(digitalRead(29)==LOW)  //on-off
  {
    
    if (laserDoubleStart==true)
    {
      laserDouble=true;
      laserDoubleStart=false;
    }

//------------------------------------------------------
//nixie routin pwm

    if (nixieFlag ==true){
      if (tim <= millis()) 
      {
    
        if(flag==0){
          brightness = brightness +40;
          if(brightness >= 300){
            flag=1;
          }
      
        }
          if(flag==1){
            brightness = brightness -40;
            if(brightness <= -10){
              flag=0;
            }
            
          }
        tim=millis()+300;
        
      }
        bright=brightness;
      if(bright > 255){
        bright=255;
      }
      if(bright < miniNixie){
        bright=miniNixie;
      }
      
      analogWrite(44, bright);
      Serial.println(bright);

    }
//-----------------------------------------------------------------------------------
  // pwm led

    if(ledCort == true)
    {
      if (timLed <= millis())
      {
        // set the brightness 
        analogWrite(45, brightLed);
        
        // change the brightness for next time through the loop:
        brightLed = brightLed + fadeLed;
        
        // reverse the direction of the fading at the ends of the fade:
        if (brightLed == 0 || brightLed == 255) {
        fadeLed = -fadeLed ;

        timLed = millis()+200;
        }
      }
    }

//------------------------------------------------------------------------------------
// routine funee

   if (fumee==true)
    {
      digitalWrite(31,LOW); //relay 2 fumee
      
      if(timFumee<millis()-5000){ //temp de fumer
        digitalWrite(31,HIGH); //relay 2 fumee
        fumee=false;
      }
    }

//----------------------------------------------------------------------------------------
// rourtine vabntilo

   if (ventilo==true)
    {
      digitalWrite(33,LOW); //relay 4
      
      if(timVentilo<millis()-30000){ //temp de fumer
        digitalWrite(33,HIGH); //relay 2 fumee
        ventilo=false;
      }
    }
    
//-------------------------------------------------------------------------------------
// routinr mp3

    if(mp==true)
    {
        mp3_play (1);
        mp=false;
    }

//---------------------------------------------------------------------------------
//routine lock1

    if (lock1==true)
    {
      digitalWrite(34,LOW); //relay 5 trappe 1
      
      if(timerLock1<millis()-200){ //temp 
        digitalWrite(34,HIGH); //relay 5 trappe 1
        lock1=false;
      } 
    }
    
//---------------------------------------------------------------------------------
//routine lock2

    if (lock2==true)
    {
      digitalWrite(35,LOW); //relay 6 trappe 2
      
      if(timerLock2<millis()-2000){ //temp 
        digitalWrite(35,HIGH); //relay 6 trappe 2
        lock2=false;
      } 
    }
    
//---------------------------------------------------------------------------------    
//routine lock3

    if (lock3==true)
    {
      digitalWrite(40,LOW); //relay 2 2 trappe 3
      
      if(timerLock3<millis()-200){ //temp 
        digitalWrite(40,HIGH); //relay 2 2 trappe 3
        lock3=false;
      } 
    }
    
//--------------------------------------------------------------------------------- 

//routine alarme

    if (alarme==true)
    {
      digitalWrite(40,LOW); //relay 2 2 trappe 3
      
      if(timerAlarme<millis()-30000){ //temp 
        digitalWrite(39,LOW); //relay 2,1 giro
        mp3_play (10);
        alarme=false;
      } 
    }
    
//--------------------------------------------------------------------------------- 
//routine laser double  
  
  if(laserDouble=true)
  {
    digitalWrite(37,LOW); //laser double
  }
//----------------------------------------------------------------------------------
    
 
  if(cncAxisX==true)    //rotation serpentin
  {
     myStepper.step(-stepsPerRevolution); 
    
//    digitalWrite(8,LOW); // Set Enable low
//    digitalWrite(5,HIGH ); // cnc direction y-axis
//    digitalWrite(2,HIGH); // Output high
//    delay(2); // Wait
//    digitalWrite(2,LOW); // Output low
//    delay(2); // Wait


  }

  if(digitalRead(38)==HIGH){
    cncAxisY=false;
  }
  if(cncAxisY==true)    //elevation plateau
  {

    //for(int inc=0; inc<2001; inc++){
     // digitalWrite(8,LOW); // Set Enable low
      digitalWrite(6,LOW ); // cnc direction y-axis
      digitalWrite(3,HIGH); // Output high
      delay(2); // Wait
      digitalWrite(3,LOW); // Output low
      delay(2); // Wait
   }

  if(cncAxisY2==true)    //desante plateau
  {

      cncAxisY2F++;
     // digitalWrite(8,LOW); // Set Enable low
      digitalWrite(6,HIGH ); // cnc direction y-axis
      digitalWrite(3,HIGH); // Output high
      delay(2); // Wait
      digitalWrite(3,LOW); // Output low
      delay(2); // Wait
      if(cncAxisY2F>2000)
      {
        cncAxisY2=false;
      }
   
   }
   
 //---------------------------------------------------------------------------------


 char key = customKeypad.getKey(); // get a key (if pressed)

 if (key){
 Serial.println(key); // echo to computer fo debugging

     // lcd out 

   if (keyLed == 0){
//    lc.setChar(0, 0, key, false);
        matrix.writeDigitNum(0,key);
        matrix.writeDisplay();
    
   }

   if (keyLed == 1){
   // lc.setChar(0, 1, key, false);
        matrix.writeDigitNum(1,key);
        matrix.writeDisplay();
    
   }

   if (keyLed == 2){
   // lc.setChar(0, 2, key, false);

        matrix.writeDigitNum(4,key);
        matrix.writeDisplay();    
   }

   if (keyLed == 3){
    //lc.setChar(0, 3, key, false);

        matrix.writeDigitNum(5,key);
        matrix.writeDisplay();    
   }
    


 if (key=='*') // if '*' check the last four digits are the entry code
 {
    Serial.println(inputB[0]);
    Serial.println(inputB[1]);
    Serial.println(inputB[2]);
    Serial.println(inputB[3]);





 // could do in a loop, but didn't!
    if(inputB[0]==entryCode[codetestI][0] && inputB[1]==entryCode[codetestI][1] && inputB[2]==entryCode[codetestI][2] && inputB[3]==entryCode[codetestI][3] )
    {
       codetest[codetestI]=true; // code ok - set open flag true
      Serial.println("ok");

    }
     else
     {
       codetest[codetestI]=false; // code wrong - set open flag false
       Serial.println("wrong");

//      lc.setChar(0, 0, 'f', false);
//      lc.setChar(0, 1, 'a', false);
//      lc.setChar(0, 4, ' ', false);
//      lc.setRow(0, 2, B0111110);
//      lc.setChar(0, 3, 'h', false);
        matrix.writeDigitNum(0,'.');
        matrix.writeDisplay();

      keyLed = 0;
       
     }

     if(codetest[0]==true) {
        //mise sous tensin du ionisateur allumage 4 lampe (30), et serpentin (pwm45) rotation serpentin (cncX).
        //stage1();
        digitalWrite(30,LOW); //relay 1 serpentin 
        
        
        
        cncAxisX=true;
        
        incremenEtReset();
        
        miniNixie=30;
        
        mp3_play (2);
        
        Serial.println("s2");
        
                 
      }
     if(codetest[1]==true)  {
      
        //enrichissement de l'environement en oxigen, fumee dan la boite (31)
        //stage2();

        timFumee=millis();

        fumee=true; // fumee relay 2
           
        incremenEtReset();
        
        Serial.println("s3");
        
        mp3_play (3);

        miniNixie=50;
  
       }

     if(codetest[2]==true)  {
  
        //stage3();
        //allumage du condosateur a particules, allumage sphere a plasma (32), 
        
        digitalWrite(32,LOW); // relay 3 plasma
        //rotation serpentin 
        
        incremenEtReset();
        
        mp3_play (4);
        
        Serial.println("s4");

        miniNixie=70;
  
       }
       
     if(codetest[3]==true)  {
  
         //stage4();
        // condensation de l'oxigene en ozone! aspiraton(33).
        
        timVentilo=millis();

        ventilo=true; // ventilo relay 4
        
        incremenEtReset();
        
        mp3_play (5);
        
        Serial.println("s5");

        miniNixie=100;

       }

     if(codetest[4]==true)  {
      
        //stage5();
        //uverture de la zone de chargement(34), ouverture trappe.
        
        lock1=true; //relay 5 trappe 1
        
        timerLock1=millis();
        
        incremenEtReset();
        
        mp3_play (6);
        
        Serial.println("s6");

        miniNixie=130;
  
       }

     if(codetest[5]==true)  {
  
        //stage6();
        //ouverture de la trappe a magnetite(35), ouverture de la trappe pour la poudre de fer.
        
        lock2=true; //relay 6 trappe 2
        
        timerLock2=millis();
        
        delay(10); // Wait
                
        incremenEtReset();
        
        
        mp3_play (7);

        miniNixie=160;

       }



     if(codetest[6]==true)   {
      
        //stage8();
        //enclenchement du corp de chauffe, lumier rouge sous la plaque de verre(36), 
        

        ledCort=true;
        
        incremenEtReset();
        
        mp3_play (8);

        miniNixie=190;
        
        }

     if(codetest[7]==true)  {
  
        //stage9();
        //activatin du rayon ionisateur(37),elevation aimanter++(cncY).
        
        
        incremenEtReset();
        
      //  digitalWrite(37,LOW);//relay 2,1 giro
        alarme=true; //relay 6 trappe 2
        
        timerAlarme=millis();
        
        mp3_play (9);
        
        miniNixie=220;

        cncAxisY=true;

       }
     if (codetest[8]==true) {

      //stage10
      digitalWrite(36,LOW); //relay 7 laser
      
      lock3=true; //relay 2,2 port
      
      laserDouble=false;
      digitalWrite(37,HIGH); //laser double
      
      digitalWrite(30,HIGH); //relay 1 serpentin 
      cncAxisX=false; //rotation serpentin off

      digitalWrite(32,HIGH); // relay 3 plasma
      
      digitalWrite(39,HIGH); //relay 2,1 giro

      ledCort=false;
      analogWrite(45, 0);
      
      nixieFlag=false;
      analogWrite(44, 0);

      timVentilo=millis();
      ventilo=true;
      
      timFumee=millis();
      fumee=true;
      
      cncAxisY2=true;
      
      
     }

 }
   else
   { // a key other than '*' has been pressed
     // a digit has been pressed. First shift the contents of the key buffer along...
       for (incremenKeys =0;incremenKeys <3;incremenKeys ++)
       {
         inputB[incremenKeys ]=inputB[incremenKeys +1]; // you lose the first key in the buffer doing this$
         
       }
       inputB[3]=key; // ...then add the new key at the end of the list
       keyLed++;
   }
 }
 }
}




void incremenEtReset() {
  


      codetest[codetestI]=false; // reset flag
      codetestI++; // increme global
      
       inputB[0]=' '; // reset input buffer
       inputB[1]=' ';
       inputB[2]=' ';
       inputB[3]=' ';
       
//      lc.setChar(0, 0, ' ', false); //reset led
//      lc.setChar(0, 1, ' ', false);
//      lc.setChar(0, 4, ' ', false);
//      lc.setChar(0, 2, ' ', false);
//      lc.setChar(0, 3, ' ', false);
  matrix.writeDigitNum(0,'.');
  matrix.writeDigitNum(1,'.');
  matrix.writeDigitNum(3,'.');
  matrix.writeDigitNum(4,'.');

  matrix.writeDisplay();

      keyLed = 0;

}

