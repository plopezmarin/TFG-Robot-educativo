/*
LIBRERIA PARA EL BLUETOOTH - PARTE DEL MANDO (CLIENTE)
*/

#ifndef bleMando_h
#define bleMando_h
 
#include "Arduino.h"
#include "BLEDevice.h"
#include <BLEUtils.h>
#include <BLEServer.h>

//joystick
extern const int SW_pin; // digital pin connected to switch output
extern const int X_pin; // analog pin connected to X output
extern const int Y_pin; // analog pin connected to Y output

/*FUNCI�N DEL DRIVER
Esta funci�n obtiene la posici�n del joystick, divide cada eje en 5 partes
devuelve la posici�n a modo de coordenadas. En reposo = 33*/
String control_mando();

/*FUNCION COMUNICACION
Esta funci�n notifica la caracteristica*/
void envia_movimiento_ble(String mov_notify);

/*FUNCION DE INICIALIZACION DEL BLE SERVIDOR
Esta funci�n inicializa el protocolo BLE de la parte del servidor (mando)*/
void inicializa_ble_mando();

#endif