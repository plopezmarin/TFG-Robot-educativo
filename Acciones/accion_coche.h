/*
LIBRERIA PARA LOS MOVIMIENTOS DEL COCHE
Esta librería incluye todos los movimientos/acciones
que puede realizar el coche
*/

#ifndef accion_h
#define accion_h
 
#include "Arduino.h"
#include "accion_coche.h"

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
void control_motorA(char IN1, char IN2, long velocidad);
void control_motorB(char IN3, char IN4, long velocidad);
void avanza_delante(int velocidad, int tiempo);
void avanza_atras(int velocidad, int tiempo);
void frena_motores();
void gira_derecha();
void gira_izquierda();
void inicializa_motores();
void avanza(int sentido, int velocidad, int tiempo);
void gira(int direccion, int tiempo);
void frena();


#endif