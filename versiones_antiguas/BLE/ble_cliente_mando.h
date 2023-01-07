/*
LIBRERIA PARA EL BLUETOOTH - PARTE DEL MANDO (CLIENTE)
*/

#ifndef bleMando_h
#define bleMando_h
 
#include "Arduino.h"
#include "BLEDevice.h"

//joystick
extern const int SW_pin; // digital pin connected to switch output
extern const int X_pin; // analog pin connected to X output
extern const int Y_pin; // analog pin connected to Y output

  //bool doConnect;
  //bool connected;
  //bool doScan;

 //String envioy;
 //String enviox;

//Inicializa parametros del mando para poder usarlo
 void setup_mando();

 //Conecta con el servidor (coche)
 void conectar_al_servidor();

 //Transforma los movimientos del joystick
 void control_joystick();



#endif
