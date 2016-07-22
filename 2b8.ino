#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

#include <AFMotor.h>
#include <Servo.h>

// Pins for the 4 input channels.
const int pin1 = 18;  // TX1
const int pin2 = 19;  // RX1
const int pin3 = 20;  // SDA (SPI data)
const int pin4 = 21;  // SCL (SPI clock)

// onOffBreak separates "off" and "on".
const unsigned long onOffBreak = 1500;

unsigned long lastPrintTime = 0;

const int mp3BusyPin = 48;
SoftwareSerial mp3Serial(50, 51); // RX, TX

Servo scoop;
AF_DCMotor motorForward(1);
AF_DCMotor motorTurn(2);

const int speed = 255;

void setup() {
  Serial.begin(115200);

  // MP3 player setup
  mp3Serial.begin(9600);
  mp3_set_serial(mp3Serial);  // set Serial for DFPlayer-mini mp3 module 
  delay(1);                   // delay 1ms to set volume
  mp3_set_volume(18);         // value 0~30
  pinMode(mp3BusyPin, INPUT);

  scoop.attach(9);

  motorForward.setSpeed(speed);
  motorTurn.setSpeed(speed);


}

void loop() {
  unsigned long time1 = pulseIn(pin1, HIGH);
  unsigned long time2 = pulseIn(pin2, HIGH);
  unsigned long time3 = pulseIn(pin3, HIGH);
  unsigned long time4 = pulseIn(pin4, HIGH);

  int file_index;

  // Control
  if (time4 >= onOffBreak) {
    if (digitalRead(mp3BusyPin) == HIGH) {  // If not busy
      file_index=random(32);
      Serial.print("Playing file ");
      Serial.println(file_index);
      mp3_play(file_index);
    } else {
      Serial.println("Busy");
    }
  }

  if (time1 <= 1200){
    motorForward.run(FORWARD);
  } else if (time1 >= 1600){
    motorForward.run(BACKWARD);
  } else {
    motorForward.run(RELEASE);
  }

  if (time2 <= 1200){
    motorTurn.run(FORWARD);
  } else if (time2 >= 1600){
    motorTurn.run(BACKWARD);
  } else {
    motorTurn.run(RELEASE);
  }


  if (time3 <= 1200){
    scoop.write(25);
  } else if (time3 >= 1600){
    scoop.write(90);
  }
  

  // Print the PWM width every second
  unsigned long time = millis();
  if (time - lastPrintTime >= 400) {
    Serial.print(pin1);
    Serial.print(":");
    Serial.print(time1);
    Serial.print(" ");
    Serial.print(pin2);
    Serial.print(":");
    Serial.print(time2);
    Serial.print(" ");
    Serial.print(pin3);
    Serial.print(":");
    Serial.print(time3);
    Serial.print(" ");
    Serial.print(pin4);
    Serial.print(":");
    Serial.print(time4);
    Serial.println();

    lastPrintTime = time;
  }
}
