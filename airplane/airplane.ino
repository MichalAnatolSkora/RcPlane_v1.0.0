#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
Servo servoMain;
Servo servoMain2;
Servo engine;
int xMapped = -1;
int yMapped = -1;
int engineValue = 0;
unsigned long last_usage_of_servos;
boolean attached = false;
int speed = 1218;

void setup()
{
  Serial.begin(9600);

  delay(1000);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  delay(1000);
  servoMain.attach(9);
  servoMain.write(90);
  delay(1000);
  servoMain.detach();
  servoMain2.attach(10);
  servoMain2.write(90);
  delay(1000);
  servoMain2.detach();
  last_usage_of_servos = millis();

  engine.attach(6);
  delay(50);
  arm(engine);
}

void loop()
{
  if (radio.available())
  {
    int text[3];
    radio.read(&text, sizeof(text));

    if (engineValue != text[2])
    {
      engineValue = text[2];
      engine.write(engineValue + speed);
      Serial.print("engine: ");
      Serial.println(engineValue + speed);
    }

    if (xMapped == text[0] && yMapped == text[1])
    {
      return;
    }

    xMapped = text[0];
    yMapped = text[1];

    Serial.print(text[0]);
    Serial.print(":");
    Serial.println(text[1]);

    int normalizedX = min(max(xMapped, 10), 170);
    int normalizedY = min(max(yMapped, 10), 170);

    if (87 <= normalizedX || normalizedX <= 93)
    {
      normalizedX = 90;
    }

    if (87 <= normalizedY || normalizedY <= 93)
    {
      normalizedY = 90;
    }

    servoMain.attach(9);
    servoMain2.attach(10);
    attached = true;
    servoMain.write(normalizedX);
    servoMain2.write(normalizedY);
    last_usage_of_servos = millis();
  }

  if (attached && millis() - last_usage_of_servos > 1000)
  {
    attached = false;
    servoMain.detach();
    servoMain2.detach();
  }
}

void arm(Servo m)
{
  Serial.print("Arming.");
  m.write(0);
  delay(100);
  m.write(1224); //A value at which the motor starts turning
  delay(2000);
  m.write(1024); //A value at which the motor stands still
  delay(3000);
  Serial.println(" Armed!");
}