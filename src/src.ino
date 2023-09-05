#include <math.h>

const int driveMotorIn1 = 3;
const int driveMotorIn2 = 4;
const int driveMotorEnablePin = 5;

const int turnMotorIn1 = 7;
const int turnMotorIn2 = 8;
const int turnMotorEnablePin = 9;

const int joystickRange = 60;
const int motorSpeedRange = 255;
const double gearRatio = 25.0 / 79.0;

int quadrantFromCoordinates(int x, int y) {
  if (x >= 0 && y >= 0) return 1;
  if (x < 0 && y >= 0) return 2;
  if (x < 0 && y < 0) return 3;
  if (x >= 0 && y < 0) return 4;
}

int getHypotenuse(int x, int y) {
  return sqrt((x - 30) * (x - 30) + (y - 30) * (y - 30));
}

int calculateAngle(int x, int y) {
    int dx = x - 30;
    int dy = y - 30;

    int quadrant = quadrantFromCoordinates(dx, dy);
    int angle = 0;
    
    if (dx != 0) {
        double atan_value = atan((double)dy / dx);
        angle = static_cast<int>(atan_value * 180.0 / M_PI);
    } else {
        angle = 90;
    }

    switch (quadrant) {
      case 1:
        break;
      case 2:
        angle += 180;
        break;
      case 3:
        angle += 180;
        break;
      case 4:
        angle += 360;
        break;
    }
    
    return angle;
}

void setup() {
  pinMode(driveMotorIn1, OUTPUT);
  pinMode(driveMotorIn2, OUTPUT);
  pinMode(driveMotorEnablePin, OUTPUT);
  
  pinMode(turnMotorIn1, OUTPUT);
  pinMode(turnMotorIn2, OUTPUT);
  pinMode(turnMotorEnablePin, OUTPUT);
  
  digitalWrite(driveMotorEnablePin, HIGH);
  digitalWrite(turnMotorEnablePin, HIGH);
  
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    int commaIndex = data.indexOf(',');
    if (commaIndex != -1) {
      int x = data.substring(0, commaIndex).toInt();
      int y = data.substring(commaIndex + 1).toInt();

      int driveMotorSpeed = map(y, 0, joystickRange, -motorSpeedRange, motorSpeedRange);
      int turnMotorSpeed = map(x, 0, joystickRange, -motorSpeedRange, motorSpeedRange);

      turnMotorSpeed = static_cast<int>(turnMotorSpeed * gearRatio);

      int angle = calculateAngle(x, y);
      
      int hypotenuse = getHypotenuse(x, y);
      
      driveMotorSpeed = map(hypotenuse, 0, joystickRange, 0, motorSpeedRange);

      if (driveMotorSpeed > 0) {
        digitalWrite(driveMotorIn1, HIGH);
        digitalWrite(driveMotorIn2, LOW);
      } else {
        digitalWrite(driveMotorIn1, LOW);
        digitalWrite(driveMotorIn2, HIGH);
        driveMotorSpeed = -driveMotorSpeed;
      }
      analogWrite(driveMotorEnablePin, driveMotorSpeed);
      
      if (turnMotorSpeed > 0) {
        digitalWrite(turnMotorIn1, HIGH);
        digitalWrite(turnMotorIn2, LOW);
      } else {
        digitalWrite(turnMotorIn1, LOW);
        digitalWrite(turnMotorIn2, HIGH);
        turnMotorSpeed = -turnMotorSpeed;
      }
      analogWrite(turnMotorEnablePin, turnMotorSpeed);
    }
  }
}
