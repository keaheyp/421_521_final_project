//===========================================================================
//                                                                           
//  Desc:    Arduino Code for an epee fencing scoring apparatus              
//  Devs:    Pelham Keahey and Sydney Gibson                                                            
//  Date:    Nov 2014
//  Citation:  Code and ideas pulled from wnew's epeeBox on github, thank you!!
//  Updated: Dec 2014                                                       
//  Notes:   State machine implementation of an epee scoring box.  
//                                                                          
// 
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
uint8_t weaponA          = 0;         //Used to track if there is a weapon hit on weaponA Pin
uint8_t weaponB          = 0;         //Used to track if there is a weapon hit on weaponB Pin

const uint8_t buzzerPin  = 4;         //Output pin for sounding the buzzer 

//boolean variables
boolean hitA = false;                 //boolean used for indicating weaponA hit
boolean hitB = false;                 //boolean used for indicating weaponB hit

boolean firstHitA = false;            //was this the first hit on A
boolean firstHitB = false;            //was this the first hit on B

//DEBUGGING VARIABLES
//unsigned long a,b,c,d =0;            //UNCOMMENT to enable timing testing

//timing variables
unsigned long timeHitA     = 0;      //indicates when a hit occured on weaponA
unsigned long timeHitB     = 0;      //indicates when a hit occured on weaponB
unsigned long timeFirstHit = 0;      //time of first hit

//Default const variables
unsigned long lockOut        = 4500;    // the lockout time between hits for epee is 40ms
unsigned long minHitDuration = 2000;     // the minimum amount of time the tip needs to be depressed 2ms

//state variable
int state = 1;                        //variable for setting the state, defaults to 1
boolean i = true;
//SETUP FUNCTION
void setup() {
  
   //Setup OUTPUTS
   pinMode(onTargetA,  OUTPUT);
   pinMode(onTargetB,  OUTPUT);
   //SETUP INPUTS
   pinMode(weaponPinA, INPUT);
   pinMode(weaponPinB, INPUT);
   //SETUP Buzzer
   pinMode(buzzerPin,  OUTPUT);
   //SETUP SERIAL OUTPUT
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
   
  //function calls used to check for a hit on weaponA and weaponB
  hitCheckA();
  hitCheckB();
  
  //waiting for first hit state  
  if(state==1)
   {
     if(hitA)
     {
       //record time of first hit if it was weaponA and turn on light
       timeFirstHit = timeHitA;
       digitalWrite(onTargetA, HIGH);
       tone(buzzerPin,1000);
       
       //switch to waiting for second hit state
       state = 2;
     }
     if(hitB)
     {
       //record time of first hit if it was weaponB and turn on light
       timeFirstHit = timeHitB;
       digitalWrite(onTargetB, HIGH);
       tone(buzzerPin,1000);
       //switch to second state to wait for second hit
       state = 2;
     }
   }
   //waiting for second hit if one occurs
   else if(state == 2)
   {
     //wait for second hit if still within lockout time turn on light if hit detected   
     if(micros() < timeFirstHit + lockOut)
     {
       //turn on appropriate LED if second hit occurs
       if(hitA){digitalWrite(onTargetA, HIGH);}
       if(hitB){digitalWrite(onTargetB, HIGH);}
     }
     else
     {
       //switch to reset state when lockout expires
       state = 3;
       
      // ===== CODE FOR TIMING TESTING AND DEBUGGING  ====
      /*     //Remove to enable time testing
       a = timeFirstHit; //Time of first hit as recorded from state 1
       b = micros();     //time to reach this point (all hits registered and indicated (should be lockout time, 45ms))
       c = b - a;        //take difference to find the time it took to go from the first hit to end of lockout
       //Print out all timing results
       //Serial.println(a);
       //Serial.println(b);
       Serial.print("First Hit:      ");
       Serial.print(timeFirstHit);
       Serial.println(" us");
       Serial.print("End of lockout: ");
       Serial.print(b);
       Serial.println(" us");
       Serial.print("Difference:     ");
       Serial.print(c);
       Serial.println(" us");
       Serial.println("======");
       */      //Remove to enable timing testing
      // **Through Testing timing was found to be 45 +-1 ms, micros instead of micros could have been used, but FIE rules require only ms precision
        
      // ==================================================       
     }
   }
   //lockout time expired keep buzzer on for 2 seconds and lights on for 4 seconds (2 more than buzzer) then reset
   else if(state == 3)
   {
     delay(2000);
     noTone(buzzerPin); //turn off buzzer
     delay(2000);
     digitalWrite(onTargetA, LOW); //turn off green LED
     digitalWrite(onTargetB, LOW); //turn off red LED
     hitA = false; //reset hit variables
     hitB = false; //reset hit variables
     timeFirstHit = 0; //0 first hit time;
     state = 1; //set state back to 1
     i = true;
   }
    
}
//used to check for a hit on weaponA each function call
void hitCheckA()
{
   //read status of weaponA pin (high = hit, low = no hit)
   weaponA = digitalRead(weaponPinA);
   
   //if hit detected record time and start waiting to see if the tip was depressed long enough
   if(weaponA == 1  && !firstHitA)
   {
       timeHitA  = micros();
       //set to to true to indicate a hit has been detected
       firstHitA = true;
   }
   //insure that the weaponA was depressed for the required time, if true set hitA to true
   if(micros() >= timeHitA + minHitDuration && weaponA == 1)
   {
     
     hitA = true;
   }
   //if the tip does not stay depressed long enough reset first hit and start waiting for another hit
   else if(weaponA == 0)
   {
      firstHitA = false;    
   }   
}
//used to check for a hit on weaponB each function call
void hitCheckB()
{
   //read status of weaponB pin (high = hit, low = no hit)
   weaponB = digitalRead(weaponPinB);
   
    //if hit detected record time and start waiting to see if the tip was depressed long enough
   if(weaponB == 1  && !firstHitB)
   {
       timeHitB  = micros();
       //set to to true to indicate a hit has been detected
       firstHitB = true;
   }
   if(micros() >= timeHitB + minHitDuration && weaponB == 1)
   {
     hitB = true;
   }
   //if the tip does stay depressed long enough reset first hit and start waiting for another hit
   else if(weaponB == 0)
   {
      firstHitB = false;      
   }   
}
