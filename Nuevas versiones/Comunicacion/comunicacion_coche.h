/*
LIBRERIA PARA LA COMUNICACION COCHE-CAMARA

*/

#ifndef comunicacion_h
#define comunicacion_h
 
#include "Arduino.h"

// DEFINCION DE LOS CARACTERES DE COMUNICACION
const char STX = '\x002';  //START OF TEXT
const char ETX = '\x003';  //END OF TEXT
const char ACK = '\x006';  //ACKNOWLEDGE
const char NAK = '\x015';  //NEGATIVE ACKNOWLEDGE
const char SYN = '\x016';  //SYNCHRONOUS IDLE
const char EOT = '\x004';  //END OF TRANSMISSION
const char EOA = '\x020';  //END OF ACTION


//Calculo del checksum basado en el metodo XOR
uint8_t Checksum(const byte *datos, size_t longitud);

/*Esta funci�n hace de HANDLER, esta a la espera de recibir un mensaje por el puerto serie.
Cuando recibe un mensaje, comprueba su primer byte, en funci�n de este invoca a la funci�n correspondiente*/
void comunicacion_r(const int timeout);

/*Tras recibir los datos en Accion_recibida, esta funci�n los procesa y manda al actuador correspondiente
En caso de a�adir nuevas funcionalidades, se deben a�adir m�s opciones al Switch-case.
Importante respetar el formato de la estructura:
char para el tipo de accion
int para segundo par�metro 
float para el tercer par�metro (normalmente el tiempo)*/
void ejecuta_accion();

/*Cuando el handler recibe un 'STX', significa que va a recibir una orden e invoca esta funci�n.
Esta funci�n se encarga leer los datos recibidos por el puerto serie y almacenarlos en la estructura predefinida*/
int lectura_datos(int timeout);

#endif