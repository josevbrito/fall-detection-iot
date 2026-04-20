#pragma once

#include <Arduino.h>
#include "config.h"
#include "sensor.h"

// Estruturas e variáveis para o algoritmo de detecção de queda
enum FallState {
  FALL_IDLE,
  FALL_IMPACT_DETECTED,
  FALL_CONFIRMED
};

struct FallEvent {
  bool detected;
  unsigned long timestamp;
  float impact_magnitude;
};

// Variáveis de estado do detector de queda
static FallState fallState = FALL_IDLE; // estado inicial do detector
static unsigned long impactTime = 0; // timestamp do impacto detectado
static float impactMagnitude = 0.0f; // magnitude do impacto detectado

// Função para atualizar o estado do detector de queda com base nos dados do acelerômetro
FallEvent fall_detector_update(const AccelData& data) {
  FallEvent event = { false, 0, 0.0f }; // evento de queda inicializado como não detectado

  // Lógica de detecção de queda baseada em estados
  switch (fallState) {

    // Estado de repouso — aguardando impacto
    case FALL_IDLE:
      // Verifica se a magnitude da aceleração ultrapassa o limiar de queda
      if (data.magnitude >= FALL_THRESHOLD) {
        fallState = FALL_IMPACT_DETECTED;
        impactTime = data.timestamp;
        impactMagnitude = data.magnitude;
        Serial.printf("[Detector] Impacto detectado — magnitude: %.3f g\n", data.magnitude);
      }
      // Caso contrário, permanece no estado de repouso
      break;
    
    // Estado de impacto detectado — aguardando confirmação
    case FALL_IMPACT_DETECTED:
      // Verifica se a magnitude da aceleração permanece baixa (repouso) por um período suficiente para confirmar a queda
      if (data.magnitude <= REST_THRESHOLD) {
        unsigned long restDuration = data.timestamp - impactTime; // calcula duração do repouso
        // Se o repouso for longo o suficiente, confirma a queda
        if (restDuration >= REST_DURATION_MS) {
          fallState = FALL_CONFIRMED;
          Serial.println("[Detector] QUEDA CONFIRMADA.");
        }
        // Caso contrário, continua aguardando confirmação
      } else if (data.magnitude >= FALL_THRESHOLD) {
        // novo pico — atualiza referência
        impactTime = data.timestamp;
        impactMagnitude = data.magnitude;
        Serial.printf("[Detector] Novo pico detectado — magnitude: %.3f g\n", data.magnitude); // reseta temporizador de confirmação
      } else {
        // movimento intermediário — reseta
        fallState = FALL_IDLE;
        Serial.println("[Detector] Movimento intermediário — resetando detector."); // reseta estado para aguardar novo impacto
      }
      break; // Estado de queda confirmada — gera evento e reseta para aguardar próximo impacto

    // Estado de queda confirmada — gera evento e reseta para aguardar próximo impacto
    case FALL_CONFIRMED:
      event.detected = true;
      event.timestamp = impactTime;
      event.impact_magnitude = impactMagnitude;
      fallState = FALL_IDLE;
      impactMagnitude = 0.0f;
      break;
  }

  return event;
}

// Função para imprimir informações do evento de queda no console serial
void fall_detector_print(const FallEvent& event) {
  if (event.detected) {
    Serial.println("==========================================");
    Serial.println("  ALERTA: QUEDA DETECTADA");
    Serial.printf("  Timestamp:  %lu ms\n", event.timestamp);
    Serial.printf("  Magnitude:  %.3f g\n", event.impact_magnitude);
    Serial.println("==========================================");
  }
}
