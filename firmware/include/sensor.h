#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>
#include "config.h"

static MPU6050 mpu;
static bool sensorReady = false;

// Estrutura para armazenar dados de aceleração
struct AccelData {
  float x;
  float y;
  float z;
  float magnitude;
  unsigned long timestamp;
};

// Protótipos das funções
float accel_magnitude(float x, float y, float z) {
  return sqrt(x * x + y * y + z * z);
}

// Inicializa o sensor MPU6050
bool sensor_init() {
  // Inicia comunicação I2C e configura o MPU6050
  Wire.begin(21, 22);
  mpu.initialize();
  mpu.setSleepEnabled(false);

  // Verifica se o sensor está respondendo
  if (!mpu.testConnection()) {
    Serial.println("[Sensor] ERRO: MPU6050 nao encontrado. Verifique a ligacao.");
    sensorReady = false;
    return false;
  }

  // Configura o sensor para a escala de aceleração e filtro desejados
  mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_4);
  mpu.setDLPFMode(MPU6050_DLPF_BW_20);

  // Marca o sensor como pronto para leitura
  sensorReady = true;
  Serial.println("[Sensor] MPU6050 inicializado com sucesso.");
  Serial.println("[Sensor] Escala: +/- 4g | Filtro: 20Hz");
  return true;
}

// Lê os dados de aceleração do sensor
AccelData sensor_read() {
  // Estrutura para armazenar os dados lidos
  AccelData data;
  data.timestamp = millis();

  // Verifica se o sensor está pronto antes de tentar ler os dados
  if (!sensorReady) {
    Serial.println("[Sensor] AVISO: sensor nao inicializado. Retornando zeros.");
    data.x = 0; data.y = 0; data.z = 0; data.magnitude = 0;
    return data;
  }

  // Variáveis para armazenar os valores brutos de aceleração
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Converte para g (escala +/-4g = 8192 LSB/g)
  data.x = ax / 8192.0f;
  data.y = ay / 8192.0f;
  data.z = az / 8192.0f;
  data.magnitude = accel_magnitude(data.x, data.y, data.z);

  return data;
}

// Imprime os dados de aceleração no console serial
void sensor_print(const AccelData& data) {
  Serial.printf("[Sensor] X: %6.3f g | Y: %6.3f g | Z: %6.3f g | Magnitude: %6.3f g\n",
    data.x, data.y, data.z, data.magnitude);
}
