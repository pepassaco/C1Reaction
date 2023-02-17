#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

const int pinOnOff = 4;
const int pinPitador = 3;
const int pinSens = 2;

int minEspera = 1000;
int maxEspera = 8000;

unsigned long aux = 0;
volatile unsigned long t = 0;
volatile bool esRata = false;
volatile bool boton = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void miISR(){
  analogWrite(3, 0);
  boton = true;
  if(t==0){
    esRata = true;
  }else{
    aux = millis();
    esRata = false;
  }
}


void intro(){
  lcd.backlight();
  lcd.clear();
  lcd.print("Vel. de reaccion");
  lcd.setCursor(0, 1);
  lcd.print("    v0.1 - Pepe");
  delay(5000);
}




void preparado(){
  //lcd.noBacklight();
  lcd.clear();
  //lcd.setCursor(0, 0);
  lcd.print("Menos de 10 seg.");
  lcd.setCursor(0, 1);
  lcd.print("Preparado...");
  attachInterrupt(0, miISR, FALLING);
  t = 0;
}


void reac(){
  t = millis();
  lcd.backlight();
  lcd.clear();
  lcd.print("YAYAYAYAYAYAYAYA");
  lcd.setCursor(0, 1);
  lcd.print("YAYAYAYAYAYAYAYA");
  analogWrite(3, 50);
  delay(1000);
  analogWrite(3, 0);
}

void rata(){
  detachInterrupt(0);
  lcd.clear();
  lcd.backlight();
  //lcd.setCursor(0, 0);
  lcd.print("Ladron asqueroso");
  lcd.setCursor(0, 1);
  lcd.print("Rata inmunda");
  analogWrite(3, 80);
  delay(2000);
  analogWrite(3, 0);
}

void resultado(){
  detachInterrupt(0);
  lcd.clear();
  lcd.setCursor(0, 0);
  t = aux-t;
  //t = round(t/1000);
  Serial.println(t);
  if(t<1000){
    lcd.backlight();
    lcd.print("   Tiempo de   ");
    lcd.setCursor(0, 1);
    String salida = "reaccion: 0."+String(t);
    delay(10);
    //Serial.println(salida);
    salida.concat("s");
    //strcat(salida, t);
    //strcat(salida, "s");
    lcd.print(salida);
  }else{
    lcd.backlight();
    lcd.print("    Intento");
    lcd.setCursor(0, 1);
    lcd.print("    fallido");
  }
  
}



void setup(){
  
  lcd.begin();
  lcd.clear();
  intro();
  
}



void loop(){
  
  while(digitalRead(pinOnOff)){
    lcd.noBacklight();
  }
  
  esRata = false;
  boton = false;
  
  preparado();
  
  delay(random(minEspera,maxEspera));

  reac();
  while(!boton){
  }
  boton = false;
  if(esRata){
    rata();
  }else{
    resultado();
  }
  
  delay(5000);
}
