#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// ======== CONFIGURAÇÕES DE REDE ========
const char* ssid = "NOME_DA_SUA_REDE";
const char* password = "SENHA_DA_SUA_REDE";

// ======== CONFIGURAÇÕES DA API ========
const char* serverName = "http://seu-servidor.com/api/estado";  // URL da API

// ======== CONFIGURAÇÕES DAS PORTAS ========
const int PORTA_1 = 5;   // Exemplo: GPIO5
const int PORTA_2 = 18;  // Exemplo: GPIO18

void setup() {
  Serial.begin(115200);

  pinMode(PORTA_1, OUTPUT);
  pinMode(PORTA_2, OUTPUT);

  // Conecta ao Wi-Fi
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado ao WiFi!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverName);
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.printf("Código de resposta: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println("Payload recebido: " + payload);

        // ======== PARSE DO JSON ========
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (!error) {
          bool porta1Estado = doc["porta1"];
          bool porta2Estado = doc["porta2"];

          digitalWrite(PORTA_1, porta1Estado ? HIGH : LOW);
          digitalWrite(PORTA_2, porta2Estado ? HIGH : LOW);

          Serial.printf("Porta 1: %s | Porta 2: %s\n",
                        porta1Estado ? "LIGADA" : "DESLIGADA",
                        porta2Estado ? "LIGADA" : "DESLIGADA");
        } else {
          Serial.print("Erro ao ler JSON: ");
          Serial.println(error.f_str());
        }
      }
    } else {
      Serial.printf("Falha na requisição HTTP: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    Serial.println("WiFi desconectado!");
  }

  delay(5000); // Faz nova requisição a cada 5 segundos
}
