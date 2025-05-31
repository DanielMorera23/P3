
# PRÁCTICA 3  
# Conectividad Inalámbrica con ESP32: WiFi y Bluetooth

## Introducción

En esta práctica exploraremos las capacidades inalámbricas del microcontrolador ESP32, centrándonos en las tecnologías WiFi y Bluetooth. Se implementarán ejemplos de un servidor web accesible desde una red local y una conexión BLE para comunicación con un dispositivo móvil.

---

## PRÁCTICA A - CONEXIÓN A RED WIFI Y PÁGINA WEB INTERACTIVA

### Objetivo  
Establecer conexión WiFi desde el ESP32-S3 y habilitar un servidor web que sirva contenido accesible desde un navegador.

### Materiales necesarios  
- ESP32-S3

### Procedimiento

#### Código:

```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <Arduino.h>

const char* ssid = "Nautilus";  
const char* password = "20000Leguas";  
WebServer server(80);

void handle_root();

void setup() {
    Serial.begin(115200);
    Serial.println("Conectando a la red Wi-Fi...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\nConexión establecida");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());

    server.on("/", handle_root);
    server.begin();
    Serial.println("Servidor HTTP iniciado");
}

void loop() {
    server.handleClient();
}
```

### Descripción  
Una vez conectado, el ESP32 inicia un servidor web que responde a las solicitudes HTTP en la raíz ("/"). Desde un navegador, se puede acceder mediante la IP asignada.

### Salida esperada en el monitor serie:
```
Conexión WiFi establecida
Dirección IP: 192.168.12.241
Servidor HTTP iniciado
```

### Conclusión  
Se logró habilitar un servidor web sencillo alojado en el ESP32 y accesible desde cualquier dispositivo conectado a la misma red.

---

## Código para juego web (Blackjack)

```cpp
#include <WiFi.h>
#include <WebServer.h>

// SSID & Password
const char* ssid = "Nelfi";
const char* password = "12345678";

WebServer server(80);

// HTML, CSS y JavaScript para un juego de Blackjack
String HTML = "<!DOCTYPE html><html><head>    <meta charset='UTF-8'>    <meta name='viewport' content='width=device-width, initial-scale=1.0'>    <title>Blackjack con ESP32</title>    <style>        body { font-family: Arial, sans-serif; background-color: #2e2e2e; color: white; text-align: center; }        #game { margin-top: 20px; }        button { padding: 10px 20px; font-size: 16px; margin: 10px; cursor: pointer; }        #message { margin-top: 20px; font-size: 20px; }    </style></head><body>    <h1>Blackjack con ESP32</h1>    <div id='game'>        <button onclick='hit()'>Pedir Carta</button>        <button onclick='stand()'>Plantarse</button>        <div id='playerCards'>Cartas del jugador: </div>        <div id='dealerCards'>Cartas del crupier: </div>        <div id='message'></div>    </div>    <script>        let playerCards = [];        let dealerCards = [];        function getCard() { return Math.floor(Math.random() * 11) + 1; }        function sumCards(cards) { return cards.reduce((a,b) => a+b, 0); }        function updateDisplay() {            document.getElementById('playerCards').innerText = 'Cartas del jugador: ' + playerCards.join(', ');            document.getElementById('dealerCards').innerText = 'Cartas del crupier: ' + dealerCards.join(', ');        }        function hit() {            playerCards.push(getCard());            updateDisplay();            if(sumCards(playerCards) > 21) {                document.getElementById('message').innerText = 'Te pasaste! Perdiste.';            }        }        function stand() {            while(sumCards(dealerCards) < 17) {                dealerCards.push(getCard());            }            updateDisplay();            let playerSum = sumCards(playerCards);            let dealerSum = sumCards(dealerCards);            if(dealerSum > 21 || playerSum > dealerSum) {                document.getElementById('message').innerText = '¡Ganaste!';            } else if(playerSum == dealerSum) {                document.getElementById('message').innerText = 'Empate.';            } else {                document.getElementById('message').innerText = 'Perdiste.';            }        }        // Inicializar el juego        playerCards.push(getCard());        playerCards.push(getCard());        dealerCards.push(getCard());        updateDisplay();    </script></body></html>";

void handle_root() {
    server.send(200, "text/html", HTML);
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWiFi conectado con IP: " + WiFi.localIP().toString());
    server.on("/", handle_root);
    server.begin();
    Serial.println("Servidor HTTP iniciado");
}

void loop() {
    server.handleClient();
}
```

### Descripción  
Este ejemplo muestra un juego de Blackjack que se puede jugar desde un navegador web accediendo a la IP del ESP32.

---

## PRÁCTICA B - CONEXIÓN BLUETOOTH (BLE) CON DISPOSITIVO MÓVIL

### Objetivo  
Establecer una comunicación bidireccional entre un dispositivo móvil y el ESP32 utilizando tecnología Bluetooth Low Energy (BLE).

### Materiales  
- ESP32-S3  
- Aplicación móvil: Serial Bluetooth Terminal (u otra compatible)

### Procedimiento

#### Código:

```cpp
#include <Arduino.h>
#include <NimBLEDevice.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

NimBLEServer* pServer = nullptr;
NimBLECharacteristic* pCharacteristic = nullptr;

void setup() {
    Serial.begin(115200);
    Serial.println("Iniciando BLE con NimBLE!");

    NimBLEDevice::init("ESP32-NimBLE");
    pServer = NimBLEDevice::createServer();
    NimBLEService* pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
                          CHARACTERISTIC_UUID,
                          NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE
                      );
    pCharacteristic->setValue("Hola desde ESP32 NimBLE");
    pService->start();

    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    NimBLEDevice::startAdvertising();

    Serial.println("¡Servidor BLE NimBLE activo!");
}

void loop() {
    delay(2000);
}
```

### Explicación  
Este código configura el ESP32 como un periférico BLE que puede recibir y enviar información a través de una característica.

---

## Ejercicios adicionales para mejorar la nota

### Ejercicio 1: Servidor Web en Modo Access Point (AP)

```cpp
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

void handle_root();

const char* ssid = "Nautilus";  
const char* password = "20000Leguas";  

WebServer server(80);

String HTML = "<!DOCTYPE html><html><head><title>ESP32 Access Point</title></head><body><h1>ESP32 en Modo AP</h1><p>Estás conectado al ESP32 en modo Access Point.</p></body></html>";

void handle_root() {
    server.send(200, "text/html", HTML);
}

void setup() {
    Serial.begin(115200);
    Serial.println("Iniciando Access Point...");
    WiFi.softAP(ssid, password);
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
    server.on("/", handle_root);
    server.begin();
    Serial.println("Servidor web iniciado en modo AP.");
}

void loop() {
    server.handleClient();
}
```

**Descripción**:  
Este código transforma al ESP32 en un punto de acceso WiFi, permitiendo que los dispositivos se conecten directamente a él y visualicen una página web sencilla.

---

### Ejercicio 2: Comunicación BLE básica

```cpp
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando trabajo con BLE!");

  BLEDevice::init("MyESP32");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pCharacteristic->setValue("Hola Mundo");
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();
  Serial.println("Advertencia BLE iniciada");
}

void loop() {
  delay(2000);
}
```

### Descripción:  
Ejemplo simple para iniciar un servidor BLE básico que puede ser descubierto y conectado por dispositivos BLE compatibles.

---  
