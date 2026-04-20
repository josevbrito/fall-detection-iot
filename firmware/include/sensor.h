#pragma once

#include <Arduino.h>
#include "config.h"

struct AccelData {
  float x;
  float y;
  float z;
  float magnitude;
  unsigned long timestamp;
};

enum SimState {
  SIM_NORMAL,
  SIM_FALL,
  SIM_REST
};

static SimState currentSimState = SIM_NORMAL;
static unsigned long simStateStart = 0;
static int simCycle = 0;

float accel_magnitude(float x, float y, float z) {
  return sqrt(x * x + y * y + z * z);
}

AccelData sensor_read() {
  AccelData data;
  data.timestamp = millis();

  unsigned long elapsed = millis() - simStateStart;

  // Alterna entre estados: 4s normal → 0.3s queda → 2s repouso → repete
  if (currentSimState == SIM_NORMAL && elapsed > 4000) {
    currentSimState = SIM_FALL;
    simStateStart = millis();
    simCycle++;
    Serial.printf("\n[Sensor] Ciclo %d — simulando QUEDA\n", simCycle);
  } else if (currentSimState == SIM_FALL && elapsed > 300) {
    currentSimState = SIM_REST;
    simStateStart = millis();
    Serial.println("[Sensor] Simulando REPOUSO pós-queda");
  } else if (currentSimState == SIM_REST && elapsed > 2000) {
    currentSimState = SIM_NORMAL;
    simStateStart = millis();
    Serial.println("[Sensor] Voltando ao MOVIMENTO NORMAL");
  }

  switch (currentSimState) {
    case SIM_NORMAL:
      // Movimento normal: aceleração próxima a 1g com leve variação
      data.x = 0.05f * sin(millis() / 300.0f) + random(-5, 5) / 100.0f;
      data.y = 0.05f * cos(millis() / 300.0f) + random(-5, 5) / 100.0f;
      data.z = 1.0f + random(-8, 8) / 100.0f;
      break;

    case SIM_FALL:
      // Queda: pico alto de aceleração em todos os eixos
      data.x = 2.0f + random(0, 100) / 100.0f;
      data.y = 1.5f + random(0, 80) / 100.0f;
      data.z = 2.8f + random(0, 120) / 100.0f;
      break;

    case SIM_REST:
      // Repouso: aceleração mínima, quase sem movimento
      data.x = random(-5, 5) / 100.0f;
      data.y = random(-5, 5) / 100.0f;
      data.z = 0.05f + random(-3, 3) / 100.0f;
      break;
  }

  data.magnitude = accel_magnitude(data.x, data.y, data.z);
  return data;
}

void sensor_print(const AccelData& data) {
  Serial.printf("[Sensor] X: %6.3f g | Y: %6.3f g | Z: %6.3f g | Magnitude: %6.3f g\n",
    data.x, data.y, data.z, data.magnitude);
}
