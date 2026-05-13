#define EM_PIN 6  // choisir une broche libre

void setup() {
  pinMode(EM_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(EM_PIN, HIGH);  // aimant ON
  Serial.println("Allumé");
  delay(1000);
  digitalWrite(EM_PIN, LOW);   // aimant OFF
  Serial.println("Éteint");
  delay(1000);
}