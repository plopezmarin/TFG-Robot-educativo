/*
LIBRERIA PARA EL BLUETOOTH - PARTE DEL coche (servidor)
*/

#include "ble_servidor_coche.h"

//#include <accion_coche.h>
#include <sensor_coche.h>
#include <comunicacion_coche.h>

//PRUEBAS BLE

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_X_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHARACTERISTIC_Y_UUID "d62e30e3-08e2-41de-8133-278059c871a2"

char xStatus, yStatus;

// Caracteristica para la componente X
BLECharacteristic xWrite(
  CHARACTERISTIC_X_UUID,
  BLECharacteristic::PROPERTY_WRITE);

// Caracteristica para la componente Y
BLECharacteristic yWrite(
  CHARACTERISTIC_Y_UUID,
  BLECharacteristic::PROPERTY_WRITE);
  
void control_coche() {
  int valor_x = xStatus - '0';
  int valor_y = yStatus - '0';
  // Rutina para controlar los motores del coche
  if (valor_x == 1 && valor_y == 0) {
    // Movimiento hacia delante con velocidad baja
    avanza_delante(96, 0);
  } else if (valor_x == 2 && valor_y == 0) {
    // Movimiento hacia delante con velocidad alta
    avanza_delante(192, 0);
  } else if (valor_x == -1 && valor_y == 0) {
    // Movimiento hacia atrás con velocidad baja
    avanza_atras(96, 0);
  } else if (valor_x == -2 && valor_y == 0) {
    // Movimiento hacia atrás con velocidad alta
    avanza_atras(192, 0);
  } else if (valor_x == 0 && valor_y == 1) {
    // Giro derecha con velocidad baja
    gira_derecha();
  } else if (valor_x == 0 && valor_y == 2) {
    // Giro derecha con velocidad alta
    gira_derecha();
  } else if (valor_x == 0 && valor_y == -1) {
    // Giro izquierda con velocidad baja
    gira_izquierda();
  } else if (valor_x == 0 && valor_y == -2) {
    // Giro izquierda con velocidad alta
    gira_izquierda();
  } else if (((valor_x == 1 && valor_y == 1) || (valor_x == 1 && valor_y == 2) || (valor_x == 2 && valor_y == 1) || (valor_x == 2 && valor_y == 2))) {
    // Movimiento hacia delante con giro a la derecha si existe distancia suficiente
    if (sensor_delantero.lectura_ultrasonidos() > 20) {
      control_motorA(HIGH, LOW, 192);
      control_motorB(LOW, HIGH, 96);
    }
  } else if (((valor_x == 1 && valor_y == -1) || (valor_x == 1 && valor_y == -2) || (valor_x == 2 && valor_y == -1) || (valor_x == 2 && valor_y == -2))) {
    // Movimiento hacia delante con giro a la izquierda si existe distancia suficiente
    if (sensor_delantero.lectura_ultrasonidos() > 20) {
      control_motorA(HIGH, LOW, 96);
      control_motorB(LOW, HIGH, 192);
    }
  } else if (((valor_x == -1 && valor_y == 1) || (valor_x == -1 && valor_y == 2) || (valor_x == -2 && valor_y == 1) || (valor_x == -2 && valor_y == 2))) {
    // Movimiento hacia atrás con giro a la derecha si existe distancia suficiente
    if (sensor_trasero.lectura_ultrasonidos() > 20) {
      control_motorA(LOW, HIGH, 192);
      control_motorB(HIGH, LOW, 96);      
    }
  } else if (((valor_x == -1 && valor_y == -1) || (valor_x == -1 && valor_y == -2) || (valor_x == -2 && valor_y == -1) || (valor_x == -2 && valor_y == -2))) {
    // Movimiento hacia atrás con giro a la izquierda si existe distancia suficiente
    if (sensor_trasero.lectura_ultrasonidos() > 20) {
      control_motorA(LOW, HIGH, 96);
      control_motorB(HIGH, LOW, 192);
    }
  } else {
    // Parada de los motores
    frena_motores();
  }
}

void lectura_mando() {
  // Rutina para leer los valores del mando
  std::string xControlValue = xWrite.getValue();
  std::string yControlValue = yWrite.getValue();

  if (xControlValue[0] != xStatus || yControlValue[0] != yStatus) {
    xStatus = xControlValue[0];
    yStatus = yControlValue[0];
  }
}

void inicializa_ble() {
  Serial.println("Starting BLE work!");

  BLEDevice::init("Coche");
  BLEServer *pServer = BLEDevice::createServer();               // Crear servidor
  BLEService *pService = pServer->createService(SERVICE_UUID);  // Crear servicio

  pService->addCharacteristic(&xWrite);
  pService->addCharacteristic(&yWrite);

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // Funcion que ayuda con la conexión de iPhone
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined!");


  xStatus = '0';
  yStatus = '0';
}
