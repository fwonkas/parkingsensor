/* Parking Assistant

  This is a sketch for a simple parking assistant. When parking, it indicates
  whether the vehicle is too far, close enough or too close. It uses a
  red/green bicolor LED to indicate distance.

  It uses a Parallax Ping))) sensor to determine distance.

  Derived from: http://www.arduino.cc/en/Tutorial/Ping

 */

const int pingPin = 7;
const int greenPin = 8;
const int redPin = 9;
long sameTime = 0;
const long stayOn = 10000;
long lastDistance = 0;
const unsigned int danger = 9;
const int distancePin = A0;
unsigned int stopDistance;

void setup() {
  Serial.begin(9600);
}

void loop()
{
  long duration, inches;
  int LED, GND;
  int lightLevel = 255;
  boolean closeEnough, tooClose;

  stopDistance = constrain(analogRead(distancePin), 0, 685);
  stopDistance = map(stopDistance, 0, 685, 12, 72);

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // Determine how close (in inches) the object is.
  inches = duration / 74 / 2;

  // Determine whether the object is close enough.
  closeEnough = inches <= stopDistance;

  // Is the object too close?
  tooClose = inches <= danger;

  // If the object is close enough, set the red pin to receive current
  // and the green pin to be ground. Otherwise reverse it.
  LED = closeEnough ? redPin : greenPin;
  GND = closeEnough ? greenPin : redPin;

  // TODO: Do something interesting if the object is too close.
  if (tooClose) {
    Serial.println("TOO CLOSE");
  }

  if (lastDistance != inches) {
    // reset timer
    sameTime = 0;
  } else {
    sameTime += 100;
    if (sameTime >= stayOn) {
      lightLevel = 0;
    }
  }
  lastDistance = inches;

  analogWrite(GND, 0);
  analogWrite(LED, lightLevel);
  delay(100);
}

