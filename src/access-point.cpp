#include "access-point.h"

// Set web server port number to 80
WiFiServer server(80);

void sendBadRequestResponse(WiFiClient client) {
  // HTTP requests always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 400 Bad Request");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  
  // Display the HTML web page
  client.println("<!DOCTYPE html><html>");            
  client.println("<body><h1>Bad request. Expecting query string of type: /?ssid=mySSID&password=mySSIDPassword&access_token=sensorAccessToken</h1>");
  client.println("</body></html>");
  
  // The HTTP response ends with another blank line
  client.println();
}

void sendSuccessResponse(WiFiClient client) {
  // HTTP requests always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  
  // Display the HTML web page
  client.println("<!DOCTYPE html><html>");            
  client.println("<body><h1>Successfully saved config</h1>");
  client.println("</body></html>");
  
  // The HTTP response ends with another blank line
  client.println();
}

bool setupAP() {
  Serial.print("Setting up soft-AP...");
  boolean result = WiFi.softAP(CFG_ONBOARD_WIFI_SSID, CFG_ONBOARD_WIFI_PASSWORD);
  if(!result) {
    Serial.println("Soft-AP setup failed");
    return false;
  }
  
  Serial.println("Soft-AP setup success");

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.begin();

  return true;
}

void listenForConfig() {
  bool credentialsSaved = false;

  while(!credentialsSaved) {
    // Variable to store the HTTP request
    String request;
    WiFiClient client = server.available();   // Listen for incoming clients

    if (client) {                             // If a new client connects
      Serial.println("New client connection");          
      String currentLine = "";                // make a String to hold incoming data from the client
      while (client.connected()) {          
        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();          
          request += c;
          if (c == '\n') {                    // if the byte is a newline character
            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              if (
                request.indexOf("GET /?") < 0
                || request.indexOf("ssid") < 0
                || request.indexOf("password") < 0
                || request.indexOf("access_token") < 0
              ) {
                sendBadRequestResponse(client);
                break;
              }
              Serial.println(request);

              // request in the form of /?ssid=mySSID&password=mySSIDPassword&access_token=sensorAccessToken
              char *token = strtok((char *) request.c_str(), "?"); 
              char *ssidName = strtok(NULL, "=");
              char *ssidVal = strtok(NULL, "&");
              char *passwordName = strtok(NULL, "=");
              char *passwordVal = strtok(NULL, "&");
              char *accessTokenName = strtok(NULL, "=");
              char *accessTokenVal = strtok(NULL, " ");

              if (
                strcmp("ssid", ssidName) != 0 
                || strcmp("password", passwordName) != 0 
                || strcmp("access_token", accessTokenName) != 0
              ) {
                sendBadRequestResponse(client);
                break;
              }

              credentialsSaved = saveConfig(ssidVal, passwordVal, accessTokenVal);
              sendSuccessResponse(client);
              
              break;
            } else { // if you got a newline, then clear currentLine
              currentLine = "";
            }
          } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }
        }
      }
      // Clear the request variable
      request = "";
      // Close the connection
      client.stop();
      Serial.println("Client disconnected.");
    }
  }
}

bool cleanupAP() {
  server.stop();

  return true;
}
