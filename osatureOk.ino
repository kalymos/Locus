//initialisation lcd-------------------------------------------------------------

#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 50 is connected to the DataIn
 pin 52 is connected to the CLK
 pin 53 is connected to LOAD
 We have only a single MAX72XX.
 */
LedControl lc = LedControl(50, 52, 53, 1);

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
    {'1','2','3','3'},
    {'4','5','6','6'},
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

//-----------------------------------------------------------------------------------------------------------

//initialize nixie tub

int brightness = 0;    // not rectifed bright the  nixie 
unsigned long tim =0;   //timer
int bright=0;  // how bright the  nixie is
int flag=0; //step increment
int miniNixie=0; //mini bright of nixie
//-------------------------------------------------------------------------------------------------------

void setup(){
 Serial.begin(9600); // this is added for debugging - allows you to echo the keys to the computer

 pinMode(29, INPUT); //on off               
 digitalWrite(29,HIGH); // active pull-upp

 pinMode(44,OUTPUT); //nixie tub
 digitalWrite(44,LOW); //nixie

 pinMode(30,OUTPUT); //SOLENOID  relay
 pinMode(31,OUTPUT); //SOLENOID
 pinMode(32,OUTPUT); //SOLENOID
 pinMode(33,OUTPUT); //SOLENOID
 pinMode(34,OUTPUT); //SOLENOID
 pinMode(35,OUTPUT); //SOLENOID
 pinMode(36,OUTPUT); //SOLENOID
 pinMode(37,OUTPUT); //SOLENOID
 digitalWrite(30,HIGH); // SOLENOID OFF
 digitalWrite(31,HIGH); // SOLENOID OFF
 digitalWrite(32,HIGH); // SOLENOID OFF
 digitalWrite(33,HIGH); // SOLENOID OFF
 digitalWrite(34,HIGH); // SOLENOID OFF
 digitalWrite(35,HIGH); // SOLENOID OFF
 digitalWrite(36,HIGH); // SOLENOID OFF
 digitalWrite(37,HIGH); // SOLENOID OFF
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

  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0, false);
  // Définir la luminosité à une valeur moyenne
  lc.setIntensity(0, 15);
  // effacer l'affichage
  lc.clearDisplay(0);
  // Afficher 00:00
  lc.setChar(0, 0, ' ', false);
  lc.setChar(0, 1, ' ', false);
  lc.setChar(0, 4, ' ', false);
  lc.setChar(0, 2, ' ', false);
  lc.setChar(0, 3, ' ', false);
//-------------------------------------------------


//-------------------------------


}

