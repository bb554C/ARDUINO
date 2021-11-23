#include <Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2); //LCD Display Setting
Servo servoTop1; //Top side servo 1
Servo servoTop2; //Top side servo 2
Servo servoTop3; //Top side servo 3
Servo servoBot; //Bottom side servo

const int IR = A0; //Set pin for the IR sensor
const int S1 = 1; //Set Switch 1 pin
const int S2 = 2; //Set Switch 2 pin
const int S3 = 3; //Set Switch 3 pin

int buttonState1 =0; //variable for reading the state of Switch 1
int buttonState2 =0; //variable for reading the state of Switch 2
int buttonState3 =0; //variable for reading the state of Switch 3
int sensVal = 0; // variable for reading the ir sensor
void setup() {

Serial.begin(9600); //Initiate Serial Monitor

//---Servo Startup Settings---/
  servoTop1.attach(9); //Set servo top1 to pin 9
  servoTop2.attach(10); //Set servo top2 to pin 10
  servoTop3.attach(11); //Set servo top3 to pin 11
  servoBot.attach(6); //Set servo bot to pin 6
  
  servoTop1.write(90); //Set to 90 degrees
  servoTop2.write(90); //Set to 90 degrees
  servoTop3.write(90); //Set to 90 degrees
  servoBot.write(0); //Set to 0 degrees
//---Servo Startup end---/

//---Initialize Switches and Sensor---/
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(IR, INPUT);

  digitalWrite(S1, HIGH);
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  
//---LCD Initialize---/
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("System Start");
//---LCD Initialize end---/
  

}

void loop() {

  sensVal = analogRead(IR); //read the analog value of sensor
  buttonState1 = digitalRead(S1); //read the state of switch 1
  buttonState2 = digitalRead(S2); //read the state of switch 2
  buttonState3 = digitalRead(S3); //read the state of switch 3

  Serial.print("Current Sensor Value: ");
  Serial.print(sensVal);
  Serial.print("|");

  Serial.println("Switch 1 Status: ");
  Serial.print(buttonState1);
  Serial.println("Switch 2 Status: ");
  Serial.print(buttonState2);
  Serial.println("Switch 3 Status: ");
  Serial.print(buttonState3);

  if (sensVal<30) //IF the amount of feeds is not enough
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Not enough");
    lcd.setCursor(1,0);
    lcd.print("Food inside");
  }

  if (sensVal>30) //IF the amount of feeds is sufficient
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Food Status");
    lcd.setCursor(1,0);
    lcd.print("Sufficient");

    if(S1 == LOW)
    {
      servoTop1.write(90); //Keep hole close
    }else if(S1 == HIGH && "If it already feeding time")
    {
      servoTop1.write(45); //Open Hole 45 degrees
    }

    if(S2 == LOW)
    {
      servoTop2.write(90); //Keep hole close
    }else if(S2 == HIGH && "If it already feeding time")
    {
      servoTop2.write(45); //Open Hole 45 degrees
    }

    if(S3 == LOW)
    {
      servoTop3.write(90); //Keep hole close
    }else if(S1 == HIGH && "If it already feeding time")
    {
      servoTop3.write(45); //Open Hole 45 degrees
    }

    //This can be switched to while statement instead of IF statement 
    //depends on you      
    if ("If weight is equal to and it's  feeding time")
    {
      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Feeding Time");
      
      servoBot.write(90);//Open hole
    }else if ("If weight is equal to and it's not yet feeding time")
    {
      servoBot.write(0);
    }



    
  }


}
