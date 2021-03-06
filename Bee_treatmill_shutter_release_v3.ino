//Bee treatmill shutter release & light control

//For Arduino UNO

//Remco Pieters
//Wageningen University & Research 
//v3 - 20180703

//Parameters
int hallPin = 2; //digital Hall sensor connect to digital pin 2
int trgPin = 3; //camera trigger, shutter release, connect to digitial pin 3. Circuits separated by opto-coupler
int ledPin = 13; //PWM dimming led light

boolean hall = LOW; // level of Hall sensor, hall = LOW : magnet above sensor
boolean trigger = LOW; //State value of trigger
unsigned long time1 = 0; //current time
unsigned long time2 = 0; //time of previous Hall sensor activation
unsigned long roundtime = 0; //Roundtime, time bee takes to fly 360 degrees
float roundtime2 = 0;
int cameradelay = 10; //Delay of the camera between shutter release signal and actual sensor exposure, [ms]
float travelangle = 270; //Travel angle between hall sensor and camera FOV [degrees]
int min_roundtime = 200; //Minimal round time of a flying bee, @2 m/s ~ 300 ms
void setup() {
  //Set up serial connection
  Serial.begin(9600);
  Serial.flush();
  delay(10);

  //Set up pin modes
  pinMode(hallPin, INPUT);
  pinMode(trgPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  //Read Hall sensor (continious loop ~ 1 MHz)
  hall = digitalRead(hallPin);
  //Serial.println(hall);
  //Get current timestamp
  time1 = millis();
  //Serial.println(time1);
  
  if (hall == LOW){
  //Magnet above Hall sensor
  //Compute roundtime
  roundtime = time1-time2;
  time2 = time1;
  
  if (roundtime > roundtime2/2 && roundtime > min_roundtime){
  //roundtime2 is the period of the previous round
  roundtime2 = roundtime;
  Serial.print('T');
  Serial.println(roundtime);
 // Activate camera trigger
   delay(roundtime*(travelangle/360)-cameradelay);
        digitalWrite(trgPin, HIGH);
        delay(1); //Shutter release signal high for 1 ms
        digitalWrite(trgPin, LOW);
        trigger = LOW;
       
  }
 else{

  digitalWrite(trgPin, LOW);
  delay(1); 
}
  }
  
}