void loop(){
  
 if(digitalRead(29)==LOW)  //on-off
  {
    if (tim <= millis()) //nixie routin
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

//-------------------------------------------------------------------------------------
    if(mp==true)
    {
        mp3_play (1);
        mp=false;
    }
    
//---------------------------------------------------------------------------------
    
 
  if(cncAxisX==true)    //rotation serpentin
  {
    digitalWrite(8,LOW); // Set Enable low
    digitalWrite(5,LOW ); // cnc direction y-axis
    digitalWrite(2,HIGH); // Output high
    delay(2); // Wait
    digitalWrite(2,LOW); // Output low
    delay(2); // Wait


  }
  if(cncAxisY==true)    //elevation plateau
  {
    if(digitalRead(38)==LOW){
    //for(int inc=0; inc<2001; inc++){
     // digitalWrite(8,LOW); // Set Enable low
      digitalWrite(6,HIGH ); // cnc direction y-axis
      digitalWrite(3,HIGH); // Output high
      delay(2); // Wait
      digitalWrite(3,LOW); // Output low
      delay(2); // Wait
//      if(digitalRead(38)==LOW){
//        cncAxisY=false;
//      }
    }


  }
 //---------------------------------------------------------------------------------


 char key = customKeypad.getKey(); // get a key (if pressed)

 if (key){
 Serial.println(key); // echo to computer fo debugging

     // lcd out 

   if (keyLed == 0){
    lc.setChar(0, 0, key, false);
    
   }

   if (keyLed == 1){
    lc.setChar(0, 1, key, false);
    
   }

   if (keyLed == 2){
    lc.setChar(0, 2, key, false);
    
   }

   if (keyLed == 3){
    lc.setChar(0, 3, key, false);
    
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
       
     }

     if(codetest[0]==true) {
          stage1();
                 
      }
     if(codetest[1]==true)  {
  
         stage2();
  
       }

     if(codetest[2]==true)  {
  
         stage3();
  
       }
       
     if(codetest[3]==true)  {
  
         stage4();
  
       }

     if(codetest[4]==true)  {
  
         stage5();
  
       }

     if(codetest[5]==true)  {
  
         stage6();
  
       }

     if(codetest[6]==true)  {
  
         stage7();
  
       }

     if(codetest[7]==true)  {
  
         stage8();
  
       }
//       
//     if(codetest[8]==true)  {
//  
//         stage9();
//  
//       }


 

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

//void stage1 () {
//  // enclenchement de l'alim principale allumage 2 nixi vu-metre (pwm44)
//  
//
//        
//
//          incremenEtReset();
//
// 
//  //       mp3_play (1);
// //     Serial.println(entryCdePost[0]);
//        Serial.println("s1");
//        
//
//}

void stage1 () {
  //mise sous tensin du ionisateur allumage 4 lampe (30), et serpentin (pwm45) rotation serpentin (cncX).
  
//        codetest[0]=false; ///??
        
        digitalWrite(30,LOW); //relay 1 serpentin 

        
        cncAxisX=true;

          incremenEtReset();
      
         miniNixie=30;
         mp3_play (2);
  //    Serial.println(entryCdePost[0]);
        Serial.println("s2");
}

void stage2 () {
  //enrichissement de l'environement en oxigen, fumee dan la boite (31)

        digitalWrite(31,LOW); //relay 2 fumee


          incremenEtReset();

 //     Serial.println(entryCdePost[0]);
        Serial.println("s3");
        miniNixie=30;
        mp3_play (3);
}

void stage3 (){
  //allumage du condosateur a particules, allumage sphere a plasma (32), 

        digitalWrite(32,LOW); // relay 3 plasma
         //rotation serpentin 

          incremenEtReset();

         mp3_play (4);
  //    Serial.println(entryCdePost[0]);
        Serial.println("s4");
}

void stage4 () {
  // condensation de l'oxigene en ozone! aspiraton(33).

        digitalWrite(33,LOW); //relay 4 ventilo


          incremenEtReset();


         mp3_play (5);
 //     Serial.println(entryCdePost[0]);
        Serial.println("s5");
}

void stage5 () {
  //uverture de la zone de chargement(34), ouverture trappe.

        digitalWrite(34,LOW); //relay 5 trappe 1 


          incremenEtReset();


  

         mp3_play (6);
  //    Serial.println(entryCdePost[0]);
        Serial.println("s6");
}

void stage6 (){
  //ouverture de la trappe a magnetite(35), ouverture de la trappe pour la poudre de fer.

        digitalWrite(35,LOW); //relay 6 trappe 2


          incremenEtReset();
  

         mp3_play (7);
}

void stage7 () {
  //enclenchement du corp de chauffe, lumier rouge sous la plaque de verre(36), elevation aimanter++(cncY).

        digitalWrite(36,LOW); //relay 7 trappe 2
        

          incremenEtReset();

          cncAxisY=true;

         mp3_play (8);
}

void stage8 () {
  //activatin du rayon ionisateur(37),

        digitalWrite(37,LOW); //relay 8 trappe 2


          incremenEtReset();

          mp3_play (9);
}


void incremenEtReset() {
  


      codetest[codetestI]=false; // reset flag
      codetestI++; // increme global
      
       inputB[0]=' '; // reset input buffer
       inputB[1]=' ';
       inputB[2]=' ';
       inputB[3]=' ';
       
      lc.setChar(0, 0, ' ', false); //reset led
      lc.setChar(0, 1, ' ', false);
      lc.setChar(0, 4, ' ', false);
      lc.setChar(0, 2, ' ', false);
      lc.setChar(0, 3, ' ', false);
      keyLed = 0;

}

