/*
LIBRERIA PARA EL BLUETOOTH - PARTE DEL COCHE (CLIENTE)
*/

#include "ble_cliente_coche.h"
#include <sensor_coche.h>

#include <BLEDevice.h>


static BLEUUID SERVICE_UUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
static BLEUUID CHARACTERISTIC_MOV_UUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

static BLEAdvertisedDevice* myDevice;

//Flags auxiliares
static bool conectar = false;
static bool conectado = false;
static bool scanear = false;
static BLERemoteCharacteristic* c_movimiento_remoto;

char* movimiento;
int movimiento_int;
bool cambio_direccion = true;

//FUNCIONES DE CALLBACK, CUANDO SERVIDOR ENVIE UN MOVIMIENTO CON LA PROPIEDAD NOTIFY
static void movimientoNotifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,  //caracteristica
  uint8_t* pData,                                     //el valor de la caracteristica
  size_t length,                                      //longitud
  bool isNotify) {

  movimiento = (char*)pData;
  sscanf(movimiento, "%d", &movimiento_int);	//Convertir a entero
  cambio_direccion = true;
}


bool connectToServer() {
  Serial.print("Formando conexión con: ");
  Serial.println(myDevice->getAddress().toString().c_str());

  BLEClient* pClient = BLEDevice::createClient();
  Serial.println(" - Cliente creado");

  // Connect to the remove BLE Server.
  pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
  Serial.println(" - Conectado al servidor");

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
  if (pRemoteService == nullptr) {
    Serial.print("Fallo al buscar el servicio UUID: ");
    Serial.println(SERVICE_UUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Servicio encontrado");

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  c_movimiento_remoto = pRemoteService->getCharacteristic(CHARACTERISTIC_MOV_UUID);
  if (c_movimiento_remoto == nullptr) {
    Serial.print("Fallo al buscar la caracteristica UUID: ");
    Serial.println(CHARACTERISTIC_MOV_UUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Caracteristica encontrada");

  // Read the value of the characteristic.
  if (c_movimiento_remoto->canRead()) {
    std::string value = c_movimiento_remoto->readValue();
  }

  if (c_movimiento_remoto->canNotify())
    c_movimiento_remoto->registerForNotify(movimientoNotifyCallback);

  conectado = true;
  return true;
}

/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(SERVICE_UUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      conectar = true;
      scanear = true;

    }  // Found our server
  }    // onResult
};     // MyAdvertisedDeviceCallbacks


void ejecuta_movimiento() {  //PROBAR SWITCH CASE
  Serial.println(movimiento_int);

  switch (movimiento_int) {
    case 55:	//DELANTE-DERECHA RAPIDO (45º)
      delante_ruedaA_ruedaB(100, 75);
      break;
	 case 54:	//DELANTE-DERECHA RAPIDO (22.5º)
		delante_ruedaA_ruedaB(100, 88);
		break;
    case 53:	//DELANTE RAPIDO (90º)
      delante_ruedaA_ruedaB(100, 100);
      break;
    case 52:	//DELANTE-IZQUIERDA RAPIDO (112.5º)
      delante_ruedaA_ruedaB(88, 100);
      break;
    case 51:	//DELANTE-IZQUIERDA RAPIDO (135º)
      delante_ruedaA_ruedaB(75, 100);
      break;
    case 45:	//DELANTE-DERECHA RAPIDO (22.5º)
      delante_ruedaA_ruedaB(100, 50);
      break;
    case 44:	//DELANTE-DERECHA LENTO (45º)
      delante_ruedaA_ruedaB(75, 50);
      break;
    case 43:	//DELANTE LENTO (90º)
      delante_ruedaA_ruedaB(50, 50);
      break;
    case 42:	//DELANTE-IZQUIERDA LENTO (45º)
      delante_ruedaA_ruedaB(50, 75);
      break;
    case 41:	//DELANTE-IZQUIERDA LENTO (157.5º)
      delante_ruedaA_ruedaB(50, 100);
      break;
    case 35:	//DERECHA RAPIDO (0º)
      gira_derecha(100, 0);
      break;
    case 34:	//DERECHA LENTO (0º)
      gira_derecha(50, 0);
      break;
    case 32:	//IZQUIERDA LENTO (180º)
      gira_izquierda(50, 0);
      break;
    case 31:	//IZQUIERDA RAPIDO (180º)
      gira_izquierda(100, 0);
      break;
    case 25:	//DETRAS-DERECHA RAPIDO (337.5º)
      atras_ruedaA_ruedaB(100, 50);
      break;
    case 24:	//DETRAS-DERECHA LENTO (315º)
      atras_ruedaA_ruedaB(75, 50);
      break;
    case 23:	//DETRAS LENTO (270º)
      atras_ruedaA_ruedaB(50, 50);
      break;
    case 22:	//DETRAS-IZQUIERDA LENTO (247.5º)
      atras_ruedaA_ruedaB(50, 75);
      break;
    case 21:	//DETRAS-IZQUIERDA RAPIDO (202.5º)
      atras_ruedaA_ruedaB(50, 100);
      break;
    case 15:	//DETRAS-DERECHA RAPIDO (315º)
      atras_ruedaA_ruedaB(100, 75);
      break;
    case 14:	//DETRAS-DERECHA RAPIDO (292.5º)
      atras_ruedaA_ruedaB(100, 88);
      break;
    case 13:	//DETRAS RAPIDO (270º)
      atras_ruedaA_ruedaB(100, 100);
      break;
    case 12:	//DETRAS-IZQUIERDA RAPIDO (247.5º)
      atras_ruedaA_ruedaB(50, 75);
      break;
    case 11:	//DETRAS-IZQUIERDA RAPIDO (225º)
      atras_ruedaA_ruedaB(75, 100);
      break;
    default:
      frena_motores();
  }
}

void conectar_al_servidor() {
  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
  // connected we set the connected flag to be true.
  if (conectar == true) {
    if (connectToServer()) {
      Serial.println("Conectado al servidor BLE!!");
    } else {
      Serial.println("Fallo al conectar con el servidor!!");
    }
    conectar = false;
  }
}

void inicializa_ble_coche() {
  Serial.println("Iniciando cliente BLE...");
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);

  frena_motores();
}

void control_coche() {
  conectar_al_servidor();

  if (conectado) {
    if (cambio_direccion) {
      cambio_direccion = false;
      ejecuta_movimiento();
    }

  } else if (scanear) {
    BLEDevice::getScan()->start(0);
  }
  delay(1);
}
