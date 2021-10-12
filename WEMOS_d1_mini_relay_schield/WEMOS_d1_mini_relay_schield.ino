////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////  Author: Jiri Kucera                           ////
////  Description: WEMOS Relay Schield, controlled   ////
////               a web server                     ////
////  Using board: WEMOS D1 mini                    ////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
////  include libraries                             ////
////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>
 
////////////////////////////////////////////////////////
////  setting wifi and specify relay pin            ////
////////////////////////////////////////////////////////

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";
 
const int relayPin = D1;
 
WiFiServer server(80);

////////////////////////////////////////////////////////
////  Setup                                         ////
//////////////////////////////////////////////////////// 

void setup() {
  // initialize serial for debugging
  Serial.begin(115200);
 
  // set relay pin
  pinMode(relayPin, OUTPUT);
  
  // turn off relay with voltage LOW
  digitalWrite(relayPin, LOW);  
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}

////////////////////////////////////////////////////////
////  Loop                                          ////
////////////////////////////////////////////////////////
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
  int value = HIGH;
  if (request.indexOf("/LED=ON") != -1)  {
    // turn on relay with voltage HIGH
    digitalWrite(relayPin, HIGH); 
    value = LOW;
  }

  if (request.indexOf("/LED=OFF") != -1)  {
    // turn off relay with voltage LOW
    digitalWrite(relayPin, LOW);  
    value = HIGH;
  }
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  // do not forget this one
  client.println(""); 
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Led/relay pin is now: ");
 
  if(value == LOW) {
    client.print("On");
  } else {
    client.print("Off");
  }

  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");  
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
