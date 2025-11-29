#include <WiFi.h>
#include <SocketIoClient.h>

SocketIoClient socket;

const char* ssid = "SUA_WIFI";
const char* password = "SENHA_WIFI";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Conectando...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado!");

  // IP da API + porta 3000
  socket.begin("192.168.0.10", 3000);

  // Evento inicial vindo do servidor
  socket.on("INIT", [](const char* data){
    Serial.print("🔰 INIT recebido: ");
    Serial.println(data);
  });

  // Evento de atualização
  socket.on("UPDATE", [](const char* data){
    Serial.print("⚙ Atualização recebida: ");
    Serial.println(data);

    // Exemplo: se ID for 123, ligar um relé
    // Aqui você parseia JSON se quiser
  });

  socket.on("connect", [](){
    Serial.println("🔌 Conectado ao Socket.IO!");
  });
}

void loop() {
  socket.loop();
}
