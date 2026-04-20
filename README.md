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
│   │   └── main.cpp        # Código principal do ESP32
│   ├── include/
│   │   └── config.h        # Credenciais e constantes (não versionado em produção)
│   └── platformio.ini      # Configuração do ambiente e dependências
├── docs/                   # Documentação técnica do projeto
└── README.md
```

## Configuração do Ambiente

1. Instale o [PlatformIO](https://platformio.org/)
2. Clone o repositório: `git clone https://github.com/milenafbn/fall-detection-iot`
3. Abra a pasta `firmware/` no PlatformIO
4. Copie `include/config.h`, preencha com suas credenciais Wi-Fi e HiveMQ
5. Faça o upload para o ESP32

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

- Etapa 1 - Firmware do ESP32 com sensor simulado
- [x] Etapa 1.1 — Estrutura do repositório
- [x] Etapa 1.2 — Configuração do ambiente (PlatformIO)
- [x] Etapa 1.3 — Conexão Wi-Fi
- [ ] Etapa 1.4 — Simulação do MPU6050
- [ ] Etapa 1.5 — Algoritmo de detecção de queda
- [ ] Etapa 1.6 — Publicação MQTT
- [ ] Etapa 1.7 — Teste local com Serial Monitor
