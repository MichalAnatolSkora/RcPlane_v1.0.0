#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0;  // analog pin connected to X output
const int Y_pin = 1;  // analog pin connected to Y output
const int Slicer_pin = 2;

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

void setup()
{
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  // put your setup code here, to run once:
  Serial.begin(9600);
}

int i = 0;

int xMapped_old = -1;
int yMapped_old = -1;

void loop()
{
  int xValue = analogRead(X_pin);
  int yValue = analogRead(Y_pin);

  int xMapped = map(xValue, 0, 1023, 0, 180);
  int yMapped = map(yValue, 0, 1023, 0, 180);

    if(87<=xMapped || xMapped<=93)
    {
      xMapped = 90;
    }
    
    if(87<=yMapped || yMapped<=93)
    {
      yMapped = 90;
    }

  Serial.print("X-axis: ");
  Serial.println(xMapped);
  Serial.print("Y-axis: ");
  Serial.println(yMapped);
  Serial.print("\n\n");

  if(xMapped_old != xMapped || yMapped_old != yMapped)
  {
    const int text[2] = {xMapped, yMapped};
    radio.write(&text, sizeof(text));
    xMapped_old = xMapped;
    yMapped_old = yMapped;
    Serial.print("sent:");
  }

  delay(50);
  // radio.write(&text, sizeof(text));
  // Serial.println(text);
  // delay(1000);
}

// Serial.print("Switch:  ");
// Serial.print(digitalRead(SW_pin));
// Serial.print("\n");
// Serial.print("X-axis: ");
// Serial.print(analogRead(X_pin));
// Serial.print("\n");
// Serial.print("Y-axis: ");
// Serial.println(analogRead(Y_pin));
// Serial.print("\n");
// Serial.print("Slicer: ");
// Serial.print(analogRead(Slicer_pin));
// Serial.print("\n\n");
// delay(500);
