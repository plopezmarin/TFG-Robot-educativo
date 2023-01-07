#include "Arduino.h"
#include "accion_coche.h"
#include "comunicacion_coche.h"

 const char STX = '\x002';  //START OF TEXT
 const char ETX = '\x003';  //END OF TEXT
 const char ACK = '\x006';  //ACKNOWLEDGE
 const char NAK = '\x015';  //NEGATIVE ACKNOWLEDGE
 const char SYN = '\x016';  //SYNCHRONOUS IDLE
 const char EOT = '\x004';  //END OF TRANSMISSION
 const char DLE = '\x010';  //DATA LINK ESCAPE


 struct accion_tag {
     char movimiento;
     int parametro1;
     int parametro2;
 } Accion_leida;

uint8_t Checksum(const byte *datos, size_t longitud)
{
  uint8_t valor = 0;
  for (size_t i = 0; i < longitud; i++)
  {
    valor ^= (uint8_t)datos[i];
  }
  return ~valor;
}

void establecimiento_conexion(byte* estruc_datos, const uint8_t longitud, const int timeout) {
    if (Serial.read() == SYN) {
        Serial.write(ACK);
        lectura_datos(estruc_datos, longitud, timeout);
        fin_comunicacion(timeout);
    }
    //no hacer nada, no hay datos 
}

void fin_comunicacion(int timeout) {
    unsigned long inicio = millis();
  while (Serial.available() == 0 && (millis() - inicio) < timeout)
    ;
  if (Serial.read() == EOT)
    ;
}

void ejecuta_accion() {
//void ejecuta_accion(struct accion_tag estruc_datos) {
  char movimiento = Accion_leida.movimiento;
  switch (movimiento) {
    case 'F':
      avanza(DELANTE, Accion_leida.parametro1, Accion_leida.parametro2);
      break;
    case 'B':
      avanza(ATRAS, Accion_leida.parametro1, Accion_leida.parametro2);
      break;
    case 'L':
      gira(IZQUIERDA, Accion_leida.parametro2);
      break;
    case 'R':
      gira(DERECHA, Accion_leida.parametro2);
      break;
    case 'S':
      frena_motores();
      break;
  }
}

int lectura_datos(byte *estruc_datos, uint8_t longitud_estructura, int timeout) {
  unsigned long inicio = millis();

  while (Serial.available() < (longitud_estructura + 4) && (millis() - inicio) < timeout)
    ;
  if (Serial.available() >= longitud_estructura + 4) {
    if (Serial.read() == STX) {
      Serial.readBytes(estruc_datos, longitud_estructura);

      if (Serial.read()==Checksum(estruc_datos, longitud_estructura) && Serial.read() == ETX) {
        ejecuta_accion();
        //Cuando termine de ejecutar la accion responde con un ACK
        Serial.write(ACK);
        return 0;
      }
      //ERROR EN EL MENSAJE
      Serial.write(NAK);
      return 1;
    }
    //TIMEOUT_EXCEDIDO - NO ENTRA AL IF PORQUE NO HAY DATOS
    Serial.write(NAK);
    return -1;
  }
}


void sigue_tag() {

    establecimiento_conexion((byte*)&Accion_leida, sizeof(Accion_leida), 5000);

}

