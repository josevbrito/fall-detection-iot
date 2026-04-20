#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "wifi_manager.h"

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("==========================================");
  Serial.println("  Fall Detection IoT — ESP32");
  Serial.println("  Conexão Wi-Fi");
  Serial.println("==========================================");

  wifi_connect();

  Serial.println("\n[OK] Wi-Fi configurado com sucesso.");
  Serial.println("Pronto para a Simulação do MPU6050.");
}

void loop() {
  wifi_check_reconnect();
  delay(5000);
}
