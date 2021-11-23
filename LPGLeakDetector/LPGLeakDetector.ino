int gasSensor = 3;
int buzzer = 5;

void setup() {
  Serial.begin(9600);
  pinMode(gasSensor, INPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  int lpg = digitalRead(gasSensor);
  while (lpg == 0)
  {
    LPGAlert();
    lpg = digitalRead(gasSensor);
    delay(5000);
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
