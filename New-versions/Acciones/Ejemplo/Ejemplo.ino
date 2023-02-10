#include <accion_coche.h>

//Conjunto de notas para tocar una melodia - consultar libreria pitches.h
int melodia[5] = { NOTE_D4, NOTE_CS5, NOTE_AS6, NOTE_G4, NOTE_DS8 };

void setup() {
  Serial.begin(115200); 
  inicializa_motores();
}


void loop() {

  avanza(DELANTE, 75, 1.5);

  // movimientos del servo
  servomotor_posicion(180);
  delay(500);
  servomotor_posicion(90);
  delay(500);
  servomotor_posicion(0);
  delay(500);

  gira(DERECHA, 50, 1);
  avanza(DELANTE, 75, 1);
  delay(500);

  //Tocar melodia
  for (int i = 0; i < 5; i++) {
    toca_nota(melodia[i], 2);
    delay(500); //pausa entre notas
  }

  avanza(ATRAS, 50, 1);
  gira(IZQUIERDA, 50, 1);
  avanza(ATRAS, 75, 1.5);

  delay(1000);  //pequeña pausa antes de repetir
}
