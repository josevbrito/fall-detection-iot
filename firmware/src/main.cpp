#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "wifi_manager.h"
#include "sensor.h"
#include "fall_detector.h"

// Função de setup
void setup() {
  // Inicializa comunicação serial
  Serial.begin(115200);
  delay(1000);

  // Banner de inicialização
  Serial.println("==========================================");
  Serial.println("  Fall Detection IoT — ESP32");
  Serial.println("  Algoritmo de detecção");
  Serial.println("==========================================");

  // Conecta à rede Wi-Fi
  wifi_connect();

  // Inicializa o estado do detector de queda
  Serial.println("\n[Detector] Monitorando quedas...\n");
}

// Função de loop principal
void loop() {
  // Verifica e reconecta Wi-Fi se necessário
  wifi_check_reconnect();

  // Leitura do sensor e processamento de dados
  AccelData data = sensor_read(); 
  sensor_print(data);

  // Atualiza o estado do detector de queda com os novos dados
  FallEvent event = fall_detector_update(data);
  fall_detector_print(event);

  // Aguarda o próximo ciclo de leitura
  delay(SAMPLE_INTERVAL_MS);
}
