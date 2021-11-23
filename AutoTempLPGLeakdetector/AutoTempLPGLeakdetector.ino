#include <DHT.h>
DHT dhtSensor(2, DHT22);
int gasSensor = 3;
int autoSwitch = 4;
int buzzer = 5;
int counter = 0;
int onCounter = 0;

void setup() {
  Serial.begin(9600);
  pinMode(6, INPUT);
  pinMode(2, INPUT);
  pinMode(gasSensor, INPUT);
  pinMode(autoSwitch, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(autoSwitch, LOW);
  dhtSensor.begin();
  int counter = 0;
}

void loop() {
  int lpg = digitalRead(gasSensor);
  float temp = dhtSensor.readTemperature();
  if (lpg == 0)
  {
    while (lpg == 0)
    {
      LPGAlert();
      AUTOMATICSWITCH(HIGH);
      lpg = digitalRead(gasSensor);
      delay(5000);
    }
    AUTOMATICSWITCH(LOW);
  }
  if (temp < 40)
  {
    AUTOMATICSWITCH(LOW);
  }
  else if (temp >= 40 && temp < 50)
  {
    if (counter >= 10)
    {
      Temp40Alert();
      AUTOMATICSWITCH(HIGH);
    }
  }
  else if (temp >= 50)
  {
    if (counter >= 10)
    {
      Temp50Alert();
      AUTOMATICSWITCH(HIGH);
    }
  }
  Serial.print("temp: ");
  Serial.println(temp);
  Serial.print("counter: ");
  Serial.println(counter);
  Serial.print("onCounter: ");
  Serial.println(onCounter);
  Serial.print("switch: ");
  Serial.println(digitalRead(6));
  delay(60000);
}

void AUTOMATICSWITCH(int switchvar)
{
  digitalWrite(autoSwitch, switchvar);
  Serial.print("switch: ");
  Serial.println(digitalRead(6));
  if (switchvar == HIGH)
  {
    counter = 0;
    onCounter = onCounter + 1;
    Serial.println("SWITCH ON");
  }
  else  if (switchvar == LOW)
  {
    counter = counter + 1;
    onCounter = 0;
    Serial.println("SWITCH OFF");
  }
}

void LPGAlert()
{
  tone(buzzer, 1000, 750);
  delay(500);
  tone(buzzer, 500, 750);
  delay(500);
  tone(buzzer, 1000, 750);
  delay(500);
  tone(buzzer, 500, 750);
  delay(500);
}

void Temp40Alert()
{
  tone(buzzer, 500, 200);
  delay(1000);
  tone(buzzer, 500, 200);
  delay(1000);
  tone(buzzer, 500, 200);
  delay(1000);
  tone(buzzer, 500, 200);
  delay(5000);
}

void Temp45Alert()
{
  tone(buzzer, 500, 200);
  delay(1000);
  tone(buzzer, 500, 200);
  delay(1000);
  tone(buzzer, 500, 200);
  delay(1000);
  tone(buzzer, 500, 200);
  delay(1000);
  tone(buzzer, 1000, 200);
  delay(5000);
}

void Temp50Alert()
{
  tone(buzzer, 500, 200);
  delay(1000);
  tone(buzzer, 500, 200);
  delay(1000);
  tone(buzzer, 500, 200);
  delay(1000);
  tone(buzzer, 500, 200);
  delay(1000);
  tone(buzzer, 500, 200);
  delay(5000);
}
