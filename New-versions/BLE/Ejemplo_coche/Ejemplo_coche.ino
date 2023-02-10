#include <ble_cliente_coche.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  inicializa_motores();
  inicializa_ble_coche();

}

void loop() {
  // put your main code here, to run repeatedly:
  control_coche();

}
