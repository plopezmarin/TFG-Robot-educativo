#include "Arduino.h"
#include "sensor_coche.h"
#include "accion_coche.h"


//SENSOR DE ULTRASONIDOS
Sensor_ultrasonidos::Sensor_ultrasonidos(int echo, int trig){  
	echo_pin = echo;
	trigger_pin = trig;
	
	pinMode(trigger_pin, OUTPUT);  // Trigger pin = salida
	pinMode(echo_pin, INPUT);   // Echo pin = entrada
}

int Sensor_ultrasonidos::lectura_ultrasonidos(){
	float distancia;
	long tiempo;
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
	return digitalRead(sigue_lineas_pin);
}

//CREACION DE LOS OBJETOS DE CADA SENSOR

//Sensores de ultrasonidos:
Sensor_ultrasonidos sensor_delantero(18, 5);
Sensor_ultrasonidos sensor_trasero(16, 17);

//Sensor_luminosidad:
Sensor_luminosidad sensor_1(32);

//Sensor sigue_lineas:
Sensor_sigue_lineas sensor_derecha(25);
//Sensor_sigue_lineas sensor_izquierda(33);
