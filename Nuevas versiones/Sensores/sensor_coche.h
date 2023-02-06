/*
LIBRERIA PARA LOS SENSORES DEL COCHE
también incluye el modo seguro del sensor de ultrasonidos
*/

#ifndef sensor_h
#define sensor_h
 
#include "Arduino.h"

// variables auxiliares
//extern float distancia;
//extern long tiempo;


//FUNCIONES
/* Clase para el sensor de ultrasonidos, para añadir un nuevo sensor solo tendremos que crear un nuevo objeto
 de tipo Sensor_ultrasonidos y pasarle como parámetros los pines de ECHO y TRIGGER.
 esta clase proporciona un método para obtener una lectura del sensor*/
class Sensor_ultrasonidos {
private:
	int echo_pin;
	int trigger_pin;
	float distancia;
	long tiempo;
public:
	Sensor_ultrasonidos(int echo, int trig);			//constructor
	int lectura_ultrasonidos();		//funcion externa - lectura del sensor
};

/* Clase para el sensor de luminosidad, para añadir un nuevo sensor solo tendremos que crear un nuevo objeto
 de tipo Sensor_luminosidad y pasarle como parámetro el pin correspondiente.
 esta clase proporciona un método para obtener una lectura del sensor*/
class Sensor_luminosidad {
private:
	int luminosidad_pin;
public:
	Sensor_luminosidad(int lumin_pin);			//constructor
	int lectura_luminosidad();		//funcion externa - lectura del sensor
};

/* Clase para el sensor sigue líneas, para añadir un nuevo sensor solo tendremos que crear un nuevo objeto
 de tipo Sensor_sigue_lineas y pasarle como parámetro su pin correspondiente.
 esta clase proporciona un método para obtener una lectura del sensor, devuelve 0 si detecta negro y 1 si detecta blanco*/
class Sensor_sigue_lineas {
private:
	int sigue_lineas_pin;
public:
	Sensor_sigue_lineas(int lineas_pin);			//constructor
	int lectura_linea();		//funcion externa - lectura del sensor
};

//Definición de sensores de ultrasonidos: 
extern Sensor_ultrasonidos sensor_delantero;
extern Sensor_ultrasonidos sensor_trasero;

//Definición de sensores de luminosidad:
extern Sensor_luminosidad sensor_1;

//Definición de sensores sigue lineas:
extern Sensor_sigue_lineas sensor_derecha;
extern Sensor_sigue_lineas sensor_izquierda;
#endif