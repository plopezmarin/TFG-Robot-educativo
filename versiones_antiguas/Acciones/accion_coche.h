/*
LIBRERIA PARA LOS MOVIMIENTOS DEL COCHE
Esta librería incluye todos los movimientos/acciones
que puede realizar el coche
*/

#ifndef accion_h
#define accion_h
 
#include "Arduino.h"
#include "accion_coche.h"
#include "pitches.h"

//DEFINICION DE LAS CONSTANTES PARA LOS MOVIMIENTOS
const int DELANTE = 1;
const int ATRAS = 2;

const int DERECHA = 1;
const int IZQUIERDA = 2;

// MOTORES
//Pines
const int IN1Pin = 26;
const int IN2Pin = 27;
const int IN3Pin = 14;
const int IN4Pin = 12;

//Canales
const int PWMChannelA = 0;
const int PWMChannelB = 1;

//GPIO
const int PWMPinA = 2;
const int PWMPinB = 15;

// Para controlar la velocidad usando PWM
const int freq = 1000;
const int resolution = 8;

//FUNCIONES
//Estas dos funciones son las básicas para el control de los dos motores, permiten establecer el giro y la velocidad
void control_motorA(char IN1, char IN2, long velocidad);
void control_motorB(char IN3, char IN4, long velocidad);

//Avanzar hacia delante a la velocidad y el tiempo indicados
void avanza_delante(int velocidad, int tiempo);

//Retroceder a la velocidad y el tiempo indicados
void avanza_atras(int velocidad, int tiempo);

//detiene el giro de ambos motores
void frena_motores();

//control de motores para girar a la derecha
void gira_derecha();

//control de motores para girar a la izquierda
void gira_izquierda();

//Inicilizar los parámetros para que funcionen los motres
void inicializa_motores();

//ESTAS SON LAS FUNCIONES PARA EL USUARIO//
//Para avanzar indicar sentido: DELANTE o ATRAS, velocidad: 0-100, tiempo en segundos
void avanza(int sentido, int velocidad, int tiempo);

//Para girar indicar sentido: IZQUIERDA o DERECHA, tiempo en segundos
void gira(int direccion, int tiempo);

//Para frenar indicar tiempo en segundos (tiempo que este frenado)
void frena(int tiempo);

//Para tocar una nota indicar la nota (consultar libreria pitches.h) y la duración de esta
void toca_nota(int nota, int duracion);

//Para tocar una melodia, se debe crear un array de enteros que incluya las notas que queremos tocar (consultar libreria pitches.h)
//la duracion de las notas y cuantas notas hay en el array
// un ejemplo de array seria: int melody[] = {NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
// para indicar el tamaño, podemos pasarlo directamente nostros o si no lo sabemos exacto podemos obtenerlo con la siguente formula
// cantidad = sizeof(array)/sizeof(array[0])
void toca_melodia(int melodia[], int duracion, int cantidad);


#endif
