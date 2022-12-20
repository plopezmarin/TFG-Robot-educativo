/*
LIBRERIA PARA LA COMUNICACION COCHE-CAMARA

*/

#ifndef comunicacion_h
#define comunicacion_h
 
#include "Arduino.h"

extern const char STX ;  //START OF TEXT
extern const char ETX ;  //END OF TEXT
extern const char ACK;  //ACKNOWLEDGE
extern const char NAK ;  //NEGATIVE ACKNOWLEDGE
extern const char SYN ;  //SYNCHRONOUS IDLE
extern const char EOT ;  //END OF TRANSMISSION
extern const char DLE ;  //DATA LINK ESCAPE

/*struct accion_tag {
    char movimiento;
    int parametro1;
    int parametro2;
};*/

uint8_t Checksum(const byte *datos, size_t longitud);
void establecimiento_conexion(byte *estruc_datos, const uint8_t longitud, const int timeout);
void fin_comunicacion(int timeout);
void ejecuta_accion();
//void ejecuta_accion(struct accion_tag estruc_datos);
int lectura_datos(byte *estruc_datos, uint8_t longitud_estructura, int timeout);
void sigue_tag();


#endif