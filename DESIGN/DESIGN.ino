#include <PCF8574.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

//I2C Address Table NXP Semixonductors
//PCF8574P
//0  1  0 || 0 A2 A1 A0
//0  1  0 || 0  0  0  0 = 0x20
//0  1  0 || 0  0  0  1 = 0x21
//0  1  0 || 0  0  1  0 = 0x22
//0  1  0 || 0  0  1  1 = 0x23
//0  1  0 || 0  1  0  0 = 0x24
//0  1  0 || 0  1  0  1 = 0x25
//0  1  0 || 0  1  1  0 = 0x26
//0  1  0 || 0  1  1  1 = 0x27
//PCF8574AP
//0  1  1 || 1 A2 A1 A0
//0  1  1 || 1  0  0  0 = 0x38
//0  1  1 || 1  0  0  1 = 0x39
//0  1  1 || 1  0  1  0 = 0x3A
//0  1  1 || 1  0  1  1 = 0x3B
//0  1  1 || 1  1  0  0 = 0x3C
//0  1  1 || 1  1  0  1 = 0x3D
//0  1  1 || 1  1  1  0 = 0x3E
//0  1  1 || 1  1  1  1 = 0x3F

//Pin Assignment
const int sck = 13; //Set SCK to pin 19/D13
const int weightSens1 = 6; //Set WeightSensor1 to pin 12/D6
const int weightSens2 = 7; //Set WeightSensor1 to pin 13/D7
const int weightSens3 = 8 ;//Set WeightSensor1 to pin 14/D8
const int S1 = 3; //Set Switch 1 pin 5/D3
const int S2 = 4; //Set Switch 2 pin 6/D4
const int S3 = 5; //Set Switch 3 pin 7/D5

//Address Assignment
PCF8574 pcf1(0x20);
PCF8574 pcf2(0x21);
LiquidCrystal_I2C lcd(0x38, 16, 2); //LCD Display Setting
Servo servoTop1; //Top side servo 1
Servo servoTop2; //Top side servo 2
Servo servoTop3; //Top side servo 3
Servo servoBot; //Bottom side servo

//Global Variables
int buttonState1 = 0; //variable for reading the state of Switch 1
int buttonState2 = 0; //variable for reading the state of Switch 2
int buttonState3 = 0; //variable for reading the state of Switch 3
int sensVal = 0; // variable for reading the ir sensor
int weight = 100; //Default Grams
int hour = 00; //Default Hour
int mins = 00; //Default Mins
int secs = 00; //Default Secs

void setup() {
  Serial.begin(9600); //Initiate Serial Monitor
  //---Servo Startup Settings---/
  servoTop1.attach(9); //Set servo top1 to pin 15/D9
  servoTop2.attach(10); //Set servo top2 to pin 16/D10
  servoTop3.attach(11); //Set servo top3 to pin 17/D11
  servoBot.attach(12); //Set servo bottom to pin 18/D12

  servoTop1.write(90); //Set to 90 degrees
  servoTop2.write(90); //Set to 90 degrees
  servoTop3.write(90); //Set to 90 degrees
  servoBot.write(0); //Set to 0 degrees
  //---Servo Startup end---/

  //---Initialize Switches and Sensor---/
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);

  //---LCD Initialize---/
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Start");
  //---LCD Initialize end---/
}

void loop() {
  MonitorTest(); //Monitor the values
  Time(); //Time counter
  DisplayTime(); //Display Time
  delay(1000);
}

void MonitorTest()
{
  Serial.print("Sensor: ");
  Serial.println(sensVal);
  Serial.print("Button 1: ");
  Serial.println(buttonState1);
  Serial.print("Button 2: ");
  Serial.println(buttonState2);
  Serial.print("Button 3: ");
  Serial.println(buttonState3);
}

void Time()
{
  secs++;
  if (secs == 60)
  {
    mins++;
    secs = 0;
  }
  if (mins == 60)
  {
    hour++;
    mins = 0;
  }
  if (hour == 24)
  {
    hour = 0;
  }
}

void DisplayTime()
{
  if (hour < 10)
  {
    lcd.print("0");
  }
  lcd.print(hour);
  lcd.print(":");
  if (mins < 10)
  {
    lcd.print("0");
  }
  lcd.print(mins);
}
void Dispense()
{

}
