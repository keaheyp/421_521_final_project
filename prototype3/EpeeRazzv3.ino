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
boolean firstHitA = false;
boolean firstHitB = false;
boolean i = true;
boolean y = true;
int a,b =0;
//timing variables
long timeHitA     = 0;
long timeHitB     = 0;
long timeFirstHit = 0;

//Default const variables
unsigned long lockOut        = 40;    // the lockout time between hits for epee is 40ms
unsigned long minHitDuration = 2;     // the minimum amount of time the tip needs to be depressed 2ms

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
   
  
  hitCheckA();
  
  if(hitA)
  {
  a = micros();
  }
  
  hitCheckB();
    
  if(state==1)
   {
     if(hitA)
     {
       timeFirstHit = timeHitA;
       digitalWrite(onTargetA, HIGH);
       //switch to waiting for second hit state
       state = 2;
     }
     if(hitB)
     {
       timeFirstHit = timeHitB;
       digitalWrite(onTargetB, HIGH);
       state = 2;
     }
   }
   //waiting for second hit
   else if(state == 2)
   {
     //wait for second hit if still within lockout time
     
     if(timeFirstHit + lockOut > millis())
     {
       if(hitA){digitalWrite(onTargetA, HIGH);}
       if(hitB){digitalWrite(onTargetB, HIGH);}
     }
     else
     {
       b = micros();Serial.print("Difference:");Serial.println(a);Serial.println(b);//Serial.println(timeFirstHit);Serial.println(timeHitA);
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
     digitalWrite(onTargetA, LOW);
     digitalWrite(onTargetB, LOW);
     hitA = false;
     hitB = false;
     timeFirstHit = 0;
     state = 1;
   }
    
}

void hitCheckA()
{
   weaponA = digitalRead(weaponPinA);
   if(weaponA == 1  && !firstHitA)
   {
       timeHitA  = millis();
       firstHitA = true;
   }
   if(millis() >= timeHitA + minHitDuration && weaponA == 1)
   {
     hitA = true;
   }
   else if(weaponA == 0)
   {
      firstHitA = false;
      
   }   
   else
   {
     
   }  
}
void hitCheckB()
{
   weaponB = digitalRead(weaponPinB);
   if(weaponB == 1  && !firstHitB)
   {
       timeHitB  = millis();
       firstHitB = true;
   }
   if(millis() >= timeHitB + minHitDuration && weaponB == 1)
   {
     hitB = true;
   }
   else if(weaponB == 0)
   {
      firstHitB = false;
      
   }   
   else
   {
     
   }  
}

