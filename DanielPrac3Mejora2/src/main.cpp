#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Definir UUIDs para el servicio y la característica
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando trabajo con BLE!");

  // Inicializar el dispositivo BLE con un nombre
  BLEDevice::init("MyESP32");

  // Crear el servidor BLE
  BLEServer *pServer = BLEDevice::createServer();

  // Crear el servicio BLE
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Crear la característica BLE con permisos de lectura y escritura
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  // Establecer un valor inicial para la característica
  pCharacteristic->setValue("Hola Mundo desde ESP32");
  
  // Iniciar el servicio
  pService->start();

  // Iniciar la publicidad para que otros dispositivos puedan encontrar el servidor
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // Funciones que ayudan con problemas de conexión en iPhone
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("¡Característica definida! Ahora puedes leerla en tu dispositivo.");
}

void loop() {
  // Código principal que se ejecuta repetidamente
  delay(2000);
}
