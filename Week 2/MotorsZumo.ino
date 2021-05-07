#define rightMotorDir 15 // the number of the left motor direction pin
#define leftMotorDir  16 // the number of the right motor direction pin
#define rightMotorSpeed 9 // the number of the right motor speed pin
#define leftMotorSpeed 10 // // the number of the left motor speed pin

void motorControlLeft(int dir, int power);
void motorControlRight(int dir, int power);

void setup() {
  pinMode(rightMotorDir, INPUT);
  pinMode(leftMotorDir, INPUT);
  pinMode(rightMotorSpeed, OUTPUT);
  pinMode(leftMotorSpeed, OUTPUT);
}

void loop() {
//  motorControlLeft(0, 100);
//  delay(1000);
//  motorControlLeft(1,100);
    digitalWrite(leftMotorDir, 0);
    analogWrite(leftMotorSpeed, 50);
    digitalWrite(rightMotorDir, 0);
    analogWrite(rightMotorSpeed, 50);
}

void motorControlLeft(int dir, int power){
  digitalWrite(leftMotorDir, dir);
  analogWrite(leftMotorSpeed, power);
}

void motorControlRight(int dir, int power){
  digitalWrite(rightMotorDir, dir);
  analogWrite(rightMotorSpeed, power);
}
