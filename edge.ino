#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>

#define DHTPIN 6
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

const int trigPin = 13;
const int echoPin = 5;
const int LDR_PIN = A0;

const int LED_VERMELHO = 2;
const int LED_AMARELO = 3;
const int LED_VERDE = 4;

const float GAMMA = 0.7;
const float RL10 = 50;

float duration = 0;
float distancia = 0;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LDR_PIN, INPUT);

  dht.begin();
  lcd.begin(16, 2);
  
  delay(1000);
  Serial.begin(9600);
}

void loop() {
  float temp = dht.readTemperature();
  float umid = dht.readHumidity();
  int ldr_valor = analogRead(LDR_PIN);
  float voltage = ldr_valor / 1024.0 * 5.0;
  float resistance = 2000.0 * voltage / (5.0 - voltage);
  int lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));

  if (isnan(temp) || isnan(umid)) {
    Serial.println("Sensor DHT22 inativo");
    return;
  }


  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  
  duration = pulseIn(echoPin, HIGH);
  distancia = duration * 0.034 / 2;

  Serial.print("Temperatura: ");
  Serial.print(temp);
  Serial.println(" °C");
  Serial.print(" Umidade: ");
  Serial.print(umid);
  Serial.println(" %");
  Serial.print(" Nível de Luz: ");
  Serial.print(lux);
  Serial.println(" lux");
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Umid: ");
  lcd.print(umid);
  lcd.print(" %");
  delay(2000);
  lcd.clear();
  
  lcd.setCursor(0, 0); 
  lcd.print("Luz: ");
  lcd.print(lux);
  lcd.print(" lux");

  lcd.setCursor(0, 1);
  lcd.print("Dist: ");
  lcd.print(distancia);
  lcd.print(" cm");
  delay(2000);
  lcd.clear();


  if (temp > 40 || umid > 50 || lux < 110) {
    digitalWrite(LED_VERMELHO, HIGH);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERDE, LOW);
  } else {
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERDE, HIGH);
  }

  if (digitalRead(LED_VERDE) == HIGH && distancia <= 10) {
    digitalWrite(LED_AMARELO, HIGH);
  } else {
    digitalWrite(LED_AMARELO, LOW);
  }

  delay(500);
}