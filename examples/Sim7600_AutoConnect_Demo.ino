/**
 * Project: Sim7600 Manager Auto-Connect Demo
 * Description: Demonstrates using the Sim7600Manager class for automatic
 *              modem initialization, network attachment, and diagnostics.
 * Author: Aerio Jobin
 * Date: March 2025
 */

#include "../Sim7600Manager.h"

// Pin Configuration - ESP32-S3 to EC200U Modem
#define MODEM_RX 12      // ESP32 RX1 -> EC200U TX
#define MODEM_TX 13      // ESP32 TX1 -> EC200U RX
#define MODEM_PWR 10     // GPIO 10 controls modem power (PWR_KEY)
#define MODEM_RST -1     // No hardware reset pin

Sim7600Manager modem(Serial1, MODEM_PWR, MODEM_RST);

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println(F("\n=== Sim7600 Manager Demo ==="));
  Serial.println(F("Initializing modem..."));
  
  // Initialize the modem manager
  modem.begin(115200);
  
  Serial.println(F("Modem initialization started."));
  Serial.println(F("Check serial for status updates."));
}

void loop() {
  // Call tick() regularly to drive the state machine
  modem.tick();
  
  // Print diagnostics every 2 seconds
  static unsigned long lastDiag = 0;
  if (millis() - lastDiag > 2000) {
    lastDiag = millis();
    printDiagnostics();
  }
  
  // Once ready, you can send HTTP requests or other commands
  if (modem.isReady()) {
    // Example: Send a custom AT command
    // modem.sendAT("AT+QHTTPURL=23,80", "OK", 1000);
  }
  
  delay(50);
}

void printDiagnostics() {
  Serial.print(F("[Status] State: "));
  
  switch (modem.state()) {
    case ModemState::POWERED_OFF:
      Serial.print(F("POWERED_OFF"));
      break;
    case ModemState::INITIALIZING:
      Serial.print(F("INITIALIZING"));
      break;
    case ModemState::ATTACHING_NETWORK:
      Serial.print(F("ATTACHING_NETWORK"));
      break;
    case ModemState::READY:
      Serial.print(F("READY (can send HTTP/SMS/calls)"));
      break;
    case ModemState::ERROR:
      Serial.print(F("ERROR (retrying)"));
      break;
  }
  
  Serial.println();
  
  // Optionally print last response
  String response = modem.getResponse();
  if (response.length() > 0 && response.length() < 100) {
    Serial.print(F("[Response] "));
    Serial.println(response);
  }
}

// Example: Sending HTTP POST request once modem is ready
void sendHttpPostExample() {
  if (!modem.isReady()) {
    Serial.println(F("Modem not ready yet."));
    return;
  }
  
  // Activate context (set your APN)
  modem.sendAT("AT+QIACT=1", "OK", 2000);
  
  // Set URL
  modem.sendAT("AT+QHTTPURL=23,80", "\r\n", 500);
  Serial.print("http://your-api-server.com");
  
  // Prepare POST data
  String data = "{\"device\":\"ESP32-S3\",\"status\":\"connected\"}";
  
  String cmd = "AT+QHTTPPOST=" + String(data.length()) + ",80,80";
  modem.sendAT(cmd.c_str(), "\r\n", 500);
  Serial.print(data);
  
  Serial.println(F("[HTTP] POST sent"));
}
