#include <LiquidCrystal_I2C.h>
#include <Wire.h> 

const int trigPin = 5;
const int echoPin = 18;

// Pines de LEDs y buzzer
const int ledGreen = 14;  
const int ledYellow = 27; 
const int ledRed = 26;   
const int buzzer = 25;   

const int lcdColumns = 16;
const int lcdRows = 2;

String estado[] = {"Bajo", "Medio", "Alto"};

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(buzzer, OUTPUT);

  lcd.init();                  
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("BIENVENIDO");
  lcd.setCursor(0, 1);
  lcd.print("Smart Limnograph");
  delay(3000);
  lcd.clear();  
  // Inicia la comunicación serial a 9600 baudios
  Serial.begin(9600);
  delay(2000);
}

void loop() {
  // Medir distancia
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = (duration * 0.034) / 2;

  // Mostrar distancia en la consola serial
  lcd.setCursor(0, 0);
  lcd.print(F("Distancia:"));
  lcd.print(distance);
  lcd.print("cm");

  // Control de LEDs y buzzer
  if (distance > 30) {
    // Nivel bajo (más de 30 cm)
    noTone(buzzer);
    digitalWrite(ledGreen, HIGH);  // LED verde encendido
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
    digitalWrite(buzzer, LOW);
    lcd.setCursor(0, 1);
    lcd.print(F("Nivel:"));
    lcd.print(estado[0]);
    lcd.print("     ");
  } else if (distance > 10 && distance <= 30) {
    // Nivel medio (entre 10 y 30 cm)
    noTone(buzzer);
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, HIGH);  // LED amarillo encendido
    digitalWrite(ledRed, LOW);
    digitalWrite(buzzer, LOW);
    lcd.setCursor(0, 1);
    lcd.print(F("Nivel:"));
    lcd.print(estado[1]);
    lcd.print("     ");
  } else {
    // Nivel alto (menos de 10 cm)
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, HIGH);  // LED rojo encendido
    tone(buzzer, 1000);  // Genera un tono de 1 kHz
    lcd.setCursor(0, 1);
    lcd.print(F("Nivel:"));
    lcd.print(estado[2]);
    lcd.print("     ");
  }
  delay(3000);  // Espera 1 segundo antes de volver a medir
}