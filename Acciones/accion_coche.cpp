#include "Arduino.h"
#include "accion_coche.h"

// FUNCIONES BÁSICAS

// Funcion para el control de los motores
void control_motorA(char IN1, char IN2, long velocidad) {
  ledcWrite(PWMChannelA, velocidad);

  digitalWrite(IN1Pin, IN1);
  digitalWrite(IN2Pin, IN2);
}

void control_motorB(char IN3, char IN4, long velocidad) {
  ledcWrite(PWMChannelB, velocidad);

  digitalWrite(IN3Pin, IN3);
  digitalWrite(IN4Pin, IN4);
}


// FUNCIONES INTERMEDIAS, NOMBRE PROVISIONAL
void avanza_delante(int velocidad, int tiempo) {
  //velocidad = map(velocidad, 0, 255, 0, 100); //ajustar rango a 0-100
  velocidad = (velocidad * 255) / 100;
  control_motorA(HIGH, LOW, velocidad);
  //cambiar luego si eso, es asi por los pines
  control_motorB(LOW, HIGH, velocidad);
}

void avanza_atras(int velocidad, int tiempo) {
  velocidad = (velocidad * 255) / 100;
  //velocidad = map(velocidad, 0, 255, 0, 100); //ajustar rango a 0-100
  control_motorA(LOW, HIGH, velocidad);
  control_motorB(HIGH, LOW, velocidad);
}

void frena_motores() {
  control_motorA(LOW, LOW, 0);
  control_motorB(LOW, LOW, 0);
}

void gira_derecha() {
  control_motorA(HIGH, LOW, 96);
  control_motorB(LOW, LOW, 0);
}

void gira_izquierda() {
  control_motorA(LOW, LOW, 0);
  control_motorB(HIGH, LOW, 96);
}

//Funcion para inicializar el controlador de los motores
void inicializa_motores() {
  // Ponemos todos los pines como salida
  pinMode(IN1Pin, OUTPUT);
  pinMode(IN2Pin, OUTPUT);
  pinMode(IN3Pin, OUTPUT);
  pinMode(IN4Pin, OUTPUT);

  //Configurar la señal PWM
  ledcSetup(PWMChannelA, freq, resolution);
  ledcSetup(PWMChannelB, freq, resolution);

  // GPIO donde se obtiene la señal y el canal
  ledcAttachPin(PWMPinA, PWMChannelA);
  ledcAttachPin(PWMPinB, PWMChannelB);
}

//FUNCIONES PARA EL MOVIMIENTO DEL COCHE
// SENTIDO: DELANTE / ATRAS
// VELOCIDAD: RAPIDO / LENTO
// TIEMPO: EN SEGUNDOS
void avanza(int sentido, int velocidad, int tiempo) {
  // NOTA: Para el mando se puede usar la funcion map()

  if (sentido == 1) {
    avanza_delante(velocidad, tiempo);
  }

  if (sentido == 2) {
    avanza_atras(velocidad, tiempo);
  }
  // el tiempo indicado y parar
  vTaskDelay((tiempo * 1000) / portTICK_PERIOD_MS);
  /*delay(tiempo*1000);*/
  frena();
}

//DIRECCION: IZQUIERDA / DERECHA
void gira(int direccion, int tiempo) {
  if (direccion == 1) {
    gira_derecha();
    vTaskDelay((tiempo * 1000) / portTICK_PERIOD_MS);
    frena();
  }
  if (direccion == 2) {
    gira_izquierda();
    vTaskDelay((tiempo * 1000) / portTICK_PERIOD_MS);
    frena();
  }
}


void frena() {
  frena_motores();
  vTaskDelay(500 / portTICK_PERIOD_MS);
  //delay(500);
}
