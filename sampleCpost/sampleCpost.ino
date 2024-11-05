#include <WiFi.h>
#include <HTTPClient.h>

// ssid is wifi network
const char* ssid = "VT Open Wifi";
// wifi password
const char* password = "your_PASSWORD";
// api end point
const char* serverUrl = "https://zmqjskgfggxxmpfhygni.supabase.co/rest/v1/ScannedItems";
// api key 
const char* apiKey = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InptcWpza2dmZ2d4eG1wZmh5Z25pIiwicm9sZSI6ImFub24iLCJpYXQiOjE3MzAzODM1NTcsImV4cCI6MjA0NTk1OTU1N30.So0-6hvuRcrW89GkzIOdaQhkA0k22QlFc4ev3zKSgqY";  // Replace with your actual API key

void setup() {

  // Begin Post to Supabase
    Serial.begin(115200);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi");

    // Make HTTP POST request
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        
        // Begin the HTTP connection
        http.begin(serverUrl);

        // Add headers
        http.addHeader("Content-Type", "application/json");
        http.addHeader("Authorization", String("Bearer ") + apiKey); // For Bearer token
        // Or if your API requires it as "x-api-key":
        // http.addHeader("x-api-key", apiKey);

        // Create JSON payload
        String payload = "{\"sample\":\"John Doe\"}";

        // Send HTTP POST request
        int httpResponseCode = http.POST(payload);

        // Check the response
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
}

void loop() {
    // You can place code here to run repeatedly
}
