#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

void handle_root();
// Configuración del Access Point (AP)
const char* ssid = "Nautilus";  // Enter your SSID here 
const char* password = "20000Leguas";  //Enter your Password here 

WebServer server(80); // Servidor web en el puerto 80

// Página HTML que se mostrará en el navegador
String HTML = "<!DOCTYPE html>\
<html>\
<head><title>ESP32 Access Point</title></head>\
<body>\
<h1>ESP32 en Modo AP</h1>\
<p>Estás conectado al ESP32 en modo Access Point.</p>\
</body>\
</html>";

// Manejo de la raíz "/"
void handle_root() {
    server.send(200, "text/html", HTML);
}

void setup() {
    Serial.begin(115200);
    Serial.println("Iniciando Access Point...");

    // Configurar el ESP32 como Access Point (sin conexión a internet)
    WiFi.softAP(ssid, password);

    // Mostrar la dirección IP del AP
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    // Configurar el servidor web
    server.on("/", handle_root);
    server.begin();
    Serial.println("Servidor web iniciado en modo AP.");
}

void loop() {
    server.handleClient(); // Manejar solicitudes de clientes
}
