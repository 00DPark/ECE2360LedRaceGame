#include <LiquidCrystal.h>

const int xPin1 = A0;  //the VRX attach to A0
const int yPin1 = A1;  //the VRY attach to A1
const int swPin1 = 0;  //the SW attach to D0

const int xPin2 = A2;  //the VRX attach to A2
const int yPin2 = A3;  //the VRY attach to A3
const int swPin2 = 1;  //the SW attach to D1
const int buzzerPin=7; //buzzer is attached to D7

const int DEBUG=1; //debug variable
//AUSTIN TEST
LiquidCrystal lcd(17, 16, 3, 4, 5, 6);  // initialize the LCD1602
void setup()
{
  pinMode(swPin1, INPUT_PULLUP);  //set the SW pin to INPUT_PULLUP for setting up joystick 1
  pinMode(swPin2, INPUT_PULLUP);  //set the SW pin to INPUT_PULLUP for setting up joystick 2
  pinMode(buzzerPin, OUTPUT); //initialize the buzzerpin as output
  lcd.begin(16, 2);    // set up the LCD's number of columns and rows
  Serial.begin(9600);
}

void displayLCD()
{
  // DISPLAY DATA
  lcd.setCursor(0, 0);
  lcd.print("...");
}

void printJoystick()
{
  Serial.print("X1: ");
  Serial.print(analogRead(xPin1));  // print the value of VRX in DEC
  Serial.print("|Y1: ");
  Serial.print(analogRead(yPin1));  // print the value of VRX in DEC
  Serial.print("|Z1: ");
  Serial.println(digitalRead(swPin1));   // print the value of SW

  Serial.print("X2: ");
  Serial.print(analogRead(xPin2));  // print the value of VRX in DEC
  Serial.print("|Y2: ");
  Serial.print(analogRead(yPin2));  // print the value of VRX in DEC
  Serial.print("|Z2: ");
  Serial.println(digitalRead(swPin2));   // print the value of SW
  delay(100);
}

void loop()
{
  if(DEBUG)
  {
    printJoystick();
  }
}
