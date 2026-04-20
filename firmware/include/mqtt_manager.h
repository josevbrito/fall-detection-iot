#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "sensor.h"
#include "fall_detector.h"

// Variáveis globais para MQTT
static WiFiClientSecure espClient;
static PubSubClient mqttClient(espClient);

// Função de callback para mensagens MQTT recebidas
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.printf("[MQTT] Mensagem recebida no tópico: %s\n", topic);
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.printf("[MQTT] Conteúdo: %s\n", message.c_str());
}

// Função para conectar ao broker MQTT
bool mqtt_connect() {
  // Configurações de segurança para conexão MQTT
  espClient.setInsecure();
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(mqtt_callback);
  mqttClient.setBufferSize(512);

  // Tenta conectar ao broker MQTT
  Serial.printf("[MQTT] Conectando ao broker %s...\n", MQTT_BROKER);

  // Tenta conectar com um número limitado de tentativas
  int attempts = 0;

  // Loop de conexão com o broker MQTT
  while (!mqttClient.connected()) {
    // Tenta conectar usando as credenciais definidas
    if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("[MQTT] Conectado com sucesso.");
      return true;
    }

    // Se a conexão falhar, exibe o código de erro e tenta novamente após um breve atraso
    Serial.printf("[MQTT] Falha (rc=%d). Tentativa %d/5...\n", mqttClient.state(), attempts + 1);
    attempts++;

    // Se exceder o número máximo de tentativas, retorna false para indicar falha na conexão
    if (attempts >= 5) {
      Serial.println("[MQTT] Não foi possível conectar ao broker.");
      return false;
    }
    delay(2000); // Aguarda 2 segundos antes de tentar novamente
  }
  return true;
}

// Função para verificar e manter a conexão MQTT
void mqtt_check_reconnect() {
  // Se a conexão MQTT for perdida, tenta reconectar
  if (!mqttClient.connected()) {
    Serial.println("[MQTT] Conexão perdida. Reconectando...");
    mqtt_connect();
  }
  // Processa mensagens MQTT recebidas
  mqttClient.loop();
}


// Função para publicar telemetria no broker MQTT
void mqtt_publish_telemetry(const AccelData& data) {
  if (!mqttClient.connected()) return;

  StaticJsonDocument<256> doc;
  doc["timestamp"] = data.timestamp;
  doc["accel_x"]   = serialized(String(data.x, 3));
  doc["accel_y"]   = serialized(String(data.y, 3));
  doc["accel_z"]   = serialized(String(data.z, 3));
  doc["magnitude"] = serialized(String(data.magnitude, 3));
  doc["status"]    = "normal";

  char buffer[256];
  serializeJson(doc, buffer);

  mqttClient.publish(TOPIC_TELEMETRY, buffer);
  Serial.printf("[MQTT] Telemetria publicada: %s\n", buffer);
}

// Função para publicar alertas de queda no broker MQTT
void mqtt_publish_alert(const FallEvent& event) {
  if (!mqttClient.connected()) return;

  StaticJsonDocument<256> doc;
  doc["timestamp"]        = event.timestamp;
  doc["status"]           = "QUEDA_DETECTADA";
  doc["impact_magnitude"] = serialized(String(event.impact_magnitude, 3));
  doc["device_id"]        = MQTT_CLIENT_ID;

  char buffer[256];
  serializeJson(doc, buffer);

  mqttClient.publish(TOPIC_ALERT, buffer);

  Serial.println("[MQTT] ==========================================");
  Serial.println("[MQTT] ALERTA DE QUEDA PUBLICADO NO BROKER");
  Serial.printf("[MQTT] Tópico: %s\n", TOPIC_ALERT);
  Serial.printf("[MQTT] Payload: %s\n", buffer);
  Serial.println("[MQTT] ==========================================");
}
