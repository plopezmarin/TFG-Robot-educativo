#include "Arduino.h"
#include "accion_coche.h"
//#include "comunicacion_coche.h"

const char STX = '\x002';  //START OF TEXT
const char ETX = '\x003';  //END OF TEXT
const char ACK = '\x006';  //ACKNOWLEDGE
const char NAK = '\x015';  //NEGATIVE ACKNOWLEDGE
const char SYN = '\x016';  //SYNCHRONOUS IDLE
const char EOT = '\x004';  //END OF TRANSMISSION
const char EOA = '\x020';  //END OF ACTION


struct accion_struct {
  char movimiento;
  int parametro1;
  float parametro2;
} Accion_recibida;

bool conectado = false;

uint8_t Checksum(const byte *datos, size_t longitud) {
  uint8_t valor = 0;
  for (size_t i = 0; i < longitud; i++) {
    valor ^= (uint8_t)datos[i];
  }
  return ~valor;
}

void ejecuta_accion() {
  //void ejecuta_accion(struct accion_tag estruc_datos) {
  char movimiento = Accion_recibida.movimiento;
  switch (movimiento) {
    case 'F':
      avanza(DELANTE, Accion_recibida.parametro1, Accion_recibida.parametro2);
      break;
    case 'B':
      avanza(ATRAS, Accion_recibida.parametro1, Accion_recibida.parametro2);
      break;
    case 'L':
      gira(IZQUIERDA, Accion_recibida.parametro1, Accion_recibida.parametro2);
      break;
    case 'R':
      gira(DERECHA, Accion_recibida.parametro1, Accion_recibida.parametro2);
      break;
    case 'S':
      frena_motores();
      break;
    case 'T':
      toca_nota(Accion_recibida.parametro1, Accion_recibida.parametro2);
      break;   
  }
}
//---------------------------------


//-------------

//--------------------------
void lectura_datos(int timeout) {
  //[longitud_estructura];
  int longitud = sizeof(Accion_recibida);
  unsigned long inicio = millis();

  while (Serial.available() < longitud && (millis() - inicio) < timeout)
    ;

  if (Serial.available() >= longitud) {
    Serial.readBytes((byte *)&Accion_recibida, longitud);
   /* if (Serial.read() == Checksum((byte *)&Accion_recibida, longitud) && Serial.read() == ETX) {
      ejecuta_accion();
    }*/
    ejecuta_accion();
  }
}

void comunicacion_r(const int timeout) {
  if (Serial.available() > 0) {
    char mensaje_entrante = Serial.read();

    if (mensaje_entrante == SYN) {
      conectado = true;
      Serial.write(ACK);
    } else if (mensaje_entrante == STX) {
      lectura_datos(200);
      Serial.write(EOA);  //cuando termine la ejecucion de la accion enviamos el fin de accion
    } else if (mensaje_entrante == EOT) {
      Serial.write(ACK);
      conectado = false;
    }
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  inicializa_motores();
}

void loop() {
  // put your main code here, to run repeatedly:
  comunicacion_r(300);

  //print();
}
