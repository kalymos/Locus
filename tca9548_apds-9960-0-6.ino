

#include <Wire.h>

#include <SparkFun_I2C_Mux_Arduino_Library.h> //Click here to get the library: http://librarymanager/All#SparkFun_I2C_Mux
QWIICMUX myMux;

#include <SparkFun_APDS9960.h>

// Global Variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint16_t ambient_light = 0;
uint16_t red_light = 0;
uint16_t green_light = 0;
uint16_t blue_light = 0;

int pinCal = 14;
int pinLED = 15;
int pinRelai1 = 16;
int pinRelai2 = 17;
bool cal = LOW;


float a = 0;
float r = 0;
float g = 0;
float b = 0;
float adr = 0;
float adg = 0;
float adb = 0;

float RMin [8] [3]= {
                  {1.15, 1.80, 2.65 },  //Port0
                  {2.02, 2.82, 2.97,},  //Port1
                  {1.81, 2.70, 3.25 },  //Port2
                  {2.10, 3.62, 4.10 },  //Port3
                  {2.30, 2.96, 3.22 },  //Port4
                  {2.27, 3.51, 3.41 },  //Port5
                  {1.94, 3.07, 3.10 },  //Port6
                  {2.02, 3.13, 3.77 }   //Port7
                  };
float RMax [8] [3]= {
                  {2.26, 2.94, 2.69 },  //Port0
                  {2.17, 2.95, 3.07 },  //Port1
                  {1.92, 2.82, 3.34 },  //Port2
                  {2.20, 3.77, 4.22 },  //Port3
                  {2.45, 3.10,  3.32},  //Port4
                  {2.36, 3.59, 3.50 },  //Port5
                  {2.07, 3.20, 3.20 },  //Port6
                  {2.19, 3.32, 3.93 }   //Port7
                  };
float BMin [8] [3]= {
                  {5.90, 2.31, 1.77 },  //Port0
                  {5.48, 2.31, 1.88 },  //Port1
                  {3.30, 2.30, 2.18 },  //Port2
                  {5.87, 2.90, 2.23 },  //Port3
                  {5.80, 2.53, 2.01 },  //Port4
                  {6.79, 2.96, 1.94 },  //Port5
                  {4.89, 2.52, 1.91 },  //Port6
                  {5.91, 2.54, 2.13 }   //Port7
                  };
float BMax [8] [3]= {
                  {6.34, 2.42, 1.89 },  //Port0
                  {5.65, 2.38, 1.96 },  //Port1
                  {3.41, 2.38, 2.26 },  //Port2
                  {6.15, 3.00, 2.34 },  //Port3
                  {6.05, 2.62, 2.10 },  //Port4
                  {7.18, 3.05, 2.04 },  //Port5
                  {5.06, 2.60, 2.00 },  //Port6
                  {6.32, 2.64, 2.27 }   //Port7
                  };

bool ope = LOW;
int testR = 0;



void setup()
{
  Serial.begin(115200);

  pinMode(pinCal, INPUT);
  pinMode(pinLED, OUTPUT); //  LED
  pinMode(pinRelai1, OUTPUT); // relai 
  pinMode(pinRelai2, OUTPUT);
  Serial.println();
  Serial.println("Qwiic Mux Shield Read");

  Wire.begin();


  if (myMux.begin() == false)
  {
    Serial.println("Mux not detected. Freezing...");
    while (1)
      ;
  }
  Serial.println("Mux detected");

    
   digitalWrite(pinLED, HIGH);   // turn the LED on 
   
    for (int port=0; port<8; port++) {
      
        digitalWrite(pinLED, HIGH);   // turn the LED on (HIGH is the voltage level)
         myMux.setPort(port); //Connect master to port labeled 'port' on the mux
        Serial.println();
  
        byte currentPortNumber = myMux.getPort();
        Serial.print("CurrentPort: ");
        Serial.println(currentPortNumber);
      
        Serial.println("Begin scanning for I2C devices");
  
        
  
        byte nDevices = 0;
        for (byte address = 1; address < 127; address++)
        {
          Wire.beginTransmission(address);
          byte error = Wire.endTransmission();
      
          if (error == 0)
          {
            Serial.print("I2C device found at address 0x");
            if (address < 0x10)
              Serial.print("0");
            Serial.print(address, HEX);
            Serial.println();
      
            nDevices++;
          }
          else if (error == 4)
          {
            Serial.print("Unknown error at address 0x");
            if (address < 0x10)
              Serial.print("0");
            Serial.println(address, HEX);
          }
        }
      
        if (nDevices == 0)
          Serial.println("No I2C devices found");
        else
          Serial.println("Done");
  
  
         
        // Initialize APDS-9960 (configure I2C and initial values)
        if ( apds.init() ) {
          Serial.println(F("APDS-9960 initialization complete"));
        } else {
          Serial.println(F("Something went wrong during APDS-9960 init!"));
        }
        
        // Start running the APDS-9960 light sensor (no interrupts)
        if ( apds.enableLightSensor(false) ) {
          Serial.println(F("Light sensor is now running"));
        } else {
          Serial.println(F("Something went wrong during light sensor init!"));
        }
        delay(500);
       if (  !apds.readAmbientLight(ambient_light) ||
            !apds.readRedLight(red_light) ||
            !apds.readGreenLight(green_light) ||
            !apds.readBlueLight(blue_light) ) {
        Serial.println("Error reading light values");
        } 
        else {
          Serial.print("Ambient: ");
          Serial.print(ambient_light);
          Serial.print(" Red: ");
          Serial.print(red_light);
          Serial.print(" Green: ");
          Serial.print(green_light);
          Serial.print(" Blue: ");
          Serial.print(blue_light);
          
        }
        delay(100);
    }
    digitalWrite(pinLED, LOW);    // turn the LED off by making the voltage

}

