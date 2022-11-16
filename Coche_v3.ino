//IDENTIFICADORES DE TAREAS
TaskHandle_t Tarea1;
//TaskHandle_t Tarea2;

bool peligro = false;

// SEMAFOROS
SemaphoreHandle_t semaforo;
// ESTA FUNCION SIRVE PARA CREAR UN SEMAFORO:
void crea_semaforo(){
  semaforo = xSemaphoreCreateMutex();
  xSemaphoreGive(semaforo);   //lo libera
}

// BLOQUEO E.M.
void lock(){
    xSemaphoreTake(semaforo, portMAX_DELAY);
}

// DESBLOQUEO E.M.
void unlock(){
    xSemaphoreGive(semaforo);
}

const int RAPIDO = 192;
const int LENTO = 96;

const int DELANTE = 1;
const int ATRAS = 2;

const int DERECHA = 1;
const int IZQUIERDA = 2;

// SENSORES DE ULTRASONIDO
// Pines
const int trigger1 = 5;
const int echo1 = 18;
const int trigger2 = 17;
const int echo2 = 16;

// variables auxiliares
float distancia;
long tiempo;

int Trigpin = 0;
int Echopin = 0;

// SENSOR DE LUMINOSIDAD
int luminosidad = 0;
const int LUMIPin = 35;


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
void avanza_delante(int velocidad) {
  //velocidad = map(velocidad, 0, 255, 0, 100); //ajustar rango a 0-100
  velocidad = (velocidad * 255)/100;
  control_motorA(HIGH, LOW, velocidad);
  //cambiar luego si eso, es asi por los pines
  control_motorB(LOW, HIGH, velocidad);
}

void avanza_atras(int velocidad) {
  velocidad = (velocidad * 255)/100;
  //velocidad = map(velocidad, 0, 255, 0, 100); //ajustar rango a 0-100
  control_motorA(LOW, HIGH, velocidad);
  control_motorB(HIGH, LOW, velocidad);
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
void avanza(int sentido, int velocidad) {
  // NOTA: Para el mando se puede usar la funcion map()

  if (sentido == 1) {
    avanza_delante(velocidad);
  }

  if (sentido == 2) {
    avanza_atras(velocidad);
  }
}

//DIRECCION: IZQUIERDA / DERECHA
void gira(int direccion, int tiempo) {
  if (direccion == 1) {
    gira_derecha();
    delay(tiempo);
  }
  if (direccion == 2) {
    gira_izquierda();
    delay(tiempo);
  }
}


void frena() {
  control_motorA(LOW, LOW, 0);
  control_motorB(LOW, LOW, 0);
  delay(500);
}

//Funcion para inicializar el sensor de ultrasonidos
void inicializa_sensor_ultrasonidos(int trigpin, int echopin) {
  pinMode(trigpin, OUTPUT);  // Trigger pin = salida
  pinMode(echopin, INPUT);   // Echo pin = entrada
}

// FUNCIÓN PARA OBTENER EL VALOR DEL SENSOR DE ULTRASONIDOS
int lectura_ultrasonidos(int sensor) {
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
}

// FUNCION PARA EL SENSOR DE LUMINOSIDAD
int lectura_luminosidad() {
  luminosidad = analogRead(LUMIPin);
  return luminosidad;
}

// TAREA PARA EL SENSOR DE ULTRASONIDOS, EN EL CORE 0
void tarea_ultrasonidos(void *parameter){
  for(;;){
    lock();
    if (lectura_ultrasonidos(1)<20 /*|| lectura_ultrasonidos(2)<20*/){
      //peligro = true; 
      frena();
      while(lectura_ultrasonidos(1)<20);
    } 
    unlock();
    vTaskDelay(100);    //bloquea la tarea 100 ticks
  }
}

void setup() {
  Serial.begin(9600);

  //Inicializar SENSOR ULTRASONIDOS
  inicializa_sensor_ultrasonidos(trigger1, echo1);
  inicializa_sensor_ultrasonidos(trigger2, echo2);

  // Inicializar motores
  inicializa_motores();

  // Crear semaforo
  crea_semaforo();

  // CREACION DE LA TAREA PARA EL MULTIPROCESAMIENTO
  xTaskCreatePinnedToCore(
    tarea_ultrasonidos,  // Función para implemntar la tarea
    "TareaUltrasonidos",   //Nombre de la tarea
    10000,               // tamaño de la pila **ver que es
    NULL,                // Parametro de entrada para la tarea, suele ser null
    1,                   // Prioridad de la tarea, 1 = que la del loop
    &Tarea1,             // Handle de la tarea
    0);                  // CORE donde se ejecuta
}


void loop() {
  lock();

  avanza(DELANTE, 70);

  /*if(peligro == true){
    frena();
    peligro = false;
  }*/
  unlock();
  vTaskDelay(5);

}