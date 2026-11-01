#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// ---------- Wi-Fi ----------
#define WIFI_SSID "Redmi 10A"
#define WIFI_PASSWORD "Satara@12"

// ---------- Firebase ----------
#define API_KEY "AIzaSyB5ybpp85_TOk59kP0syNzNUdmRyJLjNnE"
#define DATABASE_URL "https://jaldrishti-iot-default-rtdb.asia-southeast1.firebasedatabase.app/"

// ---------- HC-SR04 ----------
#define TRIG_PIN 5
#define ECHO_PIN 18

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected!");
  Serial.println(WiFi.localIP());

  // Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("Firebase started!");
}

void loop() {

  // Ultrasonic measurement
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    Serial.println("No obstacle / No echo");
    delay(1000);
    return;
  }

  float distance = duration * 0.0343 / 2;

  if (distance > 400) {
    Serial.println("Invalid reading");
    delay(1000);
    return;
  }

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Send distance to Firebase
  if (Firebase.ready()) {

    if (Firebase.RTDB.setFloat(&fbdo, "/ultrasonic/distance", distance)) {
      Serial.println("Firebase: Distance uploaded!");
    } 
    else {
      Serial.print("Firebase Error: ");
      Serial.println(fbdo.errorReason());
    }
  }

  delay(1000);
}