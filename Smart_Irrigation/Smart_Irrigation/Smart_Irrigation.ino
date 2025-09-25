#define BLYNK_TEMPLATE_ID "TMPL5uB1wguly"
#define BLYNK_TEMPLATE_NAME "Smart Irrigation"
#define BLYNK_AUTH_TOKEN "7TSaVSrAUf79PtkAjKbD1OxR260R_XB0"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiS3.h>
#include <DHT.h>
#include <Blynk.h>
#include <BlynkSimpleWiFi.h>
#include <ThingSpeak.h>

// WiFi credentials
char ssid[] = "Monsieur's Galaxy A55 5G";
char pass[] = "ojmn4004";

// ThingSpeak
WiFiClient client;
unsigned long ts_channel_id = 2990424;
const char* ts_api_key = "38QNBZGCIB3F9I3D";

// LCD (16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin setup
#define soilPin A0
#define dhtPin 2
#define ldrPin 3
#define relayPin 7

#define DHTTYPE DHT11
DHT dht(dhtPin, DHTTYPE);

int mode = 0;             // 0 = Auto, 1 = Manual
int manualState = HIGH;   // HIGH = OFF
BlynkTimer timer;

void sendToBlynk() {
  int soilRaw = analogRead(soilPin);
  int moisture = map(soilRaw, 1023, 0, 0, 100);
  moisture = constrain(moisture, 0, 100);

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int light = digitalRead(ldrPin);

  Blynk.virtualWrite(V0, moisture);
  Blynk.virtualWrite(V3, temp);
  Blynk.virtualWrite(V4, hum);
  Blynk.virtualWrite(V5, light);
}

void sendToThingSpeak() {
  int soilRaw = analogRead(soilPin);
  int moisture = map(soilRaw, 1023, 0, 0, 100);
  moisture = constrain(moisture, 0, 100);

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int light = digitalRead(ldrPin);

  if (isnan(temp) || isnan(hum)) {
    Serial.println("DHT read failed. Skipping ThingSpeak update.");
    return;
  }

  ThingSpeak.setField(2, moisture);
  ThingSpeak.setField(3, temp);
  ThingSpeak.setField(4, hum);
  ThingSpeak.setField(5, light);

  int status = ThingSpeak.writeFields(ts_channel_id, ts_api_key);
  if (status == 200) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.print("ThingSpeak Error: ");
    Serial.println(status);
  }
}

BLYNK_WRITE(V1) {
  manualState = param.asInt() == 1 ? LOW : HIGH;
}

BLYNK_WRITE(V2) {
  mode = param.asInt();
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(soilPin, INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);

  dht.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  ThingSpeak.begin(client);

  timer.setInterval(2000L, sendToBlynk);
  timer.setInterval(15000L, sendToThingSpeak);
}

void loop() {
  Blynk.run();
  timer.run();

  int soilRaw = analogRead(soilPin);
  int moisture = map(soilRaw, 1023, 0, 0, 100);
  moisture = constrain(moisture, 0, 100);

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int light = digitalRead(ldrPin);

  Serial.print("Moisture: "); Serial.print(moisture); Serial.print("%  ");
  Serial.print("Temp: "); Serial.print(temp); Serial.print("°C  ");
  Serial.print("Hum: "); Serial.print(hum); Serial.print("%  ");
  Serial.print("Light: "); Serial.println(light == 0 ? "Dark" : "Bright");

  lcd.setCursor(0, 0);
  lcd.print("M:"); lcd.print(moisture); lcd.print("% ");
  lcd.print("T:"); lcd.print((int)temp); lcd.print("C ");
  lcd.print("H:"); lcd.print((int)hum); lcd.print("% ");

  lcd.setCursor(0, 1);
  if (mode == 1) {
    digitalWrite(relayPin, manualState);
    lcd.print("MANUAL ");
    lcd.print(manualState == LOW ? "PUMP ON " : "PUMP OFF");
  } else {
    if (moisture < 30 && temp > 25 && hum < 50 && light == 1) {
      digitalWrite(relayPin, LOW);
      lcd.print("AUTO   PUMP ON ");
      delay(2000);
      digitalWrite(relayPin, HIGH);
    } else {
      digitalWrite(relayPin, HIGH);
      lcd.print("AUTO   PUMP OFF");
    }
  }

  delay(2000);
}
