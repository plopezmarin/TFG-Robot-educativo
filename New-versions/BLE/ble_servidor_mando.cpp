/*
LIBRERIA PARA EL BLUETOOTH - PARTE DEL MANDO (SERVIDOR)
*/

#include "BLEDevice.h"
#include <BLEUtils.h>
#include <BLEServer.h>
#include "ble_servidor_mando.h"

//Definición de los pines del joystick
const int SW_pin = 25;  // Pin digital - Conectado a la salida switch (no usado)
const int X_pin = 35;   // Pin analógico - Conectado a la salida X
const int Y_pin = 34;   // Pin analógico - Conectado a la salida Y

bool deviceConnected = false;

// UUID del servicio que buscamos
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");

// Caracteristica del servicio
static BLEUUID charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

// Caracteristica para el movimiento
BLECharacteristic *c_movimiento;

//Setup callbacks onConnect and onDisconnect
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
  }
};

String control_mando() {
  String mov;
  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  int x = analogRead(X_pin);
  if (x < 400) {
    mov = "1";
  } else if (x < 1700) {
    mov = "2";
  } else if (x < 2100) {
    mov = "3";
  } else if (x < 3800) {
    mov = "4";
  } else mov = "5";

  int y = analogRead(Y_pin);
  if (y < 400) {
    mov = mov + "1";
  } else if (y < 1700) {
    mov = mov + "2";
  } else if (y < 2100) {
    mov = mov + "3";
  } else if (y < 3800) {
    mov = mov + "4";
  } else mov = mov + "5";
  return mov;
}

void inicializa_ble_mando(){
Serial.println("Inicializando BLE!");

  BLEDevice::init("Coche");
  BLEServer *pServer = BLEDevice::createServer();  // Crear servidor
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(serviceUUID);  // Crear servicio

  c_movimiento = pService->createCharacteristic(	//Crear característica
    charUUID,
    BLECharacteristic::PROPERTY_NOTIFY);


  //Arrancar el servicio
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(serviceUUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();
  Serial.println("Caracteristica definida!");
}

void envia_movimiento_ble(String mov_notify) {
  if (deviceConnected) {
    //
    c_movimiento->setValue(mov_notify.c_str());
    c_movimiento->notify();

    delay(1);  //Para evitar la excepcion del WDT
  }
}