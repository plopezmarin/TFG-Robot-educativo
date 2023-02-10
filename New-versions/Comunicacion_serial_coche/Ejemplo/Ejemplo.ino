#include <accion_coche.h>
#include <comunicacion_coche.h>
///https://www.thomascountz.com/2021/02/21/arduino-tone-for-esp32


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  inicializa_motores();

}

void loop() {
  // put your main code here, to run repeatedly:
  comunicacion_r(300);



}
