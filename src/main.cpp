#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>


// Pin Declarations
const int DIR_PIN_1 = 32;
const int BRAKE_PIN_1 = 33;
const int SPEED_PIN_1 = 26;
const int PWM_PIN_1 = 25;

const int DIR_PIN_2 = 27;
const int BRAKE_PIN_2 = 14;
const int SPEED_PIN_2 = 13;
const int PWM_PIN_2 = 12;

const int DEBUG_LED = 2;

// Set these to your desired credentials.
const char *ssid = "Mowerbot";
const char *password = "Password";

NetworkServer server(80);



void stopMotors();
void moveForward();
void turnLeft();
void turnRight();

void setup() {
  pinMode(2, OUTPUT); // debug light
  pinMode(PWM_PIN_1, OUTPUT);
  pinMode(PWM_PIN_2, OUTPUT);
  pinMode(DIR_PIN_1, OUTPUT);
  pinMode(DIR_PIN_2, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while (1);
  }
  
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}
void loop() {
  WiFiClient client = server.accept();

  if (client) {
    Serial.println("Client connected");

    String request = client.readStringUntil('\r');
    Serial.println("Request: " + request);

    // 🌐 Serve HTML
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html\n");
    client.println("<html><body><h1>ESP32 Motor Control</h1>");
    client.println("<a href='/N'>ON</a><br>");
    client.println("<a href='/F'>OFF</a><br>");
    client.println("<a href='/L'>LEFT</a><br>");
    client.println("<a href='/R'>RIGHT</a><br>");
    client.println("</body></html>");

    // ⚙️ Command Handling
    if (request.indexOf("GET /N") != -1) moveForward();
    else if (request.indexOf("GET /F") != -1) stopMotors();
    else if (request.indexOf("GET /L") != -1) turnLeft();
    else if (request.indexOf("GET /R") != -1) turnRight();

    client.stop();
    Serial.println("Client disconnected");
  }
}

void stopMotors() {
  analogWrite(PWM_PIN_1, 0);
  analogWrite(PWM_PIN_2, 0);
  digitalWrite(DIR_PIN_1, LOW);
  digitalWrite(DIR_PIN_2, LOW);

  digitalWrite(DEBUG_LED, LOW);
}

void moveForward() {
  analogWrite(PWM_PIN_1, 40);
  analogWrite(PWM_PIN_2, 40);
  digitalWrite(DIR_PIN_1, HIGH);
  digitalWrite(DIR_PIN_2, HIGH);

  digitalWrite(DEBUG_LED, HIGH);
}

void turnLeft() {
  analogWrite(PWM_PIN_1, 40);
  analogWrite(PWM_PIN_2, 40);
  digitalWrite(DIR_PIN_1, HIGH);
  digitalWrite(DIR_PIN_2, LOW);

  digitalWrite(DEBUG_LED, HIGH);
}

void turnRight() {
  analogWrite(PWM_PIN_1, 40);
  analogWrite(PWM_PIN_2, 40);
  digitalWrite(DIR_PIN_1, LOW);
  digitalWrite(DIR_PIN_2, HIGH);

  digitalWrite(DEBUG_LED, HIGH);
}