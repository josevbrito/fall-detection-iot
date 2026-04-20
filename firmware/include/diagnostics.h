#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"

void diagnostics_print(const PubSubClient& client) {
  Serial.println("\n==========================================");
  Serial.println("  DIAGNÓSTICO DO SISTEMA");
  Serial.println("==========================================");

  // Wi-Fi
  Serial.print("[WiFi]   Status:     ");
  Serial.println(WiFi.status() == WL_CONNECTED ? "CONECTADO" : "DESCONECTADO");
  Serial.printf("[WiFi]   IP:         %s\n", WiFi.localIP().toString().c_str());
  Serial.printf("[WiFi]   RSSI:       %d dBm\n", WiFi.RSSI());

  // MQTT
  Serial.print("[MQTT]   Status:     ");
  Serial.println(client.connected() ? "CONECTADO" : "DESCONECTADO");
  Serial.printf("[MQTT]   Broker:     %s:%d\n", MQTT_BROKER, MQTT_PORT);
  Serial.printf("[MQTT]   Client ID:  %s\n", MQTT_CLIENT_ID);

  // Tópicos
  Serial.printf("[MQTT]   Telemetria: %s\n", TOPIC_TELEMETRY);
  Serial.printf("[MQTT]   Alertas:    %s\n", TOPIC_ALERT);

  // Parâmetros de detecção
  Serial.println("\n[Detector] Parâmetros configurados:");
  Serial.printf("[Detector] Limiar de queda:   %.1f g\n", FALL_THRESHOLD);
  Serial.printf("[Detector] Limiar de repouso: %.1f g\n", REST_THRESHOLD);
  Serial.printf("[Detector] Duração repouso:   %d ms\n", REST_DURATION_MS);
  Serial.printf("[Detector] Intervalo leitura: %d ms\n", SAMPLE_INTERVAL_MS);

  Serial.println("==========================================\n");
}
