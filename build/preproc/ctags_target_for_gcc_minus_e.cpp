# 1 "C:\\Users\\lylyi\\Documents\\Code\\swerve\\src\\src.ino"
# 2 "C:\\Users\\lylyi\\Documents\\Code\\swerve\\src\\src.ino" 2


# 3 "C:\\Users\\lylyi\\Documents\\Code\\swerve\\src\\src.ino"
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
        angle = static_cast<int>(atan_value * 180.0 / 
# 35 "C:\\Users\\lylyi\\Documents\\Code\\swerve\\src\\src.ino" 3
                                                     3.14159265358979323846 /* pi */
# 35 "C:\\Users\\lylyi\\Documents\\Code\\swerve\\src\\src.ino"
                                                         );
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
  pinMode(driveMotorIn1, 0x1);
  pinMode(driveMotorIn2, 0x1);
  pinMode(driveMotorEnablePin, 0x1);

  pinMode(turnMotorIn1, 0x1);
  pinMode(turnMotorIn2, 0x1);
  pinMode(turnMotorEnablePin, 0x1);

  digitalWrite(driveMotorEnablePin, 0x1);
  digitalWrite(turnMotorEnablePin, 0x1);

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
        digitalWrite(driveMotorIn1, 0x1);
        digitalWrite(driveMotorIn2, 0x0);
      } else {
        digitalWrite(driveMotorIn1, 0x0);
        digitalWrite(driveMotorIn2, 0x1);
        driveMotorSpeed = -driveMotorSpeed;
      }
      analogWrite(driveMotorEnablePin, driveMotorSpeed);

      if (turnMotorSpeed > 0) {
        digitalWrite(turnMotorIn1, 0x1);
        digitalWrite(turnMotorIn2, 0x0);
      } else {
        digitalWrite(turnMotorIn1, 0x0);
        digitalWrite(turnMotorIn2, 0x1);
        turnMotorSpeed = -turnMotorSpeed;
      }
      analogWrite(turnMotorEnablePin, turnMotorSpeed);
    }
  }
}
