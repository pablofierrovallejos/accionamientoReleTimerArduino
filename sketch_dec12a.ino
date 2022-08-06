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
String tiempoOFF = "00:00:10";         // Tiempo base para el conteo
String estado;                         // Estado de la salida a activar
String tiempoActual="";
const int ledPIN = 3;                  // Pin para activar relé
int  modoseteo=0;                      // Modo seteo tiempo 1 = seteando por usuario (presionó SET)
byte tiemposeteadoRST = 0;
byte setDuty = 1;                      // Para setear el tiempo de encendido
byte memon =0;                         // Contenido de memoria posicion 0 para el tiempo en on
byte memoff =0;                        // Contenido de memoria posicion 1 para el tiempo en off
byte memonS =0;                        // Contenido de memoria posicion 2 para el tiempo en on  segundos
byte memoffS =0;                       // Contenido de memoria posicion 3 para el tiempo en off segundos

byte cicloSalida = 1;                  // Define el estado actual de la salida
byte imenu = -1;                        // Opción de menú actual

void setup() {
  
  //inicializaMemoria();
  
  Serial.begin(9600);
  lcd.begin(16, 2);                   // Indicar a la libreria que tenemos conectada una pantalla de 16x2 (COLUMNAS x FILAS)
  lcd.clear();                        // Mover el cursor a la primera posición de la pantalla (0, 0)
  lcd.print("INICIO TIMER");
  Serial.println("INICIO TIMER");
  
  setTime(0, 0, 0, 12, 12, 2021);     // Establecemos la fecha
  pinMode(ledPIN , OUTPUT);
  encenderSalida();
  Serial.println("LED ON");
  estado = "ON";

  memon   = EEPROM.read(0);
  memonS  = EEPROM.read(1);
  memoff  = EEPROM.read(2);
  memoffS = EEPROM.read(3);
  
  Serial.print("EEPROM ");

  tiempoON =   formateaHoraCadena(0,int(memon),  int(memonS));
  tiempoOFF =  formateaHoraCadena(0,int(memoff), int(memoffS));
  
}
void inicializaMemoria(){
     EEPROM.write(0, 0);
     EEPROM.write(1, 1);
     EEPROM.write(2, 0);
     EEPROM.write(3, 1);
}

void loop() {

  Serial.println("Loop -> ");
  Serial.print("Time On  "); Serial.println(tiempoON);
  Serial.print("Time Off "); Serial.println(tiempoOFF);
  if(modoseteo==0){
      
      fecha = now();
      tiempoActual = timeToString(fecha);
      lcd.setCursor(0, 0);
      lcd.print("PFIERRO TIMER");
      lcd.setCursor(0, 1);
      lcd.print(tiempoActual + " " + estado);

      Serial.println(tiempoActual);

      reseteaTiempo(tiempoActual);

      leerTeclado();
  }else{
      lcd.setCursor(0, 1);
      leerTeclado();
  }
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
        apagarSalida();
        cicloSalida = 0;
      }
  }else{
      if (tiempoOFF == tiempoActual) {
        setTime(0, 0, 0, 12, 12, 2021);     // Resetea a cero el reloj de tiempo
        encenderSalida();
        cicloSalida = 1;
      }
  }
  
}

void encenderSalida(){
      estado = "ON ";
      digitalWrite(ledPIN, 0);
      Serial.println("LED ON_");
      lcd.setCursor(0, 1);
      lcd.print(tiempoActual + " " + estado);
}
void apagarSalida(){
      estado = "OFF";
      digitalWrite(ledPIN, 1);
      Serial.println("LED OFF");
      lcd.setCursor(0, 1);
      lcd.print(tiempoActual + " " + estado);
}
void intercambiaEstadoLed() {
  if (digitalRead(ledPIN)) {
      estado = "OFF";
      digitalWrite(ledPIN, 0);
      Serial.println("LED OFF");
  } else {
      estado = "ON ";
      digitalWrite(ledPIN, 1);
      Serial.println("LED ON");
  }
}



void leerTeclado() {
  int x = analogRead (0);
  if (x < 60) {               // lcd.print ("Right ");  ALMACENA NUEVO VALOR
         modoseteo= 0;         // Sale del modo SETEO de tiempo
         EEPROM.write(0, memon);
         EEPROM.write(1, memonS);
         Serial.print("MEM STORE ON: ");
         Serial.println(memon);
         lcd.setCursor(0,0);
         lcd.print("MEM STORE ON: ");
         lcd.setCursor(14,0);
         lcd.print(memon);
         tiempoON = formateaHoraCadena(0, int(memon),  int(memonS));
         delay(1500); 
         
         setDuty = 1;
         EEPROM.write(2, memoff);
         EEPROM.write(3, memoffS);
         Serial.print("MEM STORE OFF: ");
         Serial.println(memoff);
         lcd.setCursor(0,0);
         lcd.print("MEM STORE OFF: ");
         lcd.setCursor(14,0);
         lcd.print(memoff);
         tiempoOFF = formateaHoraCadena(0, int(memoff),  int(memoffS));
         delay(1500);
         
  }else if (x < 200){         // lcd.print ("Up    "); 
        lcd.setCursor(0, 1); lcd.print("   "); lcd.setCursor(0, 1);
        actualizaMemoria(1);
  
        Serial.print("memon "); Serial.print(memon); Serial.print(memonS);
        Serial.print(" memoff "); Serial.print(memoff); Serial.println(memoffS); 
  }else if (x < 400){         // lcd.print ("Down  "); 
        lcd.setCursor(0, 1); lcd.print("   "); lcd.setCursor(0, 1);
        actualizaMemoria(-1);
        
        Serial.print("memon "); Serial.print(memon); Serial.print(memonS);
        Serial.print(" memoff "); Serial.print(memoff); Serial.println(memoffS);  
  }else if (x < 600){   
        lcd.print ("Cancelando...");
        delay(1000); 
        modoseteo= 0;
  }else if (x < 800){         // lcd.print ("Select"); <-----SELECT
        lcd.clear();
        lcd.setCursor(0,0);
        imenu++;
        if(imenu==4){ imenu=0;}
        switch(imenu){
          case 0:                   
                  lcd.print ("SET MIN ON ");
                  lcd.setCursor(0,1);
                  lcd.print(memon);
                  break;
          case 1: 
                  lcd.print ("SET SEC ON ");
                  lcd.setCursor(0,1);
                  lcd.print(memonS);
                  break;
          case 2: 
                  lcd.print ("SET MIN OFF");
                  lcd.setCursor(0,1);
                  lcd.print(memoff);
                  break;
          case 3: 
                  lcd.print ("SET SEC OFF");
                  lcd.setCursor(0,1);
                  lcd.print(memoffS);
                  break;
        }
        modoseteo= 1;

  }
}

void actualizaMemoria(byte paso){
    switch(imenu){
             case 0:
                    memon = memon + paso;
                    lcd.print(memon);
                    break;
             case 1:
                    memonS = memonS + paso;
                    lcd.print(memonS);          
                    break;
             case 2:
                    memoff = memoff + paso;
                    lcd.print(memoff);          
                    break;
             case 3:
                    memoffS = memoffS + paso;
                    lcd.print(memoffS);             
                    break;
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
}
*/
