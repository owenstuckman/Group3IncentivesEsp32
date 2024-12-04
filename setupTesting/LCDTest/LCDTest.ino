// Start request to server
                http.begin(serverUrlKickbacks + barcode);

                // Add headers
                http.addHeader("apikey", apiKey);  // Supabase expects the API key in this header
                
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