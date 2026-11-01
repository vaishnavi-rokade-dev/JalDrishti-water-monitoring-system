#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// WiFi
#define WIFI_SSID "Redmi 10A"
#define WIFI_PASSWORD "Satara@12"

// Firebase
#define DATABASE_URL "https://jaldrishti-iot-default-rtdb.asia-southeast1.firebasedatabase.app"

// DS18B20
#define TEMP_PIN 4

// HC-SR04
#define TRIG_PIN 5
#define ECHO_PIN 18

// Turbidity
#define TURBIDITY_PIN 34

OneWire oneWire(TEMP_PIN);
DallasTemperature tempSensor(&oneWire);

void setup()
{
  Serial.begin(115200);

  tempSensor.begin();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(TURBIDITY_PIN, INPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");
  Serial.println(WiFi.localIP());
}

void loop()
{
  // Temperature
  tempSensor.requestTemperatures();
  float temperature = tempSensor.getTempCByIndex(0);

  // Ultrasonic
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  float distance = -1;

  if (duration > 0)
  {
    distance = duration * 0.0343 / 2;

    if (distance > 400)
    {
      distance = -1;
    }
  }

  // Turbidity
  int turbidityValue = analogRead(TURBIDITY_PIN);

  Serial.println("-------------");
  Serial.print("Temperature: ");
  Serial.println(temperature);

  Serial.print("Distance: ");
  Serial.println(distance);

  Serial.print("Turbidity: ");
  Serial.println(turbidityValue);

  if (WiFi.status() == WL_CONNECTED)
  {
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;

    // Temperature Upload
    String tempURL =
      String(DATABASE_URL) + "/temperature.json";

    http.begin(client, tempURL);
    http.addHeader("Content-Type", "application/json");
    http.PUT(String(temperature));
    http.end();

    // Distance Upload
    if(distance > 0)
    {
      String distanceURL =
      String(DATABASE_URL) +
      "/ultrasonic/distance.json";

      http.begin(client, distanceURL);
      http.addHeader("Content-Type", "application/json");
      http.PUT(String(distance));
      http.end();
    }

    // Turbidity Upload
    String turbidityURL =
      String(DATABASE_URL) + "/turbidity.json";

    http.begin(client, turbidityURL);
    http.addHeader("Content-Type", "application/json");
    http.PUT(String(turbidityValue));
    http.end();
  }

  delay(2000);
}