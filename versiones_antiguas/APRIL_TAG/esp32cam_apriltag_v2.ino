#include <stdio.h>
//#include <WiFi.h>
#include "esp_log.h"

//esp32-camera
#include "esp_camera.h"
#define CAMERA_MODEL_AI_THINKER  // Has PSRAM
#include "camera_pins.h"

// apriltag
#include "apriltag.h"
#include "tag25h9.h"
#include "apriltag_pose.h"
#include "common/image_u8.h"
#include "common/zarray.h"


apriltag_detector_t *td = apriltag_detector_create();
apriltag_family_t *tf = tag25h9_create();


//VARIABLES PARA LA COMUNICACION
const char STX = '\x002';  //START OF TEXT
const char ETX = '\x003';  //END OF TEXT
const char ACK = '\x006';  //ACKNOWLEDGE
const char NAK = '\x015';  //NEGATIVE ACKNOWLEDGE
const char SYN = '\x016';  //SYNCHRONOUS IDLE
const char EOT = '\x004';  //END OF TRANSMISSION

int TAG = 0;
bool error_ack;
bool error_timeout;

struct accion_tag {
  char movimiento;
  int parametro1;
  //int parametro2;
} Accion;

//apriltag_detector_t *td = NULL; //apriltag_detector_create();
//apriltag_family_t *tf = NULL; //tag25h9_create();

//FUNCION PARA ESTABLECER LA CONEXION
int inicia_comunicacion(const int timeout) {
  digitalWrite(12, HIGH);
  Serial.write(SYN);

  return espera_ACK(timeout);
}

int termina_comunicacion(const int timeout) {
  int respuesta_movimiento = espera_ACK(timeout);
  if (respuesta_movimiento == 0) {
    Serial.write(EOT);
    digitalWrite(12, LOW);
    return 0;
  } else {
    Serial.write(EOT);
    return 1;
  }
  return -1;
}

int espera_ACK(int timeout) {
  unsigned long inicio = millis();
  while (!Serial.available() && (millis() - inicio) < timeout)
    ;
  if (Serial.available()) {
    if (Serial.read() == ACK) return 0;  //TODO OK
    else return 1;                       //ERROR DE ACK
  }
  return -1;  //NO RESPUESTA EXCEDE TIMEOUT
}


//FUNCION PARA ENVIAR LA ACCION
void envia_movimiento(byte *estructura_mov, int longitud) {
  Serial.write(STX);                       //Inicio de la trama
  Serial.write(estructura_mov, longitud);  //aqui va todo lo que es el movimiento
  //comproamos integridad
  uint16_t checksum = Fletcher16(estructura_mov, longitud);
  Serial.write((byte)checksum);
  Serial.write((byte)checksum >> 8);
  Serial.write(ETX);  //fin de trama
}

//COMPROBAR INTEGRIDAD DE LOS DATOS CON CHECKSUM FLETCHER
uint16_t Fletcher16(byte *datos, int cont) {
  uint8_t suma1 = 0;
  uint8_t suma2 = 0;

  for (size_t indice = 0; indice < cont; ++indice) {
    suma1 = suma1 + (uint8_t)datos[indice];
    suma2 = suma2 + suma1;
  }
  return (suma2 << 8) | suma1;
}

//INICIALIZA UNA ESTRUCTURA CON LOS DATOS DE LA ACCION SEGUN EL TAG
struct accion_tag crea_accion(char mov, int param1) {
  accion_tag st;
  st.movimiento = mov;
  st.parametro1 = param1;
  return st;
}

