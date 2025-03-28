#include <WiFi.h>
#include <WebServer.h>

/* WiFi Credentials */
const char* ssid = "SJAY-5G";   // Enter SSID here
const char* password = "12345678"; // Enter Password here

WebServer server(80);

bool LEDstatus = LOW;
const int LED_PIN = 2; // GPIO2

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(LED_PIN, OUTPUT);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  // Wait until connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Define routes
  server.on("/", handle_OnConnect);
  server.on("/ledon", handle_ledon);
  server.on("/ledoff", handle_ledoff);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP Server Started");
}

void loop() {
  server.handleClient();
  digitalWrite(LED_PIN, LEDstatus);
}

void handle_OnConnect() {
  LEDstatus = LOW;
  Serial.println("LED: OFF");
  server.send(200, "text/html", updateWebpage(LEDstatus));
}

void handle_ledon() {
  LEDstatus = HIGH;
  Serial.println("LED: ON");
  server.send(200, "text/html", updateWebpage(LEDstatus));
}

void handle_ledoff() {
  LEDstatus = LOW;
  Serial.println("LED: OFF");
  server.send(200, "text/html", updateWebpage(LEDstatus));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String updateWebpage(uint8_t LEDstatus) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html {font-family: Helvetica; text-align: center;}\n";
  ptr += "body {margin-top: 50px;} h1 {color: #444;} h3 {color: #444;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;font-size: 25px;margin: 20px auto;border-radius: 4px;cursor: pointer;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ESP32 Web Server</h1>\n";
  ptr += "<h3>Using Station(STA) Mode</h3>\n";

  if (LEDstatus) {
    ptr += "<p>BLUE LED: ON</p><a class=\"button button-off\" href=\"/ledoff\">OFF</a>\n";
  } else {
    ptr += "<p>BLUE LED: OFF</p><a class=\"button button-on\" href=\"/ledon\">ON</a>\n";
  }

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

