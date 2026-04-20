#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "wifi_manager.h"
#include "sensor.h"
#include "fall_detector.h"
#include "mqtt_manager.h"

// Variáveis para controle de publicação de telemetria
unsigned long lastTelemetryTime = 0;
const unsigned long TELEMETRY_INTERVAL_MS = 1000;

// Função de setup
void setup() {
  // Inicializa comunicação serial
  Serial.begin(115200);
  delay(1000);

  // Banner de inicialização
  Serial.println("==========================================");
  Serial.println("  Fall Detection IoT — ESP32");
  Serial.println("  Publicação MQTT");
  Serial.println("==========================================");

  // Conecta à rede Wi-Fi
  wifi_connect();
  // Conecta ao broker MQTT
  mqtt_connect();

  // Mensagem inicial
  Serial.println("\n[Sistema] Monitorando e publicando dados...\n");
}

// Função de loop principal
void loop() {
  // Verifica e reconecta Wi-Fi se necessário
  wifi_check_reconnect();
  // Verifica e reconecta MQTT se necessário
  mqtt_check_reconnect();

  // Leitura do sensor e processamento de dados
  AccelData data = sensor_read();
  sensor_print(data);

  // Atualiza o estado do detector de queda com os novos dados
  FallEvent event = fall_detector_update(data);
  fall_detector_print(event);

  // Publica alertas de queda e telemetria no broker MQTT
  if (event.detected) {
    mqtt_publish_alert(event);
  }

  // Publica telemetria em intervalos regulares
  unsigned long now = millis();
  if (now - lastTelemetryTime >= TELEMETRY_INTERVAL_MS) {
    mqtt_publish_telemetry(data);
    lastTelemetryTime = now;
  }

  // Aguarda o próximo ciclo de leitura
  delay(SAMPLE_INTERVAL_MS);
}
