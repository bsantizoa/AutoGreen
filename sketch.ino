
// ============================================================
// AutoGreen - Sistema de Monitoreo y Control de Invernadero
// Archivo   : sketch.ino
// Autor     : Byron Efrain Santizo Arana / Jefferson Alberto Soto Chigua
// Curso     : Arquitectura de Computadoras I
// Plataforma: Arduino UNO (ATmega328P, 16 MHz)
// Simulador : Wokwi (https://wokwi.com/projects/465771822219307009)
// ============================================================
// Librería para manejar el sensor DHT22
#include <DHT.h>

// Librería para controlar la pantalla LCD 
#include <LiquidCrystal_I2C.h>

// Pin donde está conectado el sensor DHT22
#define DHTPIN 2

// Tipo de sensor DHT utilizado
#define DHTTYPE DHT22

// Creación del objeto DHT
DHT dht(DHTPIN, DHTTYPE);

// Configuración de la pantalla LCD
// Dirección I2C: 0x27, 16 columnas y 2 filas
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin analógico donde está conectado el sensor de humedad
const int pinSuelo = A1;

// Definición de pines para actuadores
const int ventilador = 4;   // Ventilación
const int bomba = 5;        // Riego
const int calefactor = 6;   // Calefacción
const int luzAux = 7;       // Iluminación auxiliar

// Pin analógico donde está conectado el sensor de luz (LDR)
const int pinLDR = A0;

void setup() {

  // Configuración de los actuadores
  pinMode(ventilador, OUTPUT);
  pinMode(bomba, OUTPUT);
  pinMode(calefactor, OUTPUT);
  pinMode(luzAux, OUTPUT);

  // Inicialización de comunicación serial
  Serial.begin(9600);

  // Inicialización del sensor DHT22
  dht.begin();

  // Inicialización de la pantalla LCD
  lcd.init();
  lcd.backlight();
}

void loop() {

  // Lectura de temperatura en grados Celsius
  float temperatura = dht.readTemperature();

  // Lectura de humedad relativa del ambiente
  float humedad = dht.readHumidity();

  // Lectura del sensor de humedad del suelo
  int sueloRaw = analogRead(pinSuelo);

  // Conversión de la lectura a porcentaje (0 - 100%)
  int humedadSuelo = map(sueloRaw, 0, 1023, 0, 100);

  // Lectura del sensor LDR (luminosidad)
  int ldrRaw = analogRead(pinLDR);

  // Conversión aproximada a un rango de lux
  int lux = map(ldrRaw, 0, 1023, 0, 1000);

  //Si la temperatura supera los 30°C, se activa el ventilador.
  if (temperatura > 30) {
    digitalWrite(ventilador, HIGH);
  } else {
    digitalWrite(ventilador, LOW);
  }

  //Si la temperatura baja de 15°C, se activa el calefactor.
  if (temperatura < 15) {
    digitalWrite(calefactor, HIGH);
  } else {
    digitalWrite(calefactor, LOW);
  }

  //Si la humedad del suelo es menor al 40% se activa la bomba de agua.
  if (humedadSuelo < 40) {
    digitalWrite(bomba, HIGH);
  } else {
    digitalWrite(bomba, LOW);
  }

  //Si la humedad del suelo es muy baja (menos del 20%), se enciende la luz auxiliar.
  if (humedadSuelo < 20) {
      digitalWrite(luzAux, HIGH);
    } else {
      digitalWrite(luzAux, LOW);
    }

  //Si la iluminación es menor a 200 lux se enciende la luz auxiliar.
   
  if (lux < 200) {
    digitalWrite(luzAux, HIGH);
  } else {
    digitalWrite(luzAux, LOW);
  }

  // Limpia la pantalla LCD antes de actualizar datos
  lcd.clear();

  // Muestra la temperatura en la primera fila
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temperatura);

  // Se utiliza para poder ver en pantalla LCD el valor de la humedad
  // lcd.setCursor(0, 1);
  // lcd.print("Suelo:");
  // lcd.print(humedadSuelo);

  // Muestra el nivel de iluminación en la segunda fila
  lcd.setCursor(0, 1);
  lcd.print("Luz:");
  lcd.print(lux);

  // Espera 1 segundo antes de la siguiente lectura
  delay(1000);
}