/*
Working code to connect to WiFi with an ESP32 board and post to our Supabase Database.
Inserts a row into 'Scanned Items'
Idea is for this post to send the HokieP and the barcode scanned into the database.

This is the main file, which is the firmware which will go onto the ESP32 itself.

Written by Owen Stuckman.
*/

#include <WiFi.h> // Wifi library
#include <HTTPClient.h> // Http client
#include <LiquidCrystal_I2C.h>

// Define LED pin (on board)
#define LED 2  

// Define LCD I2C device -> through GPIO
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Name of the network
const char* ssid = "VT Open WiFi";  
// API endpoint (Supabase Table)
const char* serverUrl = "https://zmqjskgfggxxmpfhygni.supabase.co/rest/v1/ScannedItems";
// API key (Supabase)
const char* apiKey = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InptcWpza2dmZ2d4eG1wZmh5Z25pIiwicm9sZSI6ImFub24iLCJpYXQiOjE3MzAzODM1NTcsImV4cCI6MjA0NTk1OTU1N30.So0-6hvuRcrW89GkzIOdaQhkA0k22QlFc4ev3zKSgqY";

// bools for logic
bool hokiePScanned = false;
bool scannedAnItem = false;
//vars
String barcode = "";
String hokieP = "";

void setup() {

    // init serial monitor, set baud rate to 115200
    Serial.begin(115200);
    // setup on board led
    pinMode(LED, OUTPUT);

// connect to WiFi

    // Initialize ESP32 to station mode (connecting)
    WiFi.mode(WIFI_STA);  
    // Connect to specific network (VT Open WiFi)
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

// initialize LCD

    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    
// initialize camera
/*



    To-Do



*/


}

void loop() {

// Scan with camera
    // thing

    /*
    
    
    To-Do    
    
    
    
    */
    
    // put output of scanner to this
    std::string hokieP = '';

    // ensure valid hokieP
    if ((std::all_of(str.begin(), str.end(), ::isdigit)) && str.length() == 9) { 
        hokiePScanned = true;
    }


    
    if (hokiePScanned == true){

        for(int i = 0; i < 5000; i++){
        
        // Scan for barcode
            // if scanned, set scanned an item to true
            /*
            
            To-Do


            barcode = ...
            scannedAnItem = true;
            
            */

            if (scannedAnItem = true){

            // Post Scanned Item to Supabase

                // Init http client, and start request to server
                HTTPClient http;
                http.begin(serverUrl);

                // Add headers
                http.addHeader("Content-Type", "application/json");
                http.addHeader("apikey", apiKey);  // Supabase expects the API key in this header
                // Create JSON payload
                String payload = "{\"hokieP\":\"" + hokieP + "\", \"barcodeID\":\"" + barcode + "\"}";

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

                // end http request
                http.end();
                
                // restart scan
                scannedAnItem = false;

                // reset scanning cycle
                i = 0;

                // take a second in between
                delay(500);

            }

            // 1 ms cycle 
            delay(1);
        }

        // reset hokieP scanning
        hokiePScanned = false;

    }
}
