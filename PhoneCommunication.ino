/**
 * Project: 7Semi ESP32-S3 EC200U HTTP POST
 * Description: Sends a JSON payload to a remote server via 4G LTE.
 */

#define MODEM_RX 12   // ESP32 RX1 -> EC200U TX [cite: 914]
#define MODEM_TX 13   // ESP32 TX1 -> EC200U RX [cite: 912]
#define MODEM_PWR 10  // GPIO 10 controls 4G ON/OFF [cite: 918]

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Initialize Hardware Serial 1 for the EC200U Modem [cite: 746]
  Serial1.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);

  // Power on the modem [cite: 918]
  pinMode(MODEM_PWR, OUTPUT);
  digitalWrite(MODEM_PWR, HIGH);
  delay(1000); 
  digitalWrite(MODEM_PWR, LOW);

  Serial.println(F("--- 7Semi HTTP POST System Initialized ---"));
  Serial.println(F("Type 'GO' to trigger a POST request."));
}

void loop() {
  // Read modem responses and print to Serial Monitor
  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }

  // Trigger POST request when user types 'GO'
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.equalsIgnoreCase("GO")) {
      sendHttpPost();
    }
  }
}

void sendHttpPost() {
  Serial.println(F("[Action] Starting HTTP POST..."));

  // 1. Check Network Registration [cite: 1212]
  Serial1.println("AT+CREG?");
  delay(500);

  // 2. Configure and Activate PDP Context (Set your APN here) 
  // Format: AT+QICSGP=context_id, context_type, "APN"
  Serial1.println("AT+QIACT=1"); 
  delay(2000);

  // 3. Set the target URL 
  // Length of URL must match the first parameter (e.g., 23 characters)
  Serial1.println("AT+QHTTPURL=23,80"); 
  delay(500); 
  Serial1.print("http://your-api-url.com");
  delay(1000);

  // 4. Send the POST Body 
  // Define your data string
  String postData = "{\"device\":\"ESP32-S3\",\"status\":\"online\"}";
  
  // Format: AT+QHTTPPOST=data_length, input_time, response_time
  Serial1.print("AT+QHTTPPOST=");
  Serial1.print(postData.length());
  Serial1.println(",80,80");
  delay(500);
  Serial1.print(postData);
  
  Serial.println(F("[Action] Data Sent. Waiting for server response..."));
}