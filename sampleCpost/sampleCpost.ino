#include <iostream>
#include <string>
#include <httplib.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void setup() {

    httplib::Client cli("https://zmqjskgfggxxmpfhygni.supabase.co", 443);
    cli.enable_server_certificate_verification(false); // Only for testing, disable in production

    json payload = {
        {"sample", "John Doe"},
     
    };

    std::string body = payload.dump();
    httplib::Headers headers = {
        {"Authorization", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InptcWpza2dmZ2d4eG1wZmh5Z25pIiwicm9sZSI6ImFub24iLCJpYXQiOjE3MzAzODM1NTcsImV4cCI6MjA0NTk1OTU1N30.So0-6hvuRcrW89GkzIOdaQhkA0k22QlFc4ev3zKSgqY"},
        {"Content-Type", "application/json"}
    };

    auto res = cli.Post("/rest/v1/ScannedItems", headers, body, "application/json");

    if (res) {
        std::cout << "Status: " << res->status << std::endl;
        std::cout << "Response: " << res->body << std::endl;
    } else {
        std::cerr << "Error: " << res.error() << std::endl;
    }

    return 0;

}

void loop() {
  // put your main code here, to run repeatedly:
  print('Test');

  dalay(5);
}