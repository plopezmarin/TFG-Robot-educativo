/*
LIBRERIA PARA EL BLUETOOTH - PARTE DEL MANDO (CLIENTE)
*/

#include "BLEDevice.h"
#include "ble_cliente_mando.h"
//joystick
const int SW_pin = 25; // digital pin connected to switch output
const int X_pin = 35; // analog pin connected to X output
const int Y_pin = 34; // analog pin connected to Y output

// The remote service we wish to connect to.
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");

// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

static bool doConnect = false;
static bool connected = false;
static bool doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;


static BLEAdvertisedDevice* myDevice;

 
String envio_movimiento;


bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    //pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");
    //pClient->setMTU(517); //set client to request maximum MTU from server (default is 23 otherwise)
  
    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
   
    connected = true;
    return true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks

void setup_mando() {
    pinMode(SW_pin, INPUT);
    digitalWrite(SW_pin, HIGH);
    Serial.begin(115200);
    Serial.println("Starting Arduino BLE Client application...");
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
}


void conectar_al_servidor(){
	// If the flag "doConnect" is true then we have scanned for and found the desired
	// BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
	// connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }
}


void control_joystick(){
	// If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {

    int x = analogRead(X_pin);
	
    if (x < 400){
      envio_movimiento = "RH";
    }
    else if (x <1700){
      envio_movimiento = "RL";
    }
    else if (x < 2100){
      envio_movimiento = "SS";
    }else if (x < 3800){
      envio_movimiento = "FL";
    }
    else envio_movimiento = "FH";

	int y = analogRead(Y_pin);
    if (y < 400){
      envio_movimiento = envio_movimiento + "LH";
    }
    else if (y <1700){
      envio_movimiento = envio_movimiento + "LL";    
    }
    else if (y < 2100){
      envio_movimiento = envio_movimiento + "SS";
    }else if (y < 3800){
      envio_movimiento= envio_movimiento + "RL";
    }
    else envio_movimiento= envio_movimiento + "RH";
    Serial.println(envio_movimiento);
    pRemoteCharacteristic->writeValue(envio_movimiento.c_str(), envio_movimiento.length());
    
  }else if(doScan){
    BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
  }
  
  delay(50); // Delay a second between loops.

}