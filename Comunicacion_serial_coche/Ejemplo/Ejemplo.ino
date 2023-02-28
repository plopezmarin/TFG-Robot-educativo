#include <accion_coche.h>
#include <comunicacion_coche.h>


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //los baudios deben ser los mismos que la cámara

  inicializa_motores(); //driver de inicializar motores 

}

void loop() {
  // put your main code here, to run repeatedly:
  comunicacion_r(300);



}