void loop() {
  
  cal = digitalRead (pinCal);     //calibration

//**************************fonction de calibration*********************************************** 
  if (cal == HIGH ){
    digitalWrite(pinLED, HIGH);    // turn the LED on
    delay(200);
    Serial.println();
    for (int port=0; port<8; port++) {
      
         myMux.setPort(port); //Connect master to port labeled 'port' on the mux
        byte currentPortNumber = myMux.getPort();
        
        Serial.print(":Port");
        Serial.print(currentPortNumber);
        Serial.print(":");
     if (  !apds.readAmbientLight(ambient_light) ||
          !apds.readRedLight(red_light) ||
          !apds.readGreenLight(green_light) ||
          !apds.readBlueLight(blue_light) ) {
      Serial.println("Error reading light values");
      } 
      else {

        //Serial.print(":sum a/r,a/g,a/b: ");
         a = ambient_light;
         r = red_light;
         g = green_light;
         b = blue_light;
        adr = a/r;
        Serial.print(adr);
         Serial.print(" ");
        adg = a/g;
        Serial.print(adg);
         Serial.print(" ");
        adb = a/b;
        Serial.print(adb);
      }
      
      // Wait 1 second before next reading
      delay(100);
    }
    digitalWrite(pinLED, LOW);    // turn the LED off 
    delay(2000);
  }
//*********************fin de la fonction de calibration*******************************************


//***********************test luminosité**************************************************
  myMux.setPort(0);  
  delay(500);
  byte currentPortNumber = myMux.getPort();
  
  if (  !apds.readAmbientLight(ambient_light) ) {
      Serial.println("Error reading light values");
  }
  
  else {
    Serial.println("");
    Serial.print("CurrentPort: ");
    Serial.print(currentPortNumber);
    Serial.print(" ambient_light:");
    Serial.println(ambient_light);
  }

  if (ambient_light > 10){
    ope = HIGH;
  }


//********************************fonction de reconnaissance***********************************

  if (ambient_light < 10 && ope == HIGH && cal == LOW){
    
  
    digitalWrite(pinLED, HIGH);    // turn the LED on
    delay(200);
    Serial.println();
    for (int port=0; port<8; port++) {
      
         myMux.setPort(port); //Connect master to port labeled 'port' on the mux
        byte currentPortNumber = myMux.getPort();
        
        Serial.print(":Port");
        Serial.print(currentPortNumber);
        Serial.print(":");
     if (  !apds.readAmbientLight(ambient_light) ||
          !apds.readRedLight(red_light) ||
          !apds.readGreenLight(green_light) ||
          !apds.readBlueLight(blue_light) ) {
      Serial.println("Error reading light values");
      } 
      else {
        a = ambient_light;
        r = red_light;
        g = green_light;
        b = blue_light;
        adr = a/r;
        Serial.print(adr);
        Serial.print(" ");
        adg = a/g;
        Serial.print(adg);
        Serial.print(" ");
        adb = a/b;
        Serial.print(adb);

//*********************************************comparaison avec tableau**********************************

        if (adr <= RMax [port] [0] && adr >= RMin [port] [0] && adg <= RMax [port] [1] && adg >= RMin [port] [1] && adb <= RMax [port] [2] && adb >= RMin [port] [2] ){
          Serial.print("OK.R...............");
        }
        if (adr <= BMax [port] [0] && adr >= BMin [port] [0] && adg <= BMax [port] [1] && adg >= BMin [port] [1] && adb <= BMax [port] [2] && adb >= BMin [port] [2] ){
          Serial.print("OK.B...............");
        }
        Serial.println("");
      }
      delay(200);
    }
    digitalWrite(pinLED, LOW);
  }
  
}
