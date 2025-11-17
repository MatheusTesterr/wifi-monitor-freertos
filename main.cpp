#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

const char* redesSeguras[5] = {
  "Casa",
  "Empresa",
  "LabIF",
  "MeuHotspot",
  "RedeSegura123"
};

QueueHandle_t filaRedes;
QueueHandle_t filaAlertas;
SemaphoreHandle_t semaforoLista;

String redeAtual = "Empresa";

bool redeEhSegura(const char* rede) {
  xSemaphoreTake(semaforoLista, portMAX_DELAY);
  bool segura = false;
  for (int i = 0; i < 5; i++) {
    if (strcmp(rede, redesSeguras[i]) == 0) {
      segura = true;
      break;
    }
  }
  xSemaphoreGive(semaforoLista);
  return segura;
}

void WiFiMonitorTask(void *param) {
  while (true) {
    const char* r = redeAtual.c_str();
    xQueueSend(filaRedes, &r, portMAX_DELAY);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}

void NetworkCheckerTask(void *param) {
  const char* redeRecebida;
  while (true) {
    if (xQueueReceive(filaRedes, &redeRecebida, portMAX_DELAY) == pdTRUE) {
      if (!redeEhSegura(redeRecebida)) {
        xQueueSend(filaAlertas, &redeRecebida, portMAX_DELAY);
      }
    }
  }
}

void AlertTask(void *param) {
  const char* alertaRede;
  while (true) {
    if (xQueueReceive(filaAlertas, &alertaRede, portMAX_DELAY) == pdTRUE) {
      Serial.print("ALERTA: Rede nao autorizada detectada: ");
      Serial.println(alertaRede);
    }
  }
}

void setup() {
  Serial.begin(115200);

  filaRedes = xQueueCreate(5, sizeof(char*));
  filaAlertas = xQueueCreate(5, sizeof(char*));
  semaforoLista = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(WiFiMonitorTask, "Monitor", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(NetworkCheckerTask, "Checker", 4096, NULL, 2, NULL, 0);
  xTaskCreatePinnedToCore(AlertTask, "Alert", 4096, NULL, 3, NULL, 1);
}

void loop() {}
