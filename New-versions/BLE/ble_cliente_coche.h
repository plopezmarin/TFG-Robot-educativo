/*
LIBRERIA PARA EL BLUETOOTH - PARTE DEL COCHE (CLIENTE)
*/

#ifndef bleCoche_h
#define blecoche_h

#include "Arduino.h"
#include "BLEDevice.h"
#include "accion_coche.h"

/*FUNCION DE INICIALIZACION
Esta función inicializa el protocolo BLE del lado del cliente (coche)*/
void inicializa_ble_coche();

/*FUNCION HANDLER DE MOTORES BLE
Esta funcion maneja los motores en función del valor obtenido de la posición del joystick */
void ejecuta_movimiento();

/*FUNCION DE ALTO NIVEL PARA CONTROL
Est afunción invoca a la del ejecuta_movimiento(), pero antes comprueba que la ocnexión siga establecida y 
comprueba si la posición del joystick ha cambiado, en caso contrario no la invoca, pues no es necesario variar 
la accion de los motores*/
void control_coche();


#endif