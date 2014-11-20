//===========================================================================
//                                                                           
//  Desc:    Arduino Code for an epee fencing scoring apparatus              
//  Dev:     Keaheyp                                                            
//  Date:    Nov 2014
//  Citation:  Code and ideas pulled from wnew's epeeBox on github, thank you!!
//  Updated:                                                        
//  Notes:   State machine implementation of an epee scoring box.  
//                                                                           
//  To do:                                  
//              Test to see if adding interrupts is actually needed or if current
//              implementation is within FIE timing variances
//===========================================================================

//============
// #Variable Instantiation
//============


// Pin Configuration

//red & green target lights
const uint8_t onTargetA  = 12;        // On TargetA LED pin
const uint8_t onTargetB  = 13;        // On TargetB LED pin

//weapons
const uint8_t weaponPinA = 2;         // WeaponA pin
const uint8_t weaponPinB = 3;         // WeaponB pin
uint8_t weaponA          = 0;
uint8_t weaponB          = 0;

const uint8_t buzzerPin  = 4;

//boolean variables
boolean hitA = false;
boolean hitB = false;

//timing variables
long timeHitA     = 0;
long timeHitB     = 0;
long timeFirstHit = 0;

//Default const variables
int lockOut        = 40;    // the lockout time between hits for epee is 40ms
int minHitDuration = 4;     // the minimum amount of time the tip needs to be depressed 4ms

//state variable
int state = 1;

void setup() {
   //Setup OUTPUTS
   pinMode(onTargetA,  OUTPUT);
   pinMode(onTargetB,  OUTPUT);
   //SETUP INPUTS
   pinMode(weaponPinA, INPUT);
   pinMode(weaponPinB, INPUT);

   pinMode(buzzerPin,  OUTPUT);

   Serial.begin(9600);
   Serial.println("Epee Scoring Box Init");
   Serial.println("====================");  
}
//Main Loop, will use state system
/*
  state 1 = waiting for first hit
  state 2 = waiting for second hit
  state 3 = reset variables
*/
void loop() {
   
  
  weaponA = digitalRead(weaponPinA);
  weaponB = digitalRead(weaponPinB);
     
   //Debugging 
 /*Serial.print("Weapon A:");
   Serial.println(weaponA);
   Serial.println(weaponB);*/
   
   //waiting for first hit state
   if(state==1)
   {
     if(weaponA == 1 || weaponB == 1)
     {
       //record time of weapon hit
       timeFirstHit = millis();
       //turn buzzer on and LED for which weapon hit
       tone(buzzerPin, 1000);        
       if(weaponA==1){digitalWrite(onTargetA, HIGH);hitA=true;}
       if(weaponB==1){digitalWrite(onTargetB, HIGH);hitB=true;}
       //switch to waiting for second hit state
       state = 2;
     }
   }
   //waiting for second hit
   else if(state == 2)
   {
     //wait for second hit if still within lockout time
     if(timeFirstHit + lockOut > millis())
     {
       if(weaponA==1 && hitA == false){digitalWrite(onTargetA, HIGH);hitA=true;}
       if(weaponB==1 && hitB == false){digitalWrite(onTargetB, HIGH);hitB=true;}
     }
     else
     {
       //switch to reset state when lockout expires
       state = 3;
     }
   }
   //lockout time expired keep buzzer on for 2 seconds and lights on for 4 seconds (2 more than buzzer) then reset
   else if(state == 3)
   {
     delay(2000);
     noTone(buzzerPin);
     delay(2000);
     if(hitA == true){digitalWrite(onTargetA, LOW);}
     if(hitB == true){digitalWrite(onTargetB, LOW);}
     hitA = false;
     hitB = false;
     timeFirstHit = 0;
     state = 1;
   }
}
   
