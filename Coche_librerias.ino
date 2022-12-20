#include <accion_coche.h>
#include <sensor_coche.h>
#include <comunicacion_coche.h>

//PRUEBAS BLE
/*
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHARACTERISTIC_UUID2 "d62e30e3-08e2-41de-8133-278059c871a2"
#define CHARACTERISTIC_UUID3 "1e580b4e-2c8b-48e7-a325-d3bf5e620082"

String xStatus;
String yStatus;

// Caracteristica para la componente X
BLECharacteristic xWrite(
  CHARACTERISTIC_UUID,
  BLECharacteristic::PROPERTY_WRITE);

// Caracteristica para la componente Y
BLECharacteristic yWrite(
  CHARACTERISTIC_UUID2,
  BLECharacteristic::PROPERTY_WRITE);

// Caracteristica para el sensor de proximidad frontal
BLECharacteristic sensorFrontal(
  CHARACTERISTIC_UUID3,
  BLECharacteristic::PROPERTY_NOTIFY);


void controlCoche() {
  // Rutina para controlar los motores del coche
  if (xStatus == "1" && yStatus == "0") {
    // Movimiento hacia delante con velocidad baja
    avanza_delante(96,0);
  } else if (xStatus == "2" && yStatus == "0") {
    // Movimiento hacia delante con velocidad alta
    avanza_delante(192,0);
  } else if (xStatus == "-1" && yStatus == "0") {
    // Movimiento hacia atrás con velocidad baja
    avanza_atras(96,0);
  } else if (xStatus == "-2" && yStatus == "0") {
    // Movimiento hacia atrás con velocidad alta
    avanza_atras(192,0);
  } else if (xStatus == "0" && yStatus == "1") {
    // Giro derecha con velocidad baja
    gira_derecha();
  } else if (xStatus == "0" && yStatus == "2") {
    // Giro derecha con velocidad alta
    gira_derecha();
  } else if (xStatus == "0" && yStatus == "-1") {
    // Giro izquierda con velocidad baja
    gira_izquierda();
  } else if (xStatus == "0" && yStatus == "-2") {
    // Giro izquierda con velocidad alta
    gira_izquierda();
  } else if (((xStatus == "1" && yStatus == "1") || (xStatus == "1" && yStatus == "2") || (xStatus == "2" && yStatus == "1") || (xStatus == "2" && yStatus == "2"))) {
    // Movimiento hacia delante con giro a la derecha si existe distancia suficiente
    ledcWrite(PWMChannelA, 192);
    ledcWrite(PWMChannelB, 96);
    digitalWrite(IN1Pin, HIGH);
    digitalWrite(IN2Pin, LOW);
    digitalWrite(IN3Pin, LOW);
    digitalWrite(IN4Pin, HIGH);
  } else if (((xStatus == "1" && yStatus == "-1") || (xStatus == "1" && yStatus == "-2") || (xStatus == "2" && yStatus == "-1") || (xStatus == "2" && yStatus == "-2"))) {
    // Movimiento hacia delante con giro a la izquierda si existe distancia suficiente
    ledcWrite(PWMChannelA, 96);
    ledcWrite(PWMChannelB, 192);
    digitalWrite(IN1Pin, HIGH);
    digitalWrite(IN2Pin, LOW);
    digitalWrite(IN3Pin, LOW);
    digitalWrite(IN4Pin, HIGH);
  } else if (((xStatus == "-1" && yStatus == "1") || (xStatus == "-1" && yStatus == "2") || (xStatus == "-2" && yStatus == "1") || (xStatus == "-2" && yStatus == "2"))) {
    // Movimiento hacia atrás con giro a la derecha si existe distancia suficiente
    ledcWrite(PWMChannelA, 192);
    ledcWrite(PWMChannelB, 96);
    digitalWrite(IN1Pin, LOW);
    digitalWrite(IN2Pin, HIGH);
    digitalWrite(IN3Pin, HIGH);
    digitalWrite(IN4Pin, LOW);
  } else if (((xStatus == "-1" && yStatus == "-1") || (xStatus == "-1" && yStatus == "-2") || (xStatus == "-2" && yStatus == "-1") || (xStatus == "-2" && yStatus == "-2"))) {
    // Movimiento hacia atrás con giro a la izquierda si existe distancia suficiente
    ledcWrite(PWMChannelA, 96);
    ledcWrite(PWMChannelB, 192);
    digitalWrite(IN1Pin, LOW);
    digitalWrite(IN2Pin, HIGH);
    digitalWrite(IN3Pin, HIGH);
    digitalWrite(IN4Pin, LOW);
  } else {
    // Parada de los motores
    frena();
  }
}

void lecturaMando() {
  // Rutina para leer los valores del mando
  std::string xControlValue = xWrite.getValue();
  std::string yControlValue = yWrite.getValue();

  if ((xControlValue != xStatus.c_str() || yControlValue != yStatus.c_str())) {
    xStatus = xControlValue.c_str();
    yStatus = yControlValue.c_str();
  }
}

void inicializa_ble(){
  Serial.println("Starting BLE work!");

  BLEDevice::init("Grupo5");
  BLEServer *pServer = BLEDevice::createServer(); // Crear servidor
  BLEService *pService = pServer->createService(SERVICE_UUID);  // Crear servicio

  pService->addCharacteristic(&xWrite);
  pService->addCharacteristic(&yWrite);
  pService->addCharacteristic(&sensorFrontal);

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // Funcion que ayuda con la conexión de iPhone
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined!");


  xStatus = "0";
  yStatus = "0";
}
*/

void setup() {
  Serial.begin(9600);

  //Inicializar SENSOR ULTRASONIDOS
  inicializa_sensor_ultrasonidos(trigger1, echo1);
  inicializa_sensor_ultrasonidos(trigger2, echo2);

  // Inicializar motores
  inicializa_motores();

  // Crear semaforo
  crea_semaforo();

  setup_tarea();

  //inicializa_ble();
}


void loop() {
//lecturaMando();
//controlCoche();
//delay(50);
sigue_tag();
}