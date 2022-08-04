#include <Time.h>
#include <TimeLib.h>
#include <LiquidCrystal.h>
//LCD pin to Arduino
const int pin_RS = 8; 
const int pin_EN = 9; 
const int pin_d4 = 4; 
const int pin_d5 = 5; 
const int pin_d6 = 6; 
const int pin_d7 = 7; 
const int pin_BL = 10; 
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);


time_t fecha;                         // Declaramos la variable del tipo time_t
String tiempoReset = "00:00:10";      // Tiempo base para el conteo
const int ledPIN = 3;                 // Pin para activar relé
String estado;                        // Estado de la salida a activar

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2); // Indicar a la libreria que tenemos conectada una pantalla de 16x2
  lcd.clear();      // Mover el cursor a la primera posición de la pantalla (0, 0)
  lcd.print("INICIO TIMER");

  setTime(0, 0, 0, 12, 12, 2021);     // Establecemos la fecha
  pinMode(ledPIN , OUTPUT);
  digitalWrite(ledPIN, 1);
  Serial.println("LED ON");

  estado = "ON";
  delay(1000);      // Esperar un segundo
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
  leerTeclado();
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

void leerTeclado() {
  int x;
  x = analogRead (0);
  lcd.setCursor(10,1);
  if (x < 60) { lcd.print ("Right ");}
  else if (x < 200){   lcd.print ("Up    "); }
  else if (x < 400){   lcd.print ("Down  "); }
  else if (x < 600){   lcd.print ("Left  "); }
  else if (x < 800){   lcd.print ("Select"); }
}

/*Fuente Base 
#include <LiquidCrystal.h>
//LCD pin to Arduino
const int pin_RS = 8; 
const int pin_EN = 9; 
const int pin_d4 = 4; 
const int pin_d5 = 5; 
const int pin_d6 = 6; 
const int pin_d7 = 7; 
const int pin_BL = 10; 
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Electropeak.com");
  lcd.setCursor(0,1);
  lcd.print("Press Key:");
}          
void loop() {
  int x;
  x = analogRead (0);
  lcd.setCursor(10,1);
  if (x < 60) { lcd.print ("Right ");}
  else if (x < 200){   lcd.print ("Up    "); }
  else if (x < 400){   lcd.print ("Down  "); }
  else if (x < 600){   lcd.print ("Left  "); }
  else if (x < 800){   lcd.print ("Select"); }
}
*/
