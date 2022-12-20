#include <stdio.h>
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
bool flag_fin_accion = true;

//prueba
int tagid = 0;

struct accion_tag {
  char movimiento;
  int parametro1;
  int parametro2;
} Accion;


//FUNCION PARA ESTABLECER LA CONEXION
void inicia_comunicacion(const int timeout, int tag) {
  if (flag_fin_accion = true) {
    digitalWrite(12, HIGH);
    delay(5);
    Serial.write(SYN);
    flag_fin_accion = false;
    int ack = espera_ACK(timeout);
    if (ack == 0) {
      comunicacion(timeout, tag);
    } else if (ack == 1) {
      //error
      comunicacion(timeout, tag);
    }
  }
  flag_fin_accion = false;
}

int termina_comunicacion(const int timeout) {
  int respuesta_movimiento = espera_ACK(timeout);
  flag_fin_accion = true;
  if (respuesta_movimiento == 0) {
    Serial.write(EOT);
    digitalWrite(12, LOW);
    flag_fin_accion = true;
    return 0;
  } else {
    return 1;
  }
  return -1;
  Serial.write(EOT);
  digitalWrite(12, LOW);
  flag_fin_accion = true;
}

int espera_ACK(int timeout) {
  unsigned long inicio = millis();
  while (!Serial.available() && (millis() - inicio) < timeout)
    ;
  if (Serial.available()) {
    if (Serial.read() == ACK) return 0;  //TODO OK
    else {
      return 1;  //ERROR DE ACK
    }
  }
  Serial.write(EOT);
  return -1;  //NO RESPUESTA EXCEDE TIMEOUT
}


//FUNCION PARA ENVIAR LA ACCION
void envia_movimiento(byte *estructura_mov, int longitud) {
  Serial.write(STX);                       //Inicio de la trama
  Serial.write(estructura_mov, longitud);  //aqui va todo lo que es el movimiento
                                           //Calculo y envio del checksum
  uint8_t checksum = Checksum(estructura_mov, longitud);
  Serial.write((byte)checksum);
  Serial.write(ETX);  //fin de trama
}

//COMPROBAR INTEGRIDAD DE LOS DATOS CON CHECKSUM XOR
uint8_t Checksum(const byte *datos, size_t longitud) {
  uint8_t valor = 0;
  for (size_t i = 0; i < longitud; i++) {
    valor ^= (uint8_t)datos[i];
  }
  return ~valor;
}


void comunicacion(const int timeout, int tag) {
  int end;
  switch (tag) {
    case 0:
      {
        accion_tag msj{ 'F', 75, 5 };
        envia_movimiento((byte *)&msj, sizeof(msj));
        end = termina_comunicacion(timeout);
        if (end == 1) envia_movimiento((byte *)&msj, sizeof(msj));
        break;
      }

    case 1:
      {
        accion_tag msj2{ 'B', 50, 5 };
        envia_movimiento((byte *)&msj2, sizeof(msj2));
        end = termina_comunicacion(timeout);
        if (end == 1) envia_movimiento((byte *)&msj2, sizeof(msj2));
        break;
      }

    case 2:
      {
        accion_tag msj3{ 'L', 5, 5 };
        envia_movimiento((byte *)&msj3, sizeof(msj3));
        end = termina_comunicacion(timeout);
        if (end == 1) envia_movimiento((byte *)&msj3, sizeof(msj3));
        break;
      }

    case 3:
      {
        accion_tag msj4{ 'R', 5, 5 };
        envia_movimiento((byte *)&msj4, sizeof(msj4));
        end = termina_comunicacion(timeout);
        if (end == 1) envia_movimiento((byte *)&msj4, sizeof(msj4));
        break;
      }

    case 4:
      {
        accion_tag msj5{ 'S', 0, 0 };
        envia_movimiento((byte *)&msj5, sizeof(msj5));
        end = termina_comunicacion(timeout);
        if (end == 1) envia_movimiento((byte *)&msj5, sizeof(msj5));
        break;
      }
    default:
      termina_comunicacion(timeout);
  }
}




void setup() {
  Serial.begin(9600);
  flag_fin_accion = true;

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
  config.fb_count = 1;

  //Inicializar la cámara
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // AprilTAG
  apriltag_detector_add_family(td, tf);

  //struct apriltag_detector:
  td->nthreads = 1;
  td->quad_decimate = 1.0;
  td->quad_sigma = 0.8;
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
  pinMode(12, OUTPUT);  //pin para avisar al ESP32
}

void loop() {
  tagid = 11;

  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Fallo de captura");
    return;
  }


  //image_u8_t *im = image_u8_create_stride(fb->width, fb->height, fb->width);
  image_u8_t im = {
    .width = fb->width,
    .height = fb->height,
    .stride = fb->width,
    .buf = fb->buf
  };


  zarray_t *detections = apriltag_detector_detect(td, &im);
  //Serial.println("despues detect");


  for (int i = 0; i < zarray_size(detections); i++) {
    apriltag_detection_t *det;
    zarray_get(detections, i, &det);

    // Hacer cosas con las detecciones aquí

    tagid = det->id;
  }
  //Serial.print(tagid);
  if (tagid < 9) {
    flag_fin_accion == true;
    inicia_comunicacion(500, tagid);
  }

  tagid = 10;

  apriltag_detections_destroy(detections);

  //devolver buffer
  esp_camera_fb_return(fb);
}
