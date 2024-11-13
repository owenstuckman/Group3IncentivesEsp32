#include <WiFi.h>
#include <HTTPClient.h>

#define LED 2  // Define LED pin

const char* ssid = "VT Open WiFi";  // Name of the network

// API endpoint
const char* serverUrl = "https://zmqjskgfggxxmpfhygni.supabase.co/rest/v1/ScannedItems";
// API key
const char* apiKey = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InptcWpza2dmZ2d4eG1wZmh5Z25pIiwicm9sZSI6ImFub24iLCJpYXQiOjE3MzAzODM1NTcsImV4cCI6MjA0NTk1OTU1N30.So0-6hvuRcrW89GkzIOdaQhkA0k22QlFc4ev3zKSgqY";

void setup() {
    Serial.begin(115200);
    pinMode(LED, OUTPUT);

    WiFi.mode(WIFI_STA);  // Station mode
    WiFi.begin(ssid);      // Connect to open Wi-Fi

    int attempts = 0;
    const int maxAttempts = 10;

    while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
        delay(500);
        Serial.print(".");
        attempts++; 
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to Wi-Fi\n\n\n");
    } else {
        Serial.println("\nFailed to connect to Wi-Fi\n\n\n");
       
    }

    HTTPClient http;
    http.begin(serverUrl);

    // Add headers
    http.addHeader("Content-Type", "application/json");
    http.addHeader("apikey", apiKey);  // Supabase expects the API key in this header

    // Create JSON payload
    String payload = "{\"sample\":\"John Doe\"}";

    // Send HTTP POST request
    int httpResponseCode = http.POST(payload);

    // Check HTTP response
    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("HTTP Response code: " + String(httpResponseCode));
        Serial.println("Response: " + response);
    } else {
        Serial.println("Error on HTTP request: " + String(httpResponseCode));
    }

    http.end();
}

void loop() {
    digitalWrite(LED, HIGH);
    delay(3000);
    digitalWrite(LED, LOW);
    delay(3000);
    Serial.println("Things are going");
}
