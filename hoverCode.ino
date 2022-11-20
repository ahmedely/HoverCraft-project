/*
  ENGR 290 FALL 2022
  AUTONOMOUS HOVERCRAFT CODE

  Team #08:
  Abdallah, Yasmine
  El Yamani, Ahmed
  Pedroza Hernandez, Marcelo
  Shirazi, Aryan

  *NOTES*

  tailFan pin is YET TO BE DETERMINED, for now all of its code is commented out.
  possible tailFan pins are:
  P18: (PD7 = 7) ON/OFF channel 0 negative terminal for the fan. Drain of the control MOSFET. NOT GND!
  P17: (PD4 = 4) ON/OFF channel 1 negative terminal for the fan. Drain of the control MOSFET. NOT GND!
  P11: IF (P9 = 9) is used for servo, P11 CANNOT be used for fan. IF NOT: Pin#1 (yellow wire) of a servo-motor connector goes to this pin.

  *** liftFan and servoTailPin are both connected to (P3 = 5) THIS MUST BE CHANGED ***

*/

//Libraries
#include <Servo.h> 

//Pin Assignments
//Fans
#define liftFan  5 //P3
#define noseFan 6 //P4
//#define tailFan //TBD
//US LEFT
#define trigPinL 11 //P6
#define echoPinL 2 //P6
//US RIGHT
#define trigPinR 13 //P13
#define echoPinR 3 //P13
//US High Perf
const int highPerf = 8; //P10
//Servos
const int servoNosePin = 9; //P11
const int servoTailPin = 5; //P3

//Servo Objects
Servo servoNose;
Servo servoTail; 

//Variables
float durationL, durationR, distanceL, distanceR;
float pulse, inches, distanceNose;
float speed = 343; 

void setup() {
  Serial.begin(9600); 
  
  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinL, INPUT);
  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR, INPUT);
  pinMode(highPerf, INPUT);

  servoNose.attach(servoNosePin);
  servoTail.attach(servoTailPin);

  //Set Fans on Servos to Neutral Position
  servoNose.write(90);
  servoTail.write(90);
}

void loop() {

  //Left US Pulse & Distance
  digitalWrite(trigPinL, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPinL, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinL, LOW);
  durationL = pulseIn(echoPinL, HIGH);
  distanceL=((speed*durationL)/10000)/2;

  //Right US Pulse & Distance
  digitalWrite(trigPinR, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPinR, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinR, LOW);
  durationR = pulseIn(echoPinR, HIGH);
  distanceR=((speed*durationR)/10000)/2;

  //Left US Print
  Serial.print("Left Distance: ");
  Serial.print(distanceL);
  Serial.print(" cm ");
  Serial.println(" ");

  //Right US Print
  Serial.print("Right Distance: ");
  Serial.print(distanceR);
  Serial.print(" cm ");
  Serial.println(" ");
  
  //highPerf Pulse & Distance
  pulse = pulseIn(highPerf, HIGH);
  inches = pulse / 147; //147uS per inch
  distanceNose = inches * 2.54;
  Serial.print("Nose Distance: ");
  Serial.print(distanceNose);
  Serial.print(" cm ");
  Serial.println(" ");

  //Provide Lift
  analogWrite(liftFan, 255);

  //Stop Hovercraft When Close to Front Obstacle
  if (distanceNose < 20) {
//    analogWrite(tailFan, 0); //Tail Fan Off
    analogWrite(noseFan, 50); //"Braking" Mechanism TBD
    analogWrite(noseFan, 0); //Nose Fan Off
    decisionTurn();
    }

  //Provide Forward Propulsion
//  else analogWrite(tailFan, 100);
    
  delay(100);
}

void decisionTurn(){
  if (distanceL < distanceR) rightTurn();
  else if(distanceL > distanceR) leftTurn();
}

void rightTurn (){

  //Both Fans Off
//  analogWrite(tailFan, 0);
  analogWrite(noseFan, 0);

  //Position Fans on Servos
  servoNose.write(180);
  servoTail.write(170);

  //Both Fans On (TIME ON TBD)
//  analogWrite(tailFan, 100);
  analogWrite(noseFan, 100);

  //Both Fans Back to Neutral
  servoNose.write(90);
  servoTail.write(90);

}

void leftTurn (){

  //Both Fans Off
//  analogWrite(tailFan, 0);
  analogWrite(noseFan, 0);

  //Position Fans on Servos
  servoNose.write(10);
  servoTail.write(10);

  //Both Fans On (TIME ON TBD)
//  analogWrite(tailFan, 100);
  analogWrite(noseFan, 100);

  //Both Fans Back to Neutral
  servoNose.write(90);
  servoTail.write(90);

}
