#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include "config.h"

void wifi_connect() {
  Serial.printf("\n[WiFi] Conectando a %s", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    attempts++;
    if (attempts >= 20) {
      Serial.println("\n[WiFi] Falha ao conectar. Reiniciando...");
      ESP.restart();
    }
  }

  Serial.println("\n[WiFi] Conectado com sucesso.");
  Serial.printf("[WiFi] IP: %s\n", WiFi.localIP().toString().c_str());
  Serial.printf("[WiFi] RSSI: %d dBm\n", WiFi.RSSI());
}

void wifi_check_reconnect() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WiFi] Conexão perdida. Reconectando...");
    wifi_connect();
  }
}
