/*

Working code to connect to WiFi with an ESP32 board and post to our Supabase Database.
Inserts a row into 'Scanned Items'
Idea is for this post to send the HokieP and the barcode scanned into the database. 

Written by Owen Stuckman on 11/13/2024. 

*/

#include <WiFi.h> // Wifi library
#include <HTTPClient.h> // Http client

// Define LED pin (on board)
#define LED 2  

// Name of the network
const char* ssid = "VT Open WiFi";  
// API endpoint (Supabase Table)
const char* serverUrl = "https://zmqjskgfggxxmpfhygni.supabase.co/rest/v1/ScannedItems";
// API key (Supabase)
const char* apiKey = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InptcWpza2dmZ2d4eG1wZmh5Z25pIiwicm9sZSI6ImFub24iLCJpYXQiOjE3MzAzODM1NTcsImV4cCI6MjA0NTk1OTU1N30.So0-6hvuRcrW89GkzIOdaQhkA0k22QlFc4ev3zKSgqY";

bool scannedAnItem = false;

// Run before Loop() is run
void setup() {

    // set baud rate to 115200
    Serial.begin(115200);
    // setup on board led
    pinMode(LED, OUTPUT);

    // Initialize ESP32 to station mode (connecting)
    WiFi.mode(WIFI_STA);  
    // Connect to specifies network
    WiFi.begin(ssid);      

    // init vars
    int attempts = 0;
    const int maxAttempts = 10;

    // Check, then wait
    while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
        delay(500);
        Serial.print(".");
        attempts++; 
    }

    // State whether was able to connect or not
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to Wi-Fi\n\n\n");
    } else {
        Serial.println("\nFailed to connect to Wi-Fi\n\n\n");
       
    }
    
}

void loop() {
    
    if(scannedAnItem == true){

    // Init http client, and start request to server
    HTTPClient http;
    http.begin(serverUrl);

    // Add headers
    http.addHeader("Content-Type", "application/json");
    http.addHeader("apikey", apiKey);  // Supabase expects the API key in this header
      // Create JSON payload
    String payload = "{\"sample\":\"John Doe\"}";

    // Send POST request
    int httpResponseCode = http.POST(payload);

    // Check HTTP response, print errors to serial monitor
    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("HTTP Response code: " + String(httpResponseCode));
        Serial.println("Response: " + response);
    } else {
        Serial.println("Error on HTTP request: " + String(httpResponseCode));
    }

    http.end()
    
    }
}
