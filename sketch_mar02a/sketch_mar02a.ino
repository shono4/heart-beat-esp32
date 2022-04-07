void setup() { 
  pinMode(35, INPUT);
  Serial.begin(74880);
  // put your setup code here, to run once:
}

void loop() {
  int dac;
  float volt;

  dac = analogRead(35);
  volt = dac * 3.3 / 4096;
  Serial.println(dac);
  delay(5);
  // put your main code here, to run repeatedly:
}
