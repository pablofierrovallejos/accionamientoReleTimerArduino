#include <Time.h>
#include <TimeLib.h>
#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>

time_t fecha;                         // Declaramos la variable del tipo time_t
String tiempoReset = "00:20:00";
const int ledPIN = 9;
LiquidCrystal_I2C lcd(0x27, 16, 2);
String estado;
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2); // Indicar a la libreria que tenemos conectada una pantalla de 16x2
  lcd.init();       // Inicializar el LCD
  lcd.backlight();  //Encender la luz de fondo.
  lcd.clear();      // Mover el cursor a la primera posición de la pantalla (0, 0)
  lcd.print("INICIO TIMER");

  setTime(0, 0, 0, 12, 12, 2021);     // Establecemos la fecha
  pinMode(ledPIN , OUTPUT);
  digitalWrite(ledPIN, 1);
  Serial.println("LED ON");
  estado = "ON";
  
  delay(1000);  // Esperar un segundo
}

void loop() {
  fecha = now();
  String tiempoActual = dosdigit(hour(fecha)) + ":" + dosdigit(minute(fecha)) + ":" + dosdigit(second(fecha));  // Imprimimos Tiempo actual
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PFIERRO TIMER");

  lcd.setCursor(0, 1);
  lcd.print(tiempoActual + " " + estado);
  Serial.println(tiempoActual);

  delay(1000);
  reseteaTiempo(tiempoActual);
}
void reseteaTiempo(String tiempoActual) {
  if (tiempoActual == tiempoReset) {
    setTime(0, 0, 0, 12, 12, 2021);     // Establecemos la fecha
    intercambiaEstadoLed();
  }
}
void intercambiaEstadoLed() {
  if (digitalRead(ledPIN)) {
    estado = "OFF";
    digitalWrite(ledPIN, 0);
    Serial.println("LED OFF");
  } else {
    estado = "ON";
    digitalWrite(ledPIN, 1);
    Serial.println("LED ON");
  }
}

String dosdigit(int numero) {
  String myString = ""; //
  if (numero >= 0 && numero < 10) {
    myString = "0" + String(numero);
  } else {
    myString = String(numero);
  }
  return myString;
}