void comunicacion(const int timeout, int tag) {

  if (tag != 0) {
    int respuesta = inicia_comunicacion(timeout);
    //ACTIVAR INTERRUPCION - INICIA COMUNICACION
    if (respuesta == 0) {
      switch (TAG) {
        case 1:
          {
            accion_tag msj{ 'F', 75 };
            envia_movimiento((byte *)&msj, sizeof(msj));
            if (termina_comunicacion(timeout) == 1) envia_movimiento((byte *)&msj, sizeof(msj));
            break;
          }

        case 2:
          {
            accion_tag msj2{ 'B', 50 };
            envia_movimiento((byte *)&msj2, sizeof(msj2));
            if (termina_comunicacion(timeout) == 1) envia_movimiento((byte *)&msj2, sizeof(msj2));
            break;
          }

        case 3:
          {
            accion_tag msj3{ 'L', 50 };
            envia_movimiento((byte *)&msj3, sizeof(msj3));
            if (termina_comunicacion(timeout) == 1) envia_movimiento((byte *)&msj3, sizeof(msj3));
            break;
          }

        case 4:
          {
            accion_tag msj4{ 'R', 75 };
            envia_movimiento((byte *)&msj4, sizeof(msj4));
            if (termina_comunicacion(timeout) == 1) envia_movimiento((byte *)&msj4, sizeof(msj4));
            break;
          }

        case 5:
          {
            accion_tag msj5{ 'S', 75 };
            envia_movimiento((byte *)&msj5, sizeof(msj5));
            if (termina_comunicacion(timeout) == 1) envia_movimiento((byte *)&msj5, sizeof(msj5));
            break;
          }
      }
    }
  }
}




void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_GRAYSCALE;  //YUV422,GRAYSCALE,RGB565,JPEG

  config.frame_size = FRAMESIZE_QQVGA;  //Do not use sizes above QVGA when not JPEG
  config.fb_count = 2;

  if (psramFound()) {
    //config.frame_size = FRAMESIZE_QQVGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    //config.frame_size = FRAMESIZE_QQVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  //gpio_install_isr_service()
  //Inicializar la cámara
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  Serial.println("Camara iniciada");
  //debug
  Serial.printf("HEAP total: %d  ", ESP.getHeapSize());
  Serial.printf("HEAP libre: %d  ", ESP.getFreeHeap());
  Serial.printf("PSRAM total: %d  ", ESP.getPsramSize());
  Serial.printf("PSRAM libre: %d  ", ESP.getFreePsram());

  //esp_log_level_set("*", ESP_LOG_VERBOSE);


  // AprilTAG
  //apriltag_detector_destroy(td);
  //tag25h9_destroy(tf);
  apriltag_detector_add_family(td, tf);

  //struct apriltag_detector:
  td->nthreads = 1;
  td->quad_decimate = 1.0;
  td->quad_sigma = 0.0;
  td->refine_edges = 0;
  td->decode_sharpening = 0;
  td->debug = 0;


  //Estrutura para detector de la pose:
  //apriltag_detection_info_t info;
  /*info.det = det;
  info.tagsize 
  info.fx = fx;         
  info.fy = fy;
  info.cx = cx;
  info.cy = cy;*/

  //INTERRUPCIONES PARA EL PROTOCOLO
  //pinMode(12, OUTPUT);  //pin para avisar al ESP32
}

void loop() {


  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Fallo de captura");
    return;
  }
  

  image_u8_t *im = image_u8_create_stride(fb->width, fb->height, fb->width);
  /*image_u8_t im = {
    .width = fb->width,
    .height = fb->height,
    .stride = fb->width,
    .buf = fb->buf
  };*/


  zarray_t *detections = apriltag_detector_detect(td, im);
  Serial.println("despues detect");


  for (int i = 0; i < zarray_size(detections); i++) {
    apriltag_detection_t *det;
    zarray_get(detections, i, &det);

    // Hacer cosas con las detecciones aquí
    //Serial.printf("ID: %d, ", det->id);

    comunicacion(1000, det->id);
  }
  Serial.println("\n");

  // Para liberar la matriz y las detecciones que contiene
  apriltag_detections_destroy(detections);

  //devolver buffer
  esp_camera_fb_return(fb);
}
