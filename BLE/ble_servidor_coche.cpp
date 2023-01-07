/*
LIBRERIA PARA EL BLUETOOTH - PARTE DEL coche (servidor)
*/

#include "ble_servidor_coche.h"

#include <sensor_coche.h>
#include <comunicacion_coche.h>


//PRUEBAS BLE

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_MOV_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


String movimiento = "SSSS";

// Caracteristica para el movimiento
BLECharacteristic c_movimiento(
  CHARACTERISTIC_MOV_UUID,
  BLECharacteristic::PROPERTY_WRITE);

int choque_frontal() {
  return sensor_delantero.lectura_ultrasonidos() - 20;
}

int choque_trasero() {
  return sensor_trasero.lectura_ultrasonidos() - 20;
}

void control_coche() {
	//Serial.println(movimiento);
	int obstaculo_delante = choque_frontal();
	int obstaculo_detras = choque_trasero();

  //DELANTE - VELOCIDAD BAJA
  if (movimiento == "FLSS" && obstaculo_delante >0) avanza_delante(50, 0);
  //DELANTE - VELOCIDAD ALTA
  else if (movimiento == "FHSS" && obstaculo_delante >0) avanza_delante(100, 0);
  
  
  //DETRAS - VELOCIDAD BAJA
  else if (movimiento == "RLSS" && obstaculo_detras >0 ) avanza_atras(50, 0);
  //DETRAS - VELOCIDAD ALTA
  else if (movimiento == "RHSS" && obstaculo_detras >0) avanza_atras(100, 0);
  
  //GIRA DERECHA -VELOCIDAD BAJA
  else if (movimiento == "SSRL") gira_derecha(50);
  //GIRA DERECHA -VELOCIDAD ALTA
  else if (movimiento == "SSRH") gira_derecha(100);
  //GIRA IZQUIERDA -VELOCIDAD BAJA
  else if (movimiento == "SSLL") gira_izquierda(50);
  //GIRA IZQUIERDA -VELOCIDAD ALTA
  else if (movimiento == "SSLH") gira_izquierda(100);

  //DELANTE & DERECHA LENTO
  else if (movimiento == "FLRL" && obstaculo_delante > (-10)) {
    control_motorA(HIGH, LOW, 50);
    control_motorB(LOW, LOW, 0);
  }
  //DELANTE & DERECHA RAPIDO
  else if (movimiento == "FHRH" && obstaculo_delante > (-10)) {
    control_motorA(HIGH, LOW, 100);
    control_motorB(LOW, HIGH, 50);
  }
  //DELANTE & IZQUIERDA LENTO
  else if (movimiento == "FLLL" && obstaculo_delante > (-10)) {
    control_motorA(HIGH, LOW, 0);
    control_motorB(LOW, HIGH, 50);
  }
  //DELANTE & IZQUIERDA RAPIDO
  else if (movimiento == "FLLH" && obstaculo_delante > (-10)) {
    control_motorA(HIGH, LOW, 50);
    control_motorB(LOW, HIGH, 100);
  }
  //ATRAS & DERECHA LENTO
  else if (movimiento == "RLRL" ) {
    control_motorA(LOW, HIGH, 50);
    control_motorB(HIGH, LOW, 0);
  }
  //ATRAS & DERECHA RAPIDO
  else if (movimiento == "RHRH" ) {
    control_motorA(LOW, HIGH, 100);
    control_motorB(HIGH, LOW, 50);
  }
  //ATRAS & IZQUIERDA LENTO
  else if (movimiento == "RLLL" ) {
    control_motorA(LOW, HIGH, 0);
    control_motorB(HIGH, LOW, 50);
  }
  //ATRAS & IZQUIERDA RAPIDO
  else if (movimiento == "RHLH" ) {
    control_motorA(LOW, HIGH, 0);
    control_motorB(HIGH, LOW, 100);
  } else {
    frena_motores();
  }

}


void lectura_mando() {
  // Rutina para leer los valores del mando
  //obstaculo_delante=choque_frontal();
  //obstaculo_detras=choque_trasero();
  
  std::string xControlValue = c_movimiento.getValue();

  if (xControlValue != movimiento.c_str()) {
    movimiento = xControlValue.c_str();
  }
}

void inicializa_ble() {
  Serial.println("Starting BLE work!");

  BLEDevice::init("Coche");
  BLEServer *pServer = BLEDevice::createServer();               // Crear servidor
  BLEService *pService = pServer->createService(SERVICE_UUID);  // Crear servicio

  pService->addCharacteristic(&c_movimiento);

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // Funcion que ayuda con la conexión de iPhone
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined!");
}