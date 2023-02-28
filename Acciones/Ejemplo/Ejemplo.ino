//Incluir libreria de acciones
#include <accion_coche.h>

void setup() {
  Serial.begin(115200); 
  inicializa_motores();
}


void loop() {

  avanza(DELANTE, 75, 1.5);

  gira(DERECHA, 50, 1);
  avanza(DELANTE, 75, 1);
  delay(500);

  avanza(ATRAS, 50, 1);
  gira(IZQUIERDA, 50, 1);
  avanza(ATRAS, 75, 1.5);

  frena(1);

}
