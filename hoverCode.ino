//  ENGR 290 FALL 2022
//  AUTONOMOUS HOVERCRAFT CODE

 // Team #08:
 // Abdallah, Yasmine
 // El Yamani, Ahmed
 // Pedroza Hernandez, Marcelo
 // Shirazi, Aryan



#include <Servo.h> 
#include <MPU6050_light.h>

#define liftFan  4
#define pushFan 7                                                                                                                                       //P4
#define backFan 6

#define trigLeft 13
#define echoLeft 3

#define echoRight 2
#define trigRight 11

#define highPerfSensor 10

const int backServoPin = 9;
const int pushServoPin = 5;

Servo backServo;
Servo pushServo;

MPU6050 mpu(Wire);
unsigned long timer = 0;

float pulse,frontDistance,durationL, distanceL, distanceR,durationR;
float speed = 343; 
int index=0;

void setup() {
  
  pinMode(highPerfSensor, INPUT);

  pinMode(liftFan,OUTPUT);
  pinMode(pushFan,OUTPUT);
  pinMode(backFan,OUTPUT);
  
  pinMode(trigLeft, OUTPUT);
  pinMode(echoLeft, INPUT);
  pinMode(trigRight, OUTPUT);
  pinMode(echoRight, INPUT);

  backServo.attach(backServoPin);
  pushServo.attach(pushServoPin);

//servo to neutral position

  pushServo.write(90);
  backServo.write(90);
  
  Serial.begin(9600); 
  Wire.begin();
  byte status = mpu.begin();
   Serial.print(F("MPU6050 status: "));
   Serial.println(status);
   while (status != 0) { } // stop everything if could not connect to MPU6050
   Serial.println(F("Calculating offsets, do not move MPU6050"));
   mpu.calcOffsets(); // gyro and accelero
   Serial.println("Done!\n");
   delay(1000);
   analogWrite(liftFan,255);
}

void loop() {
  //IMU update

  mpu.update();
  analogWrite(pushFan,155);
  //control servos
       if ((millis() - timer) > 10) { // print data every 10ms
      Serial.print("Z : ");
      Serial.print(mpu.getAngleZ());
    }
    if(-(mpu.getAngleZ())>=90 || (-mpu.getAngleZ())<=-90){
     pushServo.write(90+mpu.getAngleZ());
  } 
  else{
    pushServo.write(90-mpu.getAngleZ());
  } 



 //read front distance
  pulse = pulseIn(highPerfSensor, HIGH);
  //delay(200);
  frontDistance =  (pulse/147) * 2.54;
  Serial.print("\tF sensor: ");
  Serial.print(frontDistance);
  Serial.println("cm");
  
  if(frontDistance<35){
    analogWrite(pushFan,0);
    analogWrite(backFan, 255);
    delay(1000);

//read right sensor
     digitalWrite(trigRight, LOW);
  delayMicroseconds(10);
  digitalWrite(trigRight, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigRight, LOW);

  durationR = pulseIn(echoRight, HIGH);
  distanceR=((speed*durationR)/10000)/2;

    Serial.print("\tSensor R: ");
  Serial.print(distanceR);
  Serial.print(" cm\t");
 
  //left sensor


  digitalWrite(trigLeft, LOW);
  delayMicroseconds(10);
  digitalWrite(trigLeft, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigLeft, LOW);
 
  durationL = pulseIn(echoLeft, HIGH);
 
  distanceL=((speed*durationL)/10000)/2;

  Serial.print("Sensor L: ");
  Serial.print(distanceL);
  Serial.println(" cm");

  if(distanceL>distanceR){
    turnLeft();
  }
  else{
    turnRight();
  }
}
 else{
   //forward prop.
 analogWrite(pushFan,155);
  //control servos

 }
}

void turnLeft(){

  backServo.write(180);
  pushServo.write(180);
  
  analogWrite(pushFan,155);
  analogWrite(backFan,155);
  delay(1000);
  analogWrite(backFan,0);
  pushServo.write(90);
  backServo.write(90); 

}

void turnRight(){  
  backServo.write(0);
  pushServo.write(0);
  analogWrite(pushFan,155);
  analogWrite(backFan,155);
  delay(1000);
  analogWrite(backFan,0);
  pushServo.write(90);
  backServo.write(90);
 
}
