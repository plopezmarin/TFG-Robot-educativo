#include <ble_servidor_mando.h>

void setup() {
  //Configurar puerto serie
  Serial.begin(115200);
  //Inicializar BLE (servidor)
  inicializa_ble_mando();
}

void loop() {
  //Obtenemos posicion joystick
  String envio_movimiento = control_mando();  

  //Mostrar por pantalla la posición
  Serial.print("Posicion: ");
  Serial.println(envio_movimiento);

  //Enviar la posición al coche
  envia_movimiento_ble(envio_movimiento);
}
