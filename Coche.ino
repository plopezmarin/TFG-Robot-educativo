const int RAPIDO = 192;
const int LENTO = 96;

const int DELANTE = 1;
const int ATRAS = 2;

// SENSORES DE ULTRASONIDO    /////////////////////////////////////

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


// MOTORES  /////////////////////////////////////////////////////
//Pines
const int IN1Pin = 26;
const int IN2Pin = 27;
const int IN3Pin = 14;
const int IN4Pin = 12;

//Identificadores para el control de la velocidad
const int PWMChannelA = 0;
const int PWMChannelB = 1;
const int PWMPinA = 2;
const int PWMPinB = 15;

const int freq = 1000;
const int resolution = 8;

// FUNCIONES BÁSICAS

// Funcion para el control de los motores
void control_motores(char IN1, char IN2, char IN3, char IN4, int velocidad) {
  ledcWrite(PWMChannelA, velocidad);
  ledcWrite(PWMChannelB, velocidad);

  digitalWrite(IN1Pin, IN1);
  digitalWrite(IN2Pin, IN2);
  digitalWrite(IN3Pin, IN3);
  digitalWrite(IN4Pin, IN4);
}

//Funcion para inicializar el sensor de ultrasonidos
void inicializa_sensor_ultrasonidos(int trigpin, int echopin){
  pinMode(trigpin, OUTPUT);     // Trigger pin = salida
  pinMode(echopin, INPUT);      // Echo pin = entrada
}

//Funcion para inicializar el controlador de los motores
void inicializa_motores(){
  // Ponemos todos los pines como salida
  pinMode(IN1Pin, OUTPUT);  
  pinMode(IN2Pin, OUTPUT);  
  pinMode(IN3Pin, OUTPUT);  
  pinMode(IN4Pin, OUTPUT);  

  ledcSetup(PWMChannelA, freq, resolution);
  ledcSetup(PWMChannelB, freq, resolution);

  ledcAttachPin(PWMPinA, PWMChannelA);
  ledcAttachPin(PWMPinB, PWMChannelB);
  ledcWrite(PWMChannelA, 96);
  ledcWrite(PWMChannelB, 96);
  digitalWrite(IN1Pin, LOW);
  digitalWrite(IN2Pin, LOW);
  digitalWrite(IN3Pin, LOW);
  digitalWrite(IN4Pin, LOW);
}


void avanza(int direccion, int velocidad) {

  if (direccion == 1) {
    control_motores(HIGH, LOW, LOW, HIGH, velocidad);
  }

  if (direccion == 2) {
    control_motores(LOW, HIGH, HIGH, LOW, velocidad);
  }
}

void frena() {
  control_motores(LOW, LOW, LOW, LOW, 0);
}

// FUNCIÓN PARA OBTENER EL VALOR DEL SENSOR DE ULTRASONIDOS
int lectura_ultrasonidos(int sensor) {
  if (sensor == 1){
    Trigpin = trigger1;
    Echopin = echo1;
  } 
  else if (sensor == 2){
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

void setup() {
  //Serial.begin(115200);
  Serial.begin(9600);

  //Inicializar SENSOR ULTRASONIDOS
  inicializa_sensor_ultrasonidos(trigger1, echo1);
  inicializa_sensor_ultrasonidos(trigger2, echo2);

  // Inicializar motores
  inicializa_motores();
}

void loop() {


  while (lectura_ultrasonidos(1) > 20) {
    //Serial.println(lectura_ultrasonidos(1));
    avanza(DELANTE, LENTO);
  }
  frena();
 
  while (lectura_ultrasonidos(2) > 20) {
    //Serial.println(lectura_ultrasonidos(2));
    avanza(ATRAS, LENTO);
  }
frena();

  delay(50);
}