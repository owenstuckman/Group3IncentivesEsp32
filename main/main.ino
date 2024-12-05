/*
Working code to connect to WiFi with an ESP32 board and post to our Supabase Database.
Inserts a row into 'Scanned Items'
Idea is for this post to send the HokieP and the barcode scanned into the database.

This is the main file, which is the firmware which will go onto the ESP32 itself.

Written by Owen Stuckman.
*/

#include <WiFi.h> // Wifi library
#include <HTTPClient.h> // Http client
#include <LiquidCrystal_I2C.h> // LCD
#include <ArduinoJson.h> // Json 

// Define LED pin (on board)
#define LED 2  

// Define LCD I2C device -> through GPIO
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define http client
HTTPClient http;

// Name of the network
const char* ssid = "VT Open WiFi";  
// API endpoint (Supabase Table) for scanned items

const char* serverUrlItems = "https://zmqjskgfggxxmpfhygni.supabase.co/rest/v1/ScannedItems";
// API endpoint (supabase table) for kickbacks points
const char* serverUrlKickbacks = "https://zmqjskgfggxxmpfhygni.supabase.co/rest/v1/ItemsKickbacks?select=pointsPer&barcodeID=eq.";
// API endpoint for kickbacks itemname
const char* serverUrlKickbacksName = "https://zmqjskgfggxxmpfhygni.supabase.co/rest/v1/ItemsKickbacks?select=item_name&barcodeID=eq.";
// API key (Supabase)
const char* apiKey = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InptcWpza2dmZ2d4eG1wZmh5Z25pIiwicm9sZSI6ImFub24iLCJpYXQiOjE3MzAzODM1NTcsImV4cCI6MjA0NTk1OTU1N30.So0-6hvuRcrW89GkzIOdaQhkA0k22QlFc4ev3zKSgqY";

// bools for logic
bool hokiePScanned = false;
bool scannedAnItem = false;
//vars
String barcode = "307750";
String hokieP = "";
String response = "";

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

    // delay rounds
    delay(10000);

// Scan with camera

    /*
    
    
    To-Do    
    
    
    
    */
    
    // put output of scanner to this
        // using sample hokieP
    String hokieP = "906630896";

    // ensure valid hokieP
    if (hokieP.length() == 9) { 
        // allow to insert item
        hokiePScanned = true;

        // print lcd message
        lcd.print("Valid HokieP");
        lcd.setCursor(0, 1);
        lcd.print("Scanned!");
        delay(1000);
        lcd.clear();
        Serial.println("Valid HokieP");
    }

    // ensure a valid hokieP is scanned before attributing items
    if (hokiePScanned == true){

        // for loop controls cycles for 5 seconds
        for(int i = 0; i < 5000; i++){
        
        // Scan for barcode
            // if scanned, set scanned an item to true
            /*
            
            To-Do


            barcode = ...
            scannedAnItem = true;
            
            */
            
            // sample bar code and time frame, since code is not working
            if (i = 1000){
                barcode = "307750";
                scannedAnItem = true;
            }

            // ensure item was scanned, allow for seperate cam logic
            if (scannedAnItem = true){
                
                // print lcd message
                lcd.print("Barcode Scanned: ");
                lcd.setCursor(0, 1);
                lcd.print(barcode);
                delay(1000);
                lcd.clear();
                Serial.print("Barcode Scanned: " + barcode);

            // Post Scanned Item to Supabase

                // Start request to server
                http.begin(serverUrlItems);

                // Add headers
                http.addHeader("Content-Type", "application/json");
                http.addHeader("apikey", apiKey);  // Supabase expects the API key in this header
                // Create JSON payload
                String payload = "{\"hokieP\":\"" + hokieP + "\", \"barcodeID\":\"" + barcode + "\"}";

                // Send POST request
                int httpResponseCode = http.POST(payload);

                // Check HTTP response, print errors to serial monitor
                if (httpResponseCode > 0) {
                    response = http.getString();
                    Serial.println("HTTP Response code: " + String(httpResponseCode));
                    Serial.println("Response: " + response);
                } else {
                    Serial.println("Error on HTTP request: " + String(httpResponseCode));
                }

                // end http request
                http.end();
                
            // get item name info from supabase to display

              // Start request to server
                http.begin(serverUrlKickbacksName + barcode);

                // Add headers
                http.addHeader("apikey", apiKey);  // Supabase expects the API key in this header
                
                // Send POST request
                httpResponseCode = http.GET();

                // Check HTTP response, print errors to serial monitor
                if (httpResponseCode > 0) {
                    response = http.getString();
                    Serial.println("HTTP Response code: " + String(httpResponseCode));
                    Serial.println("Response: " + response);
                } else {
                    Serial.println("Error on HTTP request: " + String(httpResponseCode));
                }

                // end http request
                http.end();

                // parse json

                // init var
                StaticJsonDocument<1000> extractJson;
                // Parse the JSON string 
                DeserializationError error = deserializeJson(extractJson, response); 
                // Check for errors in parsing 
                if (error) { Serial.print(F("deserializeJson() failed: ")); Serial.println(error.f_str()); return;}
                // extract column
                String itemName = extractJson["itemname"];
                Serial.println(itemName);

                // LCD print
                lcd.print("You Scanned: ");
                lcd.setCursor(0, 1);
                lcd.print(itemName);
                delay(1000);
                lcd.clear();

            // get 'points' info from supabase to display

                // Start request to server
                http.begin(serverUrlKickbacks + barcode);

                // Add headers
                http.addHeader("apikey", apiKey);  // Supabase expects the API key in this header
                
                // Send POST request
                httpResponseCode = http.GET();

                // Check HTTP response, print errors to serial monitor
                if (httpResponseCode > 0) {
                    response = http.getString();
                    Serial.println("HTTP Response code: " + String(httpResponseCode));
                    Serial.println("Response: " + response);
                } else {
                    Serial.println("Error on HTTP request: " + String(httpResponseCode));
                }

                // end http request
                http.end();

                // parse json

                // Extract json info
                StaticJsonDocument<1000> extractJson2;
                // Parse the JSON string 
                DeserializationError error2 = deserializeJson(extractJson2, response); 
                // Check for errors in parsing 
                if (error) { Serial.print(F("deserializeJson() failed: ")); Serial.println(error2.f_str()); return;}
                // extract column
                String pointsPer = extractJson2["pointsPer"];
                Serial.println(pointsPer);

                // LCD print
                lcd.print("Earned points: ");
                lcd.setCursor(0,1);
                lcd.print(pointsPer);
                delay(1000);
                lcd.clear();

                // restart scan
                scannedAnItem = false;

                // reset scanning cycle
                i = 0;

            }

            // 1 ms cycle (per i)
            delay(1);
        }

        // reset hokieP scanning (full cycle)
        hokiePScanned = false;
        // reset hokieP (failsafe)
        hokieP = '906630896';

    }
}