#include <Keypad.h>


int postKey = 0;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8, 9}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


boolean codetest[2]={false,false}; // flag determining the state of the lock
boolean isValid=false; // flag determining the validity of an input
char entryCode[8]={'8','1','5','1','3','7','9','9'}; // The code you need 
char inputB[8]={'@','@','@','@','@','@','@','@'}; // the keypad input buffer
int i;
int codetestI=0;
int entryCdePost[8]={0,1,2,3,4,5,6,7};

// set up the device
void setup(){
 Serial.begin(9600); // this is added for debugging - allows you to echo the keys to the computer


 pinMode(10,OUTPUT); //SOLENOID

 digitalWrite(10,HIGH); // SOLENOID OFF



}

void loop(){


 char key = customKeypad.getKey(); // get a key (if pressed)

 if (key){
 Serial.println(key); // echo to computer fo debugging

 if (key=='*') // if '*' check the last four digits are the entry code
 {
    Serial.println(inputB[0]);
    Serial.println(inputB[1]);
    Serial.println(inputB[2]);
    Serial.println(inputB[4]);
    Serial.println(inputB[5]);
    Serial.println(inputB[6]);
    Serial.println(inputB[7]);

 // could do in a loop, but didn't!
    if(inputB[0]==entryCode[entryCdePost[0]] && inputB[1]==entryCode[entryCdePost[1]] && inputB[2]==entryCode[entryCdePost[2]] && inputB[3]==entryCode[entryCdePost[3]]&& inputB[4]==entryCode[entryCdePost[4]] && inputB[5]==entryCode[entryCdePost[5]]&& inputB[6]==entryCode[entryCdePost[6]]&& inputB[7]==entryCode[entryCdePost[7]])
    {
      digitalWrite(10,LOW); // SOLENOID OFF

    }
     else
     {
       codetest[codetestI]=false; // code wrong - set open flag false
       Serial.println("wrong");
       
     }



 }
   else
   { // a key other than '*' has been pressed
     // a digit has been pressed. First shift the contents of the key buffer along...
       for (i=0;i<7;i++)
       {
         inputB[i]=inputB[i+1]; // you lose the first key in the buffer doing this
       }
       inputB[7]=key; // ...then add the new key at the end of the list
   }
 }
}

void reset(){

       inputB[0]='@'; // reset input buffer
       inputB[1]='@';
       inputB[2]='@';
       inputB[3]='@';

}
void entryCdePostUp() {
      entryCdePost[0]+=4;
      entryCdePost[1]+=4;
      entryCdePost[2]+=4;
      entryCdePost[3]+=4;
}

