#include "Arduino.h"
#include "accion_coche.h"
#include "sensor_coche.h"
#include "ESP32Tone.h"


//FUNCION DE SETUP: Esta función incializa los motores 
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
  
  control_motorA(LOW,LOW,0);
  control_motorB(LOW,LOW,0);
}

// FUNCIONES BAJO NIVEL: Control del sentido de giro y velocidad de los motores, por separado
void control_motorA(char IN1, char IN2, long velocidad) {
  velocidad = (velocidad * 255) / 100;
  ledcWrite(PWMChannelA, velocidad);

  digitalWrite(IN1Pin, IN1);
  digitalWrite(IN2Pin, IN2);
}

void control_motorB(char IN3, char IN4, long velocidad) {
  velocidad = (velocidad * 255) / 100;
  ledcWrite(PWMChannelB, velocidad);

  digitalWrite(IN3Pin, IN3);
  digitalWrite(IN4Pin, IN4);
}


/* FUNCIONES NIVEL MEDIO: invocan las de nivel bajo para controlar los motores e 
implementan el control del tiempo de giro y los choques (sensor ultrasonidos)*/
void avanza_delante(int velocidad, int tiempo) {

  control_motorA(HIGH, LOW, velocidad);
  control_motorB(LOW, HIGH, velocidad);

  //avanza el tiempo indicado si no hay nada delante
  unsigned long inicio = millis();
  while (millis() - inicio < (tiempo * 1000)) {
    if (sensor_delantero.lectura_ultrasonidos() < 20) {
      break;
    }
  }
}

void avanza_atras(int velocidad, int tiempo) {

  control_motorA(LOW, HIGH, velocidad);
  control_motorB(HIGH, LOW, velocidad);

  //avanza el tiempo indicado si no hay nada delante
  unsigned long inicio = millis();
  while (millis() - inicio < (tiempo * 1000)) {
    if (sensor_trasero.lectura_ultrasonidos() < 20) {  
      break;
    }
  }
}

void frena_motores() {
  control_motorA(LOW, LOW, 0);
  control_motorB(LOW, LOW, 0);
}

void gira_derecha(int velocidad, int tiempo) {
  control_motorA(HIGH, LOW, velocidad);
  control_motorB(LOW, LOW, 0);
  
  vTaskDelay((tiempo * 1000) / portTICK_PERIOD_MS);
}

void gira_izquierda(int velocidad, int tiempo) {
  control_motorA(LOW, LOW, 0);
  control_motorB(LOW, HIGH, velocidad);
  
	vTaskDelay((tiempo * 1000) / portTICK_PERIOD_MS);
}


/*FUNCIONES DE NIVEL ALTO: Estas son las funciones que el usaurio usará rincipalmente.
Para cada una de ellas los parámteros son:
	SENTIDO: DELANTE / ATRAS
	VELOCIDAD: RAPIDO / LENTO
	TIEMPO: EN SEGUNDOS 							*/
void avanza(int sentido, int velocidad, int tiempo) {
  if (sentido == 1) {
    avanza_delante(velocidad, tiempo);
  }

  if (sentido == 2) {
    avanza_atras(velocidad, tiempo);
  }

  frena_motores();
}

//DIRECCION: IZQUIERDA / DERECHA
void gira(int direccion, int velocidad, int tiempo) {
  if (direccion == 1) {
    gira_derecha(velocidad, tiempo);
    frena_motores();
  }
  if (direccion == 2) {
    gira_izquierda(velocidad, tiempo);
    frena_motores();
  }
}


void frena(int tiempo) {
  frena_motores();
  vTaskDelay((tiempo * 1000) / portTICK_PERIOD_MS);
  //delay(500);
}

void toca_nota(int nota, int duracion) {
  tone2(23, nota, duracion * 1000);

}

void toca_melodia(int melodia[], int duracion, int cantidad) {

  for (int nota_actual = 0; nota_actual < cantidad; nota_actual++) {
    toca_nota(melodia[nota_actual], duracion);
    delay(1000);
    Serial.println(melodia[nota_actual]);
  }
}

