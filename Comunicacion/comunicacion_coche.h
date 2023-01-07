/*
LIBRERIA PARA LA COMUNICACION COCHE-CAMARA

*/

#ifndef comunicacion_h
#define comunicacion_h
 
#include "Arduino.h"

// DEFINCION DE LOS CARACTERES DE COMUNICACION
extern const char STX ;  //START OF TEXT
extern const char ETX ;  //END OF TEXT
extern const char ACK;   //ACKNOWLEDGE
extern const char NAK ;  //NEGATIVE ACKNOWLEDGE
extern const char SYN ;  //SYNCHRONOUS IDLE
extern const char EOT ;  //END OF TRANSMISSION
extern const char DLE ;  //DATA LINK ESCAPE

/*struct accion_tag {
    char movimiento;
    int parametro1;
    int parametro2;
};*/

//Calculo del checksum basado en el metodo XOR
uint8_t Checksum(const byte *datos, size_t longitud);

/*Esta funcion comprueba el puerto serie, cuando detecte un 'SYN' responde con un 'ACK' y
se establece la comunicacion y procede a llamar a lectura_datos*/
void establecimiento_conexion(byte *estruc_datos, const uint8_t longitud, const int timeout);

/*Finaliza la comunicacion*/
void fin_comunicacion(int timeout);

/*Tras recibir los datos en lectura_datos, esta función los procesa y manda a los motores
la accion correspondiente*/
void ejecuta_accion();

/*Tras establecer la conexion, cuando vuelve a haber datos en el puerto serie, esta función
los lee: Si es 'STX' procede a leer el movimiento recibido y comrpobar el checksum y el caracter
'ETX'. En cualquier caso de error responde con un 'NAK'*/
int lectura_datos(byte *estruc_datos, uint8_t longitud_estructura, int timeout);

//ESTA ES LA FUNCION DEL USUARIO
/*Abstrae todo simplemente tiene que llamarla*/
void sigue_tag();


#endif