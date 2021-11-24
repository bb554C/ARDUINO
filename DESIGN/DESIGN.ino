#include <HX711.h>
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
LiquidCrystal_I2C lcd(0x27, 16, 2); //LCD Display Setting
Servo servoTop1; //Top side servo 1
Servo servoTop2; //Top side servo 2
Servo servoTop3; //Top side servo 3
Servo servoBot; //Bottom side servo
HX711 scale1;
HX711 scale2;
HX711 scale3;

//Global Variables
int buttonState1 = 0; //variable for reading the state of Switch 1
int buttonState2 = 0; //variable for reading the state of Switch 2
int buttonState3 = 0; //variable for reading the state of Switch 3
int sensVal = 0; // variable for reading the ir sensor
float weight = 100; //Default Grams
int hour = 00; //Default Hour
int mins = 00; //Default Mins
int secs = 00; //Default Secs
int timerh1 = 8; //Timer1 Hour Default
int timerm1 = 00; //Timer1 Minutes
int timerh2 = 20; //Timer1 Hour Default
int timerm2 = 00; //Timer1 Minutes

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
  //Initialize pcf8574 I/O bit extenders
  pcf1.pinMode(P0, INPUT);
  pcf1.pinMode(P1, INPUT);
  pcf1.pinMode(P2, INPUT);
  pcf1.pinMode(P3, INPUT);
  pcf1.pinMode(P4, INPUT);
  pcf1.pinMode(P6, INPUT);
  pcf1.pinMode(P7, OUTPUT);
  pcf1.begin();

  //Initialize HX711
  scale1.begin(weightSens1, sck);
  scale2.begin(weightSens1, sck);
  scale3.begin(weightSens1, sck);
  //---LCD Initialize---/
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Start");
  //---LCD Initialize end---/
  pcf1.digitalWrite(P7, HIGH);
  delay(5000); //Delay for initialization of electronics
}

void loop() {
  int A = pcf1.digitalRead(P0);
  int B = pcf1.digitalRead(P1);
  int C = pcf1.digitalRead(P2);
  int D = pcf1.digitalRead(P3);
  int E = pcf1.digitalRead(P4);
  if (A == 0 && B == 0 && C == 0 && D == 0 && E == 1)
  {
    Weight1Down();
    DisplayFeedLimit();
  }
  else if (A == 0 && B == 0 && C == 0 && D == 1 && E == 0)
  {
    Weight1Up();
    DisplayFeedLimit();
  }
  else if (A == 0 && B == 0 && C == 0 && D == 1 && E == 1)
  {
    Weight10Down();
    DisplayFeedLimit();
  }
  else if (A == 0 && B == 0 && C == 1 && D == 0 && E == 0)
  {
    Weight10Up();
    DisplayFeedLimit();
  }
  else if (A == 0 && B == 0 && C == 1 && D == 0 && E == 1)
  {
    feeder2ndMinDown();
    DisplayTimer2();
  }
  else if (A == 0 && B == 0 && C == 1 && D == 1 && E == 0)
  {
    feeder2ndMinUp();
    DisplayTimer2();
  }
  else if (A == 0 && B == 0 && C == 1 && D == 1 && E == 1)
  {
    feeder2ndHrDown();
    DisplayTimer2();
  }
  else if (A == 0 && B == 1 && C == 0 && D == 0 && E == 0)
  {
    feeder2ndHrUp();
    DisplayTimer2();
  }
  else if (A == 0 && B == 1 && C == 0 && D == 0 && E == 1)
  {
    feeder1stMinDown();
    DisplayTimer1();
  }
  else if (A == 0 && B == 1 && C == 0 && D == 1 && E == 0)
  {
    feeder1stMinUp();
    DisplayTimer1();
  }
  else if (A == 0 && B == 1 && C == 0 && D == 1 && E == 1)
  {
    feeder1stHrDown();
    DisplayTimer1();
  }
  else if (A == 0 && B == 1 && C == 1 && D == 0 && E == 0)
  {
    feeder1stHrUp();
    DisplayTimer1();
  }
  else if (A == 0 && B == 1 && C == 1 && D == 0 && E == 1)
  {
    TimeMin1Down();
    DisplayTime(); //Display Time
  }
  else if (A == 0 && B == 1 && C == 1 && D == 1 && E == 0)
  {
    TimeMin1Up();
    DisplayTime(); //Display Time

  }
  else if (A == 0 && B == 1 && C == 1 && D == 1 && E == 1)
  {
    TimeHr1Down();
    DisplayTime(); //Display Time
  }
  else if (A == 1 && B == 0 && C == 0 && D == 0 && E == 0)
  {
    TimeHr1Up();
    DisplayTime(); //Display Time
  }
  else
  {
    Time(); //Time add 1 sec
    FeedTimeCheck();
    DisplayTime(); //Display Time
  }
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
  Serial.println();
}
/*Weight Methods*/
void Weight1Down()
{
  if (weight > 1)
  {
    weight--;
  }
}
void Weight1Up()
{
  if (weight < 200)
  {
    weight++;
  }
}
void Weight10Down()
{
  if (weight > 10)
  {
    weight = weight - 10;
  }
}
void Weight10Up()
{
  if (weight <= 190)
  {
    weight = weight + 10;
  }
}
/*SetFeeder2 Methods*/
void feeder2ndMinDown()
{
  if (timerm2 == 0)
  {
    timerm2 = 59;
  }
  else
  {
    timerm2--;
  }
}
void feeder2ndMinUp()
{
  if (timerm2 == 59)
  {
    timerm2 = 0;
  }
  else
  {
    timerm2++;
  }
}
void feeder2ndHrDown()
{
  if (timerh2 == 0)
  {
    timerh2 = 23;
  }
  else
  {
    timerh2--;
  }
}
void feeder2ndHrUp()
{
  if (timerh2 == 23)
  {
    timerh2 = 0;
  }
  else
  {
    timerh2++;
  }
}
/*SetFeeder1 Methods*/
void feeder1stMinDown()
{
  if (timerm1 == 0)
  {
    timerm1 = 59;
  }
  else
  {
    timerm1--;
  }
}
void feeder1stMinUp()
{
  if (timerm1 == 59)
  {
    timerm1 = 0;
  }
  else
  {
    timerm1++;
  }
}
void feeder1stHrDown()
{
  if (timerh1 == 0)
  {
    timerh1 = 23;
  }
  else
  {
    timerh1--;
  }
}
void feeder1stHrUp()
{
  if (timerh1 == 23)
  {
    timerh1 = 0;
  }
  else
  {
    timerh1++;
  }
}
/*Time Controls Methods*/
void TimeMin1Down()
{
  if (mins == 00)
  {
    mins = 59;
  }
  else
  {
    mins--;
  }
}
void TimeMin1Up()
{
  if (mins == 59)
  {
    mins = 0;
  }
  else
  {
    mins++;
  }
}
void TimeHr1Down()
{
  if (hour == 0)
  {
    hour = 23;
  }
  else
  {
    hour--;
  }
}
void TimeHr1Up()
{
  if (hour == 23)
  {
    hour = 0;
  }
  else
  {
    hour++;
  }
}
/*Time Methods*/
void DisplayTimeTest()
{
  Serial.print(hour);
  Serial.print(":");
  Serial.print(mins);
  Serial.print(":");
  Serial.print(secs);
  Serial.println();
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
/*Feed Display*/
void DisplayFeedLimit()
{
  lcd.setCursor(0, 0);
  lcd.print("Weight Limit");
  lcd.setCursor(0, 1);
  lcd.print(weight);
  lcd.print(" grams");
  Time(); //Add 1 sec
  delay(1000);
}
/*Time Display*/
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
  Time(); //Add 1 sec
  delay(1000);
}
void DisplayTimer1()
{
  lcd.setCursor(0, 0);
  lcd.print("TIMER1");
  lcd.setCursor(0, 1);
  if (timerh1 < 10)
  {
    lcd.print("0");
  }
  lcd.print(timerh1);
  lcd.print(":");
  if (timerm1 < 10)
  {
    lcd.print("0");
  }
  lcd.print(timerm1);
  Time(); //Add 1 sec
  delay(1000);
}
void DisplayTimer2()
{
  lcd.setCursor(0, 0);
  lcd.print("TIMER2");
  lcd.setCursor(0, 1);
  if (timerh2 < 10)
  {
    lcd.print("0");
  }
  lcd.print(timerh2);
  lcd.print(":");
  if (timerm2 < 10)
  {
    lcd.print("0");
  }
  lcd.print(timerm2);
  Time(); //Add 1 sec
  delay(1000);
}

void FeedTimeCheck()
{
  if (hour == timerh1 || hour == timerh2)
  {
    if (mins == timerm1 || mins == timerm2)
    {
      Dispense();
    }
  }
}
/*Servo Methoda*/
void Dispense()
{
  float currentWeight = 0;
  if (buttonState1 == HIGH)
  {
    servoTop1.write(90); //Open Servo
    currentWeight = scale1.get_units();
    while (weight > currentWeight)
    {
      currentWeight = scale1.get_units();
      if (weight >= currentWeight)
      {
        servoTop1.write(0); //Close Servo
      }
    }
  }
  if (buttonState2 == HIGH)
  {
    servoTop2.write(90); //Open Servo
    currentWeight = scale2.get_units();
    while (weight > currentWeight)
    {
      currentWeight = scale2.get_units();
      if (weight >= currentWeight)
      {
        servoTop2.write(0); //Close Servo
      }
    }
  }
  if (buttonState3 == HIGH)
  {
    servoTop3.write(90); //Open Servo
    currentWeight = scale3.get_units();
    while (weight > currentWeight)
    {
      currentWeight = scale3.get_units();
      if (weight >= currentWeight)
      {
        servoTop3.write(0); //Close Servo
      }
    }
  }
  Release();
}

void Release()
{
  servoBot.write(90); //Open Bottom Servo
  Time(); //Add 1 sec to time
  Time(); // Add 1 sec to time
  delay(2000);
  servoBot.write(0); //Close Bottom Servo
}
