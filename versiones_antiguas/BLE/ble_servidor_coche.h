/*
LIBRERIA PARA EL BLUETOOTH - PARTE DEL COCHE (SERVIDOR)
*/

#ifndef bleCoche_h
#define blecoche_h
 
#include "Arduino.h"
#include "BLEDevice.h"
#include "accion_coche.h"

void control_coche();
void lectura_mando();
void inicializa_ble();

#endif
