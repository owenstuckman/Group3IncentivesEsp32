#include <WiFi.h>
#include <HTTPClient.h>
#include "esp_wpa2.h"  // Needed for WPA2 Enterprise

#define LED 2  // Define LED pin

// Network credentials
const char* ssid = "eduroam"; // name of network
const char* username = "ostuckman@vt.edu";
const char* password = "iqsm-lspy-zgof-mpeu";

// API endpoint
const char* serverUrl = "https://zmqjskgfggxxmpfhygni.supabase.co/rest/v1/ScannedItems";
// API key
const char* apiKey = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InptcWpza2dmZ2d4eG1wZmh5Z25pIiwicm9sZSI6ImFub24iLCJpYXQiOjE3MzAzODM1NTcsImV4cCI6MjA0NTk1OTU1N30.So0-6hvuRcrW89GkzIOdaQhkA0k22QlFc4ev3zKSgqY";  // Replace with your actual API key

void setup() {
    // Initialize Serial and LED pin
    Serial.begin(115200);
    pinMode(LED, OUTPUT);  // Set LED pin as output

    WiFi.disconnect(true);  // Disconnect from previous connections
    delay(1000);
    
    WiFi.mode(WIFI_STA);  // Set Wi-Fi to Station mode
    esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)username, strlen(username));
    esp_wifi_sta_wpa2_ent_set_username((uint8_t *)username, strlen(username));
    esp_wifi_sta_wpa2_ent_set_password((uint8_t *)password, strlen(password));
    esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT();
    esp_wifi_sta_wpa2_ent_enable(&config);

    WiFi.begin(ssid);  // Connect to Wi-Fi

    int attempts = 0;
    const int maxAttempts = 10;

    while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
        Serial.print(WiFi.status());
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to Wi-Fi");

        // LED indication
        digitalWrite(LED, HIGH);
        Serial.println("LED is on");
        delay(5000);
        digitalWrite(LED, LOW);
        Serial.println("LED is off");
        delay(1000);

    } else {
        Serial.println("\nFailed to connect to Wi-Fi");
        
        // LED indication
        digitalWrite(LED, HIGH);
        Serial.println("LED is on");
        delay(1000);
        digitalWrite(LED, LOW);
        Serial.println("LED is off");
        delay(1000);

        // Optionally, restart ESP if connection fails
        ESP.restart();
    }

    // Make HTTP POST request once connected to the network
    HTTPClient http;
    http.begin(serverUrl);  // Start the HTTP connection

    // Add headers
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", String("Bearer ") + apiKey); // For Bearer token

    // Create JSON payload
    String payload = "{\"sample\":\"John Doe\"}";

    // Send HTTP POST request
    int httpResponseCode = http.POST(payload);

    // Check HTTP response
    if (httpResponseCode > 0) {
        String response = http.getString();  // Get the response payload
        Serial.println("HTTP Response code: " + String(httpResponseCode));
        Serial.println("Response: " + response);
    } else {
        Serial.println("Error on HTTP request: " + String(httpResponseCode));
    }

    // Free resources
    http.end();
}

void loop() {
    // Blink LED every 3 seconds
    digitalWrite(LED, HIGH);
    Serial.println("LED is on");
    delay(3000);
    digitalWrite(LED, LOW);
    Serial.println("LED is off");
    delay(3000);
}
