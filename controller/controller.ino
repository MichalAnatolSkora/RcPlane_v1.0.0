#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output
const int Slider_pin = 2;

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

void setup()
{
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  Serial.begin(9600);
}

int i = 0;

int xMapped_old = -1;
int yMapped_old = -1;
int sliderRaw_old = -1;

void loop()
{
  int xValue = analogRead(X_pin);
  int yValue = analogRead(Y_pin);
  int sliderValue = analogRead(Slider_pin);

  int xMapped = map(xValue, 0, 1023, 0, 180);
  int yMapped = map(yValue, 0, 1023, 0, 180);

  if (87 <= xMapped && xMapped <= 93)
  {
    xMapped = 90;
  }

  if (87 <= yMapped && yMapped <= 93)
  {
    yMapped = 90;
  }

  Serial.print("X-axis: ");
  Serial.println(xMapped);
  Serial.print("Y-axis: ");
  Serial.println(yMapped);
  Serial.print("sliderValue: ");
  Serial.println(sliderValue);
  Serial.print("\n\n");

  if (xMapped_old != xMapped || yMapped_old != yMapped || sliderRaw_old != sliderValue)
  {
    const int text[3] = {xMapped, yMapped, sliderValue};
    radio.write(&text, sizeof(text));
    xMapped_old = xMapped;
    yMapped_old = yMapped;
    sliderRaw_old = sliderValue;
    Serial.print("sent:");
  }

  delay(50);
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
// Serial.print(analogRead(Slider_pin));
// Serial.print("\n\n");
// delay(500);
