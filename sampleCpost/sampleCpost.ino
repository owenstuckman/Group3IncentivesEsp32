#include <WiFi.h>
#include <HTTPClient.h>

#define LED 2  // Define LED pin

// ssid is wifi network
const char* ssid = "eduroam";  // Name of the network
const char* username = "ostuckman@vt.edu";
const char* password = "iqsm-lspy-zgof-mpeu";

// api endpoint
const char* serverUrl = "https://zmqjskgfggxxmpfhygni.supabase.co/rest/v1/ScannedItems";
// api key 
const char* apiKey = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InptcWpza2dmZ2d4eG1wZmh5Z25pIiwicm9sZSI6ImFub24iLCJpYXQiOjE3MzAzODM1NTcsImV4cCI6MjA0NTk1OTU1N30.So0-6hvuRcrW89GkzIOdaQhkA0k22QlFc4ev3zKSgqY";  // Replace with your actual API key

void setup() {
    // Initialize Serial and LED pin
    Serial.begin(115200);
    pinMode(LED, OUTPUT);  // Set LED pin as output


    int attempts = 0;
    const int maxAttempts = 5;  // Adjust as needed

    while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
        Serial.print(Wifi.Status());
        delay(500);
        Serial.print(".");
        attempts++;
        // Connect to Wi-Fi
        WiFi.mode(WIFI_STA);  // Set Wi-Fi to Station mode
        WiFi.begin(ssid, password);
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to Wi-Fi");
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


    // Make HTTP POST request
    HTTPClient http;
    http.begin(serverUrl);  // Start the HTTP connection

    // Add headers
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", String("Bearer ") + apiKey); // For Bearer token
    // Alternatively, use http.addHeader("x-api-key", apiKey);

    // Create JSON payload
    String payload = "{\"sample\":\"John Doe\"}";

    // Send HTTP POST request
    int httpResponseCode = http.POST(payload);

    // Check HTTP response
    if (httpResponseCode > 0) {
        String response = http.getString(); // Get the response payload
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
