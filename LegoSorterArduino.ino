
#include <Servo.h>

#include <AccelStepper.h>
Servo servo1;
Servo servo2;

const int pirPin = 2;
unsigned long lastDetection = 0UL;
boolean last_sensor_state = false;
boolean current_sensor_state = false;

int stepper1Dir = 6;
int stepper1Step = 7;

int stepper2Dir = 8;
int stepper2Step = 9;

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data
boolean newData = false;
int dataNumber = 0; 

AccelStepper stepper1(1,stepper1Step, stepper1Dir);
AccelStepper stepper2(1,stepper2Step, stepper2Dir);


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  servo1.attach(12);
  servo2.attach(11);
  pinMode(pirPin, INPUT_PULLUP);

  stepper1.setMaxSpeed(500.0);
  stepper1.setAcceleration(100.0);
  stepper1.setSpeed(300);

  stepper2.setMaxSpeed(500.0);
  stepper2.setAcceleration(100.0);
  stepper2.setSpeed(100);

  servo1.write(0);
  servo2.write(0);


  Serial.println("Enter text to start");
    while(Serial.available() == 0){
    Serial.print(".");
    delay(500);
  }

  while(Serial.available()>0){
    Serial.read();
  }
  Serial.println("Setup Complete");
}

void loop() {
  
  unsigned long currentMillis = millis();
  stepper1.runSpeed();
  stepper2.runSpeed();
 if(stepper2.currentPosition()>=1000)
  {
    servo2.write(180);
  }
  if(stepper2.currentPosition()>=1500){
    servo2.write(0);
    stepper2.setCurrentPosition(0);
    stepper2.moveTo(2000);
    stepper2.setSpeed(100);
  }


  if(currentMillis - lastDetection > 500)
  {
    lastDetection = currentMillis;
    
    current_sensor_state = digitalRead(pirPin);
    if(current_sensor_state != last_sensor_state){
      last_sensor_state = current_sensor_state;
    
 
      if(current_sensor_state==false){
        Serial.println("Object detected!");
        stepper1.setSpeed(0);
        stepper2.setSpeed(0);   

        // Move the second servo to the position from serial input
      }
      
    }
    if(newData)
    moveServo();
    if(!newData)
    recvWithEndMarker();
  }
}

void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '>';
    char rc;
    
    if (Serial.available() > 0) {
        rc = Serial.read();
        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

void moveServo() {
      if (newData == true) {
        dataNumber = 0;             // new for this version
        dataNumber = atoi(receivedChars);   // new for this version
        //Serial.print("This just in ... ");
        //Serial.println(receivedChars);
        servo1.write(dataNumber);
        Serial.print("Moving Servo to ... ");    // new for this version
        Serial.println(dataNumber);     // new for this version

        newData = false;
        stepper1.setSpeed(300);
        stepper2.setSpeed(100);
    }
}
