/*
LIBRERIA PARA LOS SENSORES DEL COCHE
también incluye el modo seguro del sensor de ultrasonidos
*/

#ifndef sensor_h
#define sensor_h
 
#include "Arduino.h"

//IDENTIFICADORES DE TAREAS
extern TaskHandle_t Tarea1;


// SEMAFOROS
extern SemaphoreHandle_t semaforo;

// ESTA FUNCION SIRVE PARA CREAR UN SEMAFORO:
void crea_semaforo(); 

// BLOQUEO E.M.
void lock();

// DESBLOQUEO E.M.
void unlock(); 

// SENSORES DE ULTRASONIDO
// Pines
extern const int trigger1;
extern const int echo1;
extern const int trigger2;
extern const int echo2;

// variables auxiliares
extern float distancia;
extern long tiempo;

extern int Trigpin;
extern int Echopin;

// SENSOR DE LUMINOSIDAD
extern int luminosidad;
extern const int LUMIPin;

//FUNCIONES
void inicializa_sensor_ultrasonidos(int trigpin, int echopin);
int lectura_ultrasonidos(int sensor);
int lectura_luminosidad();

void tarea_ultrasonidos(void *parameter);
void setup_tarea();


#endif