#include <Time.h>
#include <TimeLib.h>
#include <EEPROM.h>
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


time_t fecha;                          // Declaramos la variable del tipo time_t
String tiempoON = "00:00:10";          // Tiempo base para el conteo
String tiempoOFF = "00:00:10";          // Tiempo base para el conteo
const int ledPIN = 3;                  // Pin para activar relé
String estado;                         // Estado de la salida a activar
byte dato;                             // El byte que se lee de memoria para base de tiempo en ON
byte dato2;                            // El byte que se lee de memoria para base de tiempo en OFF
int modoseteo=0;                       // Modo seteo tiempo 1 = seteando por usuario (presionó SET)
byte tiemposeteadoRST = 0;
byte setDuty = 1;                      // Para setear el tiempo de encendido
byte memon =0;
byte memoff =0;
byte cicloSalida = 1;                  //Define el estado actual de la salida
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2); // Indicar a la libreria que tenemos conectada una pantalla de 16x2 (COLUMNAS x FILAS)
  lcd.clear();      // Mover el cursor a la primera posición de la pantalla (0, 0)
  lcd.print("INICIO TIMER");
  Serial.println("INICIO TIMER");
  
  setTime(0, 0, 0, 12, 12, 2021);     // Establecemos la fecha
  pinMode(ledPIN , OUTPUT);
  digitalWrite(ledPIN, 1);
  Serial.println("LED ON");
  estado = "ON";

  memon  = EEPROM.read(0);
  memoff = EEPROM.read(1);
  
  Serial.print("EEPROM ");
  Serial.println(dato);
  tiempoON =   formateaHoraCadena(0,int(memon), 0);
  tiempoOFF =  formateaHoraCadena(0,int(memoff), 0);
  Serial.print("formateaHoraCadena ");
  Serial.println(tiempoON);
  Serial.println(tiempoOFF);

}


void loop() {
  Serial.println("Loop -> ");
  Serial.print("Time On  "); Serial.println(tiempoON);
  Serial.print("Time Off "); Serial.println(tiempoOFF);
  if(modoseteo==0){
      fecha = now();
      String tiempoActual = timeToString(fecha);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PFIERRO TIMER");
    
      lcd.setCursor(0, 1);
      lcd.print(tiempoActual + " " + estado);
      Serial.println(tiempoActual);
      
      delay(1000);
      reseteaTiempo(tiempoActual);

      leerTeclado();
  }else{
      lcd.setCursor(0, 1);
      leerTeclado();
      delay(350);
  }
 
 // midebugmem();

}

void midebugmem(){
  lcd.print(dato);
  Serial.print("Read EPROM: >>");
  Serial.print(dato);
  Serial.println("<<");
  Serial.print("Formated: ");
  Serial.println(formateaHoraCadena(0,int(dato),0));
  delay(1000);
}

String timeToString(time_t fecha){
   String hora    = dosdigit(hour(fecha));
   String minuto  = dosdigit(minute(fecha));
   String segundo = dosdigit(second(fecha));
   return (hora + ":" + minuto + ":" + segundo);
}

String formateaHoraCadena(int ihora, int iminuto, int isegundo){
   String hora    = dosdigit(ihora);
   String minuto  = dosdigit(iminuto);
   String segundo = dosdigit(isegundo);
   return (hora + ":" + minuto + ":" + segundo);
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

void reseteaTiempo(String tiempoActual) {
  if ( cicloSalida == 1){
      if (tiempoON == tiempoActual) {
        setTime(0, 0, 0, 12, 12, 2021);     // Resetea a cero el reloj de tiempo
        //intercambiaEstadoLed();
        apagarSalida();
        cicloSalida = 0;
      }
  }else{
      if (tiempoOFF == tiempoActual) {
        setTime(0, 0, 0, 12, 12, 2021);     // Resetea a cero el reloj de tiempo
        //intercambiaEstadoLed();
        encenderSalida();
        cicloSalida = 1;
      }
  }
}

void encenderSalida(){
      estado = "ON";
      digitalWrite(ledPIN, 1);
      Serial.println("LED ON");
}
void apagarSalida(){
      estado = "OFF";
      digitalWrite(ledPIN, 0);
      Serial.println("LED OFF");
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



void leerTeclado() {
  int x = analogRead (0);
  if (x < 60) {               // lcd.print ("Right ");  ALMACENA NUEVO VALOR
         modoseteo= 0;         // Sale del modo SETEO de tiempo
         EEPROM.write(0, memon);
         Serial.print("MEM STORE ON: ");
         Serial.println(memon);
         lcd.setCursor(0,0);
         lcd.print("MEM STORE ON: ");
         lcd.setCursor(14,0);
         lcd.print(memon);
         tiempoON = formateaHoraCadena(0, int(memon),0);
         delay(1500); 
         
         setDuty = 1;
         EEPROM.write(1, memoff);
         Serial.print("MEM STORE OFF: ");
         Serial.println(memoff);
         lcd.setCursor(0,0);
         lcd.print("MEM STORE OFF: ");
         lcd.setCursor(14,0);
         lcd.print(memoff);
         tiempoOFF = formateaHoraCadena(0, int(memoff),0);
         delay(1500);
         
  }else if (x < 200){         // lcd.print ("Up    "); 
        lcd.setCursor(0, 1);
        lcd.print("   ");
        lcd.setCursor(0, 1);
        if( setDuty == 1){
            memon = memon + 1;
            lcd.print(memon);
        }else{
            memoff = memoff + 1;
            lcd.print(memoff);
        }
        Serial.print("memon ");
        Serial.print(memon);
        Serial.print("memoff ");  
        Serial.println(memoff);
  }else if (x < 400){         // lcd.print ("Down  "); 
        lcd.setCursor(0, 1);
        lcd.print("   ");
        lcd.setCursor(0, 1);

        if( setDuty == 1){
            memon = memon - 1;
            lcd.print(memon);
        }else{
            memoff = memoff - 1;
            lcd.print(memoff);
        }
        Serial.print("memon ");
        Serial.print(memon);
        Serial.print("memoff ");  
        Serial.println(memoff);
  }else if (x < 600){   
        lcd.print ("Cancelando...");
        delay(1000); 
        modoseteo= 0;
  }else if (x < 800){         // lcd.print ("Select"); <-----SELECT
        lcd.clear();
        lcd.setCursor(0,0);
        if( setDuty == 1 ){
            setDuty = 0;
        }else{
            setDuty = 1;
        }
        if( setDuty == 1){
            lcd.print ("SET MIN ON ");
            lcd.setCursor(0,1);
            lcd.print(memon);

            Serial.print("EEP_ON ");
            Serial.println(memon);
        }else{
            lcd.print ("SET MIN OFF");
            lcd.setCursor(0,1);
            lcd.print(memoff);

            Serial.print("EEP_OF ");
            Serial.println(memoff);
        }
        modoseteo= 1;
  }
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
