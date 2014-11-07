//#define BUTTON2 2  //input for green
//#define BUTTON3 3 // input for red

//#define LED_PIN12 12 //output for green
//#define LED_PIN13 13 //output for red

//inputs
int BUTTON2 = 2; //input for green LED
int BUTTON3 = 3; //input for red LED


//outputs
int LED_GREEN = 12; //output to turn on green LED
int LED_RED   = 13; //output to turn on red LED



//Program Variables
int counter  = 0;
int b2       = 0;
int b1       = 0;



void setup()
{
    //start recording for serial output
    Serial.begin(9600);
    //setup Inputs
    pinMode(BUTTON2, INPUT);
    pinMode(BUTTON3, INPUT);
    //setup Outputs
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);
}

void loop()
{
    //delay(200);
    //star counting every loop (diagnostic purposes)
    counter=counter+1;
    
    //read button status
    b1 = digitalRead(BUTTON2);
    b2 = digitalRead(BUTTON3);
    
    //GREEN LED BUTTON
    if(b1 == 1)
    {
         digitalWrite(LED_GREEN, HIGH);
         delay(3000);
    }
    
    else
    {
         digitalWrite(LED_GREEN, LOW);
    }
      
    //RED LED Button
    if(b2 == 1)
    {
         digitalWrite(LED_RED, HIGH);
         delay(3000);
    }
    
    else
    {
         digitalWrite(LED_RED, LOW);
    }
    
   
    //diagnostic outputs
    Serial.print("Count:");
    Serial.println(counter);
    Serial.print("Button 1:");
    Serial.println(b1);
    Serial.print("Button 2:");
    Serial.println(b2);
}


