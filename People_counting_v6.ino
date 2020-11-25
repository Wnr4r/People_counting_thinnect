/*Protoype code for People counting sensor using PIR v6
 * Authors: Toghrul, Tomisin, Emre and Kayode
 * InterruptV6 version (c) 2020
 */

//PIN NUMBERS
#define pirPin 2
#define pirPin2 3


int counting = 0;   //Number of people in the room

int calibrationTime = 30; //10 secs    //Time required for sensor to be calibrated. Sensors output until this time point is unstable


//Variables for holding the last checked time
unsigned long timeSensor1 = millis();   
unsigned long timeSensor2 = millis();

long timeInterval = 900;        //Time interval for ignoring the output of other sensor

volatile bool SomeoneIN, SomeoneOUT;
volatile bool SomeoneReallyIN, SomeoneReallyOUT;    //Flags for noting sensor outputs
bool shouldIncrement = HIGH;
bool shouldDecrement = HIGH;
void increment();
void deccrement();


void setup() {

   Serial.begin(9600);
   Serial.println("Starting....");
   Serial.println("");
   pinMode(pirPin, INPUT);
   pinMode(pirPin2, INPUT);
   //give the sensor some time to calibrate
   Serial.print("calibrating sensor ");
   for(int i = 0; i < calibrationTime; i++){
     Serial.print(".");
     delay(1000);
     }
   Serial.println(" done");
   Serial.println("SENSOR ACTIVE");
   delay(50);
   attachInterrupt(digitalPinToInterrupt(pirPin), increment, RISING);
   attachInterrupt(digitalPinToInterrupt(pirPin2), decrement, RISING);
}

void loop() {
  
unsigned long currentTime = millis();

//Serial.print("Status of sensor 1 is: ");
//Serial.print(digitalRead(pirPin));
//Serial.print(" and sensor 2 is: ");
//Serial.println(digitalRead(pirPin2));


  
  if (SomeoneIN == LOW)
  {
    if ((currentTime - timeSensor1) < timeInterval) //if 2s has elapsed or entering sequence not complete
    {
        if ((currentTime - timeSensor1) > 100)
        {
          shouldIncrement = LOW;
        }
          
    }
    else 
     {
       SomeoneIN = HIGH;
       shouldIncrement = HIGH;
       timeSensor1 = currentTime;
     }
   
  }
  else
  {
    timeSensor1 = currentTime;
  }

  if (shouldIncrement == LOW && SomeoneReallyIN == LOW)
  {
      counting = counting + 1;
      Serial.print("Someone crossed the zone, pir reads low, was high, count now: ");
      Serial.println(counting);
      Serial.println("Incremented");
      Serial.println("someone entered");
      delay(timeInterval + 100);
      currentTime = millis();
      timeSensor1 = currentTime;
      SomeoneIN = HIGH;
      SomeoneReallyIN = HIGH;
      shouldIncrement = HIGH;
  }
  

  if(SomeoneOUT == LOW)
  {
    if ((currentTime - timeSensor2) < timeInterval)  //if 2s has elapsed or entering sequence not complete
      {
         if ((currentTime - timeSensor2) > 100)
         {
              shouldDecrement = LOW;
         }   
      }
      else   
      {
         SomeoneOUT = HIGH;
         shouldDecrement = HIGH;
         timeSensor2 = currentTime;
      }
  }
  else
  {
    timeSensor2 = currentTime;
  }



  if (shouldDecrement == LOW && SomeoneReallyOUT == LOW)
  {
                    counting = counting - 1;
                  Serial.print("Someone crossed the zone, pir reads low, was high, count now: ");
                  Serial.println(counting);
                  Serial.println("Decremented");
          
                  
                  Serial.println("someone exit");
                  delay(timeInterval + 100);
                  currentTime = millis();
                  timeSensor2 = currentTime;
                  SomeoneOUT = HIGH;
                  SomeoneReallyOUT = HIGH;
                  shouldDecrement = HIGH;
  }
}



void increment()
{
      //Serial.println("pir reads high");
    SomeoneIN = LOW; //random flag
    if (shouldDecrement == LOW)
    {
      SomeoneReallyOUT = LOW;
    }
    else
    {
      
      SomeoneReallyOUT = HIGH;
    }

}


void decrement()
{
      //Serial.println("pir reads high");
    SomeoneOUT = LOW; //random flag
    if (shouldIncrement == LOW)
    {
      SomeoneReallyIN = LOW;
    }
    else
    {
      SomeoneReallyIN = HIGH;
    }
}
