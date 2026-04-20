# Monitoramento de Quedas com IoT

Sistema distribuído para detecção automática de quedas utilizando Internet das Coisas (IoT), desenvolvido para a disciplina de Sistemas Distribuídos — Engenharia da Computação, UFMA, 2026.

## Visão Geral

O projeto implementa uma arquitetura distribuída em três camadas para detectar eventos de queda em tempo real e notificar cuidadores ou responsáveis automaticamente:

- **Edge Layer**: coleta de dados inerciais e pré-processamento local com ESP32 + MPU6050
- **Communication Layer**: transmissão leve e eficiente via protocolo MQTT (HiveMQ)
- **Cloud/Middleware Layer**: armazenamento, regras de alerta e dashboard com ThingsBoard

## Fluxo do Sistema

```
MPU6050 --(I²C)--> ESP32 --(MQTT/Wi-Fi)--> HiveMQ Broker --(MQTT)--> ThingsBoard --> Dashboard + Alertas --> Cuidador
```

## Tecnologias

| Camada | Tecnologia |
|---|---|
| Microcontrolador | ESP32 |
| Sensor | MPU6050 (acelerômetro + giroscópio) |
| Protocolo de comunicação | MQTT |
| Broker | HiveMQ Cloud |
| Middleware / Dashboard | ThingsBoard |
| Ambiente de desenvolvimento | PlatformIO |
| Versionamento | Git + GitHub |

## Estrutura do Repositório

```
fall-detection-iot/
├── firmware/
│   ├── src/
│   │   └── main.cpp            # Código principal do ESP32
│   ├── include/
│   │   ├── config.h            # Credenciais e constantes (não versionado em produção)
│   │   ├── wifi_manager.h      # Conexão e reconexão Wi-Fi
│   │   ├── sensor.h            # Simulação do MPU6050 (três estados)
│   │   ├── fall_detector.h     # Algoritmo de detecção de queda (FSM)
│   │   ├── mqtt_manager.h      # Publicação MQTT via HiveMQ
│   │   └── diagnostics.h       # Diagnóstico do sistema na inicialização
│   └── platformio.ini          # Configuração do ambiente e dependências
├── docs/                       # Documentação técnica do projeto
└── README.md
```

## Configuração do Ambiente

1. Instale o [PlatformIO](https://platformio.org/)
2. Clone o repositório:
   ```
   git clone https://github.com/milenafbn/fall-detection-iot
   ```
3. Abra a pasta `firmware/` no PlatformIO
4. Crie o arquivo `firmware/include/config.h` com suas credenciais:
   ```cpp
   #define WIFI_SSID      "SEU_SSID"
   #define WIFI_PASSWORD  "SUA_SENHA"
   #define MQTT_BROKER    "SEU_BROKER.hivemq.cloud"
   #define MQTT_PORT      8883
   #define MQTT_USER      "SEU_USUARIO"
   #define MQTT_PASSWORD  "SUA_SENHA_MQTT"
   ```
5. Faça o upload para o ESP32 via PlatformIO

## Como Testar

### Checklist de validação no Serial Monitor

Abra o Serial Monitor (115200 baud) e verifique:

- [ ] `[WiFi] Conectado com sucesso.` aparece na inicialização
- [ ] IP local é exibido corretamente
- [ ] `[MQTT] Conectado com sucesso.` aparece após o Wi-Fi
- [ ] O diagnóstico do sistema é impresso sem erros
- [ ] Leituras de telemetria aparecem a cada 1 segundo
- [ ] `[Detector] Impacto detectado` aparece durante a simulação de queda
- [ ] `ALERTA: QUEDA DETECTADA` é exibido após o repouso pós-queda
- [ ] `[MQTT] ALERTA DE QUEDA PUBLICADO NO BROKER` confirma o envio

### Verificando no HiveMQ

1. Acesse o painel do HiveMQ Cloud
2. Use o cliente MQTT integrado ou o MQTT Explorer
3. Inscreva-se nos tópicos:
   - `fall-detection/telemetry` — deve receber mensagens a cada 1 segundo
   - `fall-detection/alert` — deve receber uma mensagem a cada ciclo de queda simulado

### Exemplo de payload esperado

Telemetria:
```json
{
  "timestamp": 1200,
  "accel_x": 0.021,
  "accel_y": 0.031,
  "accel_z": 1.002,
  "magnitude": 1.003,
  "status": "normal"
}
```

Alerta de queda:
```json
{
  "timestamp": 12483,
  "status": "QUEDA_DETECTADA",
  "impact_magnitude": 4.187,
  "device_id": "esp32-fall-detector"
}
```

## Equipe

| Nome | Papel |
|---|---|
| Ana Iara Loayza Costa | Membro da equipe |
| Higor Pinheiro Costa | Membro da equipe |
| José Victor Brito Costa | Membro da equipe |
| Milena Freire Britto Neves | Membro da equipe |

Professor orientador: Prof. Dr. Luiz Henrique Neves Rodrigues  
Disciplina: Sistemas Distribuídos — CCET / UFMA

## Status do Projeto

- **[X] Etapa 1** - Firmware do ESP32 com sensor simulado
- [x] Etapa 1.1 — Estrutura do repositório
- [x] Etapa 1.2 — Configuração do ambiente (PlatformIO)
- [x] Etapa 1.3 — Conexão Wi-Fi
- [x] Etapa 1.4 — Simulação do MPU6050
- [x] Etapa 1.5 — Algoritmo de detecção de queda
- [x] Etapa 1.6 — Publicação MQTT
- [x] Etapa 1.7 — Teste local com Serial Monitor
