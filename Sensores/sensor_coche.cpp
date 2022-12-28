#include "Arduino.h"
#include "sensor_coche.h"
#include "accion_coche.h"

//TaskHandle_t Tarea1;
//SemaphoreHandle_t semaforo;

/*const int trigger1 = 5;
const int echo1 = 18;
const int trigger2 = 17;
const int echo2 = 16;

int Trigpin = 0;
int Echopin = 0;*/

float distancia;
long tiempo;


//int luminosidad = 0;
//const int LUMIPin = 35;

/*void crea_semaforo() {
  semaforo = xSemaphoreCreateMutex();
  xSemaphoreGive(semaforo);  //lo libera
}

// BLOQUEO E.M.
void lock() {
  xSemaphoreTake(semaforo, portMAX_DELAY);
}

// DESBLOQUEO E.M.
void unlock() {
  xSemaphoreGive(semaforo);
}*/

//Funcion para inicializar el sensor de ultrasonidos
/*void inicializa_sensor_ultrasonidos(int trigpin, int echopin) {
  pinMode(trigpin, OUTPUT);  // Trigger pin = salida
  pinMode(echopin, INPUT);   // Echo pin = entrada
}*/

// FUNCIÓN PARA OBTENER EL VALOR DEL SENSOR DE ULTRASONIDOS
/*int lectura_ultrasonidos(int sensor) {
  if (sensor == 1) {
    Trigpin = trigger1;
    Echopin = echo1;
  } else if (sensor == 2) {
    Trigpin = trigger2;
    Echopin = echo2;
  }

  //limpiar el pin
  digitalWrite(Trigpin, LOW);
  delayMicroseconds(2);

  // Enviar pulsos cada 10 useg
  digitalWrite(Trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigpin, LOW);

  // Leer el pin echo, devuelve el tiempo de viaje de la onda en mseg
  tiempo = pulseIn(Echopin, HIGH);

  // Calcular la distancia
  distancia = tiempo / 59;
  return distancia;
}*/

// FUNCION PARA EL SENSOR DE LUMINOSIDAD
/*int lectura_luminosidad() {
  luminosidad = analogRead(LUMIPin);
  return luminosidad;
}*/

/*void setup_tarea() {
    // CREACION DE LA TAREA PARA EL MULTIPROCESAMIENTO
    xTaskCreatePinnedToCore(
        tarea_ultrasonidos,   // Función para implemntar la tarea
        "TareaUltrasonidos",  //Nombre de la tarea
        10000,                // tamaño de la pila **ver que es
        NULL,                 // Parametro de entrada para la tarea, suele ser null
        2,                    // Prioridad de la tarea, 2 > que la del loop
        &Tarea1,              // Handle de la tarea
        0);                   // CORE donde se ejecuta
}

// TAREA PARA EL SENSOR DE ULTRASONIDOS, EN EL CORE 0
void tarea_ultrasonidos(void *parameter) {
  for (;;) {
    lock();

    if (lectura_ultrasonidos(1) < 20 || lectura_ultrasonidos(2)<20) {
      //peligro = true;
      frena();
      while (lectura_ultrasonidos(1) < 20)
        ;
    }
    unlock();
    vTaskDelay(5);  //bloquea la tarea 5 ticks
  }
}*/

//SENSOR DE ULTRASONIDOS
Sensor_ultrasonidos::Sensor_ultrasonidos(int echo, int trig){  
	echo_pin = echo;
	trigger_pin = trig;
	
	pinMode(trigger_pin, OUTPUT);  // Trigger pin = salida
	pinMode(echo_pin, INPUT);   // Echo pin = entrada
}

int Sensor_ultrasonidos::lectura_ultrasonidos(){

  //limpiar el pin
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);

  // Enviar pulsos cada 10 useg
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);

  // Leer el pin echo, devuelve el tiempo de viaje de la onda en mseg
  tiempo = pulseIn(echo_pin, HIGH);

  // Calcular la distancia
  distancia = tiempo / 59;
  return distancia;
}

//SENSOR DE LUMINOSIDAD
Sensor_luminosidad::Sensor_luminosidad(int lumin_pin){
	luminosidad_pin = lumin_pin;
}

int Sensor_luminosidad::lectura_luminosidad(){
	return analogRead(luminosidad_pin);
}

//SENSOR SIGUE_LINEAS
Sensor_sigue_lineas::Sensor_sigue_lineas(int lineas_pin){  
	sigue_lineas_pin = lineas_pin;
	
	pinMode(sigue_lineas_pin, INPUT);   // Sensor de infrarrojos como entrada
}

int Sensor_sigue_lineas::lectura_linea(){
	//delay(100);
	return digitalRead(sigue_lineas_pin);
}

//inicialización sensores de ultrasonidos:
Sensor_ultrasonidos sensor_delantero(18, 5);
Sensor_ultrasonidos sensor_trasero(16, 17);

//inicializa_sensor_luminosidad:
Sensor_luminosidad sensor_1(35);

//inicializa sensor sigue_lineas:
Sensor_sigue_lineas sensor_derecha(34);

