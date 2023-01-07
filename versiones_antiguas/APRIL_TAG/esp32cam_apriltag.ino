#include <stdio.h>
#include <WiFi.h>

//esp32-camera
#include "esp_camera.h"
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"

// apriltag
#include "apriltag.h"
#include "tag25h9.h"
#include "common/image_u8.h"
#include "common/zarray.h"


apriltag_detector_t *td = apriltag_detector_create();
apriltag_family_t *tf = tag25h9_create();


void setup() {
  Serial.begin(9600);

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

  config.frame_size = FRAMESIZE_QQVGA;        //Do not use sizes above QVGA when not JPEG
  config.fb_count = 1;

  //Inicializar la cámara
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  Serial.println("Camara iniciada");


// AprilTAG
apriltag_detector_add_family(td, tf);

//struct apriltag_detector:
td->nthreads = 1;
td->quad_decimate = 1.0;
td->quad_sigma = 0.8;
td->refine_edges = 0;
td->decode_sharpening = 0;
td->debug = 0;

}

void loop() {

  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Fallo de captura");
    return;
  }

  image_u8_t im = {
    .width = fb->width,
    .height = fb->height,
    .stride = fb->width,
    .buf = fb->buf
  };

  //detecta etiquetas de una imagen y las devulve en una matriz zarray
  zarray_t *detections = apriltag_detector_detect(td, &im);

  for (int i = 0; i < zarray_size(detections); i++) {
    apriltag_detection_t *det;
    zarray_get(detections, i, &det);

    // Hacer cosas con las detecciones aquí
    Serial.printf("ID: %d, ", det->id);
  }
  Serial.println("\n");

  // Para liberar la matriz y las detecciones que contiene
  apriltag_detections_destroy(detections);

  //devolver buffer
  esp_camera_fb_return(fb);
}



