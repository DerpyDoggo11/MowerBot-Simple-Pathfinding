#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <WiFi.h>
#include <ArduinoJson.h>



// Set these to your desired credentials.
const char *ssid = "Mowerbot";
const char *password = "Password";

float smoothingFactor = 10;
double fps1 = 5 / smoothingFactor; // feet per second, replace with acceleration data and estimated position based on time of motor running forward.
double degrees = 0; // replace with rotational data from esp32 and estimated rotation based on time of turning
float posX = 0, posY = 500;



AsyncWebServer server(80);
AsyncWebSocket ws("/ws");


void notifyClients();
void updatePositionData();

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());


  ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    
    if (type == WS_EVT_CONNECT) {
      Serial.println("Client connected");
      client->text("open");  // Send an "open" message to this client
    }
    
    if (type == WS_EVT_DATA) {
      String msg = "";
      for (size_t i = 0; i < len; i++) {
        msg += (char) data[i];
      }
      Serial.println("Received message: " + msg);

      DynamicJsonDocument doc(200);
      DeserializationError error = deserializeJson(doc, msg);
      if (!error) {
        posX = doc["x"] | posX;
        posY = doc["y"] | posY;
      }
    }

  });

  server.addHandler(&ws);
  server.begin();
  
  Serial.println("Server started");
}

void loop() {
  updatePositionData();
  notifyClients();
  delay(1000 / smoothingFactor);

}

void notifyClients() {
  String json = "{\"x\":" + String(posX) + ",\"y\":" + String(posY) + "}";
  ws.textAll(json);  // Sends to all connected WebSocket clients
}

void updatePositionData(){
  double theta = degrees * (PI / 180.0);  // ≈ 1.571 radians
  posX += cos(theta) * fps1;
  posY += sin(theta) * fps1;
}
