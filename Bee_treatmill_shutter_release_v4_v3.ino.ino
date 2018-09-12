//Bee treatmill shutter release & light control

//For Arduino UNO

//Remco Pieters
//Wageningen University & Research 
//v4.1 - 20180703

//Parameters
int hallPin = 2; //digital Hall sensor connect to digital pin 2
int trgPin = 3; //camera trigger, shutter release, connect to digitial pin 3. Circuits separated by opto-coupler
int ledPin = 13; //PWM dimming led light

boolean CameraOn = LOW; //switch on/off data acquisition from camera 
boolean hall = LOW; // level of Hall sensor, hall = LOW : magnet above sensor
boolean trigger = LOW; //State value of trigger
unsigned long time1 = 0; //current time
unsigned long time2 = 0; //time of previous Hall sensor activation
unsigned long roundtime = 0; //Roundtime, time bee takes to fly 360 degrees
float roundtime2 = 0;
int cameradelay = 10; //Delay of the camera between shutter release signal and actual sensor exposure, [ms]
float travelangle = 10; //Travel angle between hall sensor and camera FOV [degrees]
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

  Serial.println("Type 1# to start camera acquisition, Type 0# to stop");
  //CameraOn = HIGH;
}

void loop() {
  if (Serial.available() > 0) {
  // read the command
  int command = Serial.parseInt();
  //Serial.println(command);
  if (Serial.read() == '#') {
     //Serial.flush();
    if (command==1){
      CameraOn = HIGH;
      hall = LOW; 
      trigger = LOW; 
      time1 = 0; 
      time2 = millis(); 
      roundtime = 0; 
      roundtime2 = 0;
      Serial.println("start");
      }
      else
      {
      CameraOn = LOW;
      hall = LOW; 
      trigger = LOW; 
      Serial.println("stop");
      }
  }
  }

  
  if (CameraOn == HIGH){
  //Read Hall sensor (continious loop ~ 1 MHz)
  hall = digitalRead(hallPin);
  //Serial.println(hall);
  //Get current timestamp
  time1 = millis();
  //Serial.println(time1);
  
  if (hall == LOW){
  //Magnet above Hall sensor
    //overwrite time2 if time1<time2 because of unsigned longs -> doesn't go to zero a ngative substraction but 2^32 - rest
  if (time1 < time2){
    time2=0;
  }
  //Compute roundtime
  roundtime = time1-time2;

  
  
  time2 = time1;
  
  if (roundtime > roundtime2/2 && roundtime > min_roundtime){
  //roundtime2 is the period of the previous round
  roundtime2 = roundtime;
  Serial.print('T');
  Serial.println(roundtime);
  //Serial.print('D');
  //Serial.println(roundtime*(travelangle/360)-cameradelay);
 // Activate camera trigger
   delay(abs(roundtime*(travelangle/360)-cameradelay));
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
  
}


