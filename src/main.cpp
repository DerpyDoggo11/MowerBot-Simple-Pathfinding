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

// Set these to your desired credentials.
const char *ssid = "Mowerbot";
const char *password = "Password";

NetworkServer server(80);



// put function declarations here:
int myFunction(int, int);

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
  NetworkClient client = server.accept();  // listen for incoming clients

  if (client) {
    Serial.println("New Client.");
    String currentLine = ""; 

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // Enlarged Text Styling
            client.println("<html><body>");
            client.println("<h1 style='font-size:100px;'>ESP32 Motor Control</h1>");
            client.println("<p style='font-size:50px;'>Click <a href='/N'>here</a> to turn ON the motors.</p>");
            client.println("<p style='font-size:50px;'>Click <a href='/F'>here</a> to turn OFF the motors.</p>");

            client.println("<p style='font-size:50px;'>Click <a href='/R'>here</a> to turn Right.</p>");
            client.println("<p style='font-size:50px;'>Click <a href='/L'>here</a> to turn Left.</p>");
            client.println("</body></html>");

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        // Check client request
        if (currentLine.endsWith("GET /N")) { // on
          digitalWrite(2, HIGH); // debug light
          analogWrite(PWM_PIN_1, 40);
          analogWrite(PWM_PIN_2, 40);
          digitalWrite(DIR_PIN_1, HIGH);
          digitalWrite(DIR_PIN_2, HIGH);
        }
        if (currentLine.endsWith("GET /F")) { // off
          digitalWrite(2, LOW); // debug light
          analogWrite(PWM_PIN_1, 0);
          analogWrite(PWM_PIN_2, 0);
          digitalWrite(DIR_PIN_1, LOW);
          digitalWrite(DIR_PIN_2, LOW);
        }
        if (currentLine.endsWith("GET /R")) { // off
          digitalWrite(2, HIGH); // debug light
          analogWrite(PWM_PIN_1, 40);
          analogWrite(PWM_PIN_2, 40);
          digitalWrite(DIR_PIN_1, LOW);
          digitalWrite(DIR_PIN_2, HIGH);
        }
        if (currentLine.endsWith("GET /L")) { // off
          digitalWrite(2, HIGH); // debug light
          analogWrite(PWM_PIN_1, 40);
          analogWrite(PWM_PIN_2, 40);
          digitalWrite(DIR_PIN_1, HIGH);
          digitalWrite(DIR_PIN_2, LOW);
        }
      }
    }
    
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}