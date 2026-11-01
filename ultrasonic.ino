#define TRIG_PIN 5
#define ECHO_PIN 18
#define BUZZER_PIN 25

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  delay(1000);
}

void loop() {

  // Send ultrasonic pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  // Receive echo
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    Serial.println("No obstacle / No echo");
    digitalWrite(BUZZER_PIN, LOW);
  }
  else {

    float distance = duration * 0.0343 / 2;

    if (distance > 400) {
      Serial.println("Invalid reading");
      digitalWrite(BUZZER_PIN, LOW);
    }

    else {

      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");

      // Obstacle detection
      if (distance <= 50) {

        Serial.println("!!! OBSTACLE DETECTED !!!");

        digitalWrite(BUZZER_PIN, HIGH);
      }

      else if (distance <= 100) {

        Serial.println("CAUTION");

        digitalWrite(BUZZER_PIN, LOW);
      }

      else {

        Serial.println("PATH CLEAR");

        digitalWrite(BUZZER_PIN, LOW);
      }
    }
  }

  delay(500);
}