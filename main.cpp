#include <ESP8266WiFi.h>              // This includes the libraries to connect to wifi and start a server

//const char* wifiSSID = "Ifiw"; 
const char* wifiSSID = "BYU-WiFi";    // In order for this to work, you MUST specify the SSID for your wifi
const char* wifiPSK  = "";            // And the preshared key (wifi password)

WiFiServer server(80);                // This sets which port our server will listen on

int green = D5;
int yellow = D7;
int red = D6;     
int blinkFreq = 1000; 
unsigned long blinkTimer = 0;                   // Sets which GPIO pin we will use for the LED
String greenState = "off";              // Initialize a variable for tracking the state of the LED
String yellowState = "off";              // Initialize a variable for tracking the state of the LED
String redState = "off"; 
String blinkState = "on";             // Initialize a variable for tracking the state of the LED
String color = "";

void setup() {                        // Perform this part once when first powered on
  Serial.begin(9600);               // Open a serial connection (for debugging)
  delay(10);                          // Wait 10 milliseconds (1/100th of a second)
  Serial.println("Initializing ..."); // DEBUG CODE

  pinMode(green, OUTPUT);            // Set the LED pin to Output
  pinMode(yellow, OUTPUT); 
  pinMode(red, OUTPUT); 
  digitalWrite(green, LOW);          // Set the pin to LOW (Off)

  // ** Connect to WiFi network - Adapted from http://www.esp8266learning.com/wemos-webserver-example.php
  Serial.print("Connecting to ");           // Display debugging connection info
  Serial.println(wifiSSID);                 // Print the configured SSID to the Serial Monitor

  WiFi.begin(wifiSSID, wifiPSK);            // Use the provided SSID and PSK to connect

  while (WiFi.status() != WL_CONNECTED) {   // If not connected to wifi
    delay(500);                             // Pause
    Serial.print(".");                      // Print a dot each loop while trying to connect
  }
  Serial.println("");
  Serial.println("WiFi connected");         // Print "connected" message to the Serial Monitor

  server.begin();                           // Start the web server
  Serial.println("Server started");

  Serial.print("Control the LED Flashlight at: ");          // Print the connected IP address to the Serial Monitor
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
} // ** End Adapted Code - This is the end of the code that was adapted from www.esp8266learning.com

void loop() { // ** Create a web server - Adapted from http://www.esp8266learning.com/wemos-webserver-example.php
  WiFiClient client = server.available(); // Create a new client object for available connections
  if (client) {                           // If a client is connected, wait until it sends some data
    while (!client.available()) {         // If the client hasn't sent info, wait for it
      delay(10);
    }

    String request = client.readStringUntil('\r');  // read the first line of the request
    Serial.println(request);              // DEBUG CODE - Echo the request to the Serial Monitor for debug
    client.flush();                       // Wait until the buffers are clear

    if (request.indexOf("/green=on") != -1) {   // If the request is for the page "/green=on"
      digitalWrite(green, HIGH);             //   then set the ledPin to high (turn on)
      greenState = "on";                        //   and track the state as "on"
    }
    if (request.indexOf("/green=off") != -1) {  // If the request is for the page "/green=off"
      digitalWrite(green, LOW);              //   then set the ledPin to low (turn off)
      greenState = "off";                       //   and track the state as "off"
    }
    if (request.indexOf("/red=on") != -1) {   // If the request is for the page "/green=on"
      digitalWrite(red, HIGH);             //   then set the ledPin to high (turn on)
      redState = "on";                        //   and track the state as "on"
    }
    if (request.indexOf("/red=off") != -1) {   // If the request is for the page "/red=off"
      digitalWrite(red, LOW);             //   then set the ledPin to high (turn off)
      redState = "off";                        //   and track the state as "off"
    }
    if (request.indexOf("/yellow=on") != -1) {   // If the request is for the page "/yellow=on"
      digitalWrite(yellow, HIGH);             //   then set the ledPin to high (turn on)
      yellowState = "on";                        //   and track the state as "on"
    }
    if (request.indexOf("/yellow=off") != -1) {   // If the request is for the page "/yellow=off"
      digitalWrite(yellow, LOW);             //   then set the ledPin to high (turn off)
      yellowState = "off";                        //   and track the state as "off"
    }
     if (request.indexOf("/blink=on") != -1) {   // If the request is for the page "/green=on"           //   then set the ledPin to high (turn on)
      blinkState = "on";                        //   and track the state as "on"
    }
    if (request.indexOf("/blink=off") != -1) {   // If the request is for the page "/green=on"           //   then set the ledPin to high (turn on)
      blinkState = "off";  
      digitalWrite(yellow, LOW); 
      digitalWrite(red, LOW);
      digitalWrite(green, LOW);  
      color = "off";                  //   and track the state as "on"
    }
    
    // ** End Adapted Code - This is the end of the code that was adapted from www.esp8266learning.com

    // Return the response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");

    // The concept and code of the sliding button on this page came from https://www.w3schools.com/howto/howto_css_switch.asp
    client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    client.println("<style>");
    client.println("html {text-align: center;}");
    client.println(".switch { position: relative; display: inline-block; width: 60px; height: 34px;}");
    client.println(".switch input {display:none;}");
    client.println(".slider { position: absolute; cursor: pointer; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; -webkit-transition: .4s; transition: .4s;}");
    client.println(".slider:before {position: absolute; content: \"\"; height: 26px; width: 26px; left: 4px; bottom: 4px; background-color: white; -webkit-transition: .4s;transition: .4s;}");
    client.println("input:checked + .slider {background-color: #2196F3;}");
    client.println("input:focus + .slider {box-shadow: 0 0 1px #2196F3;}");
    client.println("input:checked + .slider:before {-webkit-transform: translateX(26px); -ms-transform: translateX(26px); transform: translateX(26px);}");
    client.println(".slider.round { border-radius: 34px;}");
    client.println(".slider.round:before { border-radius: 50%;}");
    client.println("</style>");
    client.println("</head>");
    client.println("<body>");

    // This section modifies the page HTML based on the current state of the LED
//green
    client.print("<h2> Green </h2>");
    client.println("<label class='switch'>");
    client.print("  <input type=\"checkbox\" ");

    if (greenState == "on") {
      client.print("checked ");
    }
    client.print("onchange=\"window.location.href='/green=");
    if (greenState == "on") {
      client.print("off");
    } else {
      client.print("on");
    }
    client.println("'\">");
    client.println("  <span class=\"slider round\"></span>");
    client.println("</label>");
    client.print("  </br> </br> </br>");
    
   //yellow
    client.print("<h2> Yellow </h2>");
    client.println("<label class='switch'>");
    client.print("  <input type=\"checkbox\" ");
    if (yellowState == "on") {
      client.print("checked ");
    }
    client.print("onchange=\"window.location.href='/yellow=");
    if (yellowState == "on") {
      client.print("off");
    } else {
      client.print("on");
    }
    client.println("'\">");
    client.println("  <span class=\"slider round\"></span>");
    client.println("</label>");
    client.print("  </br> </br> </br>");

    //red
    client.print("<h2> Red </h2>");
    client.println("<label class='switch'>");
    client.print("  <input type=\"checkbox\" ");
    if (redState == "on") {
      client.print("checked ");
    }
    client.print("onchange=\"window.location.href='/red=");
    if (redState == "on") {
      client.print("off");
    } else {
      client.print("on");
    }
    client.println("'\">");
    client.println("  <span class=\"slider round\"></span>");
    client.println("</label>");
    client.print("  </br> </br> </br>");

    //blinking
    client.print("<h2> Blink </h2>");
    client.println("<label class='switch'>");
    client.print("  <input type=\"checkbox\" ");
    if (blinkState == "on") {
      client.print("checked ");
    }
    client.print("onchange=\"window.location.href='/blink=");
    if (blinkState == "on") {
      client.print("off");
    } else {
      client.print("on");
    }
    client.println("'\">");
    client.println("  <span class=\"slider round\"></span>");
    client.println("</label>");
    client.print("  </br> </br> </br>");


    client.println("</body>");
    client.println("</html>");
  }

if ((blinkState == "on") && (blinkTimer < millis())) {            // If it is time to change
    blinkTimer = millis() + blinkFreq;    // Set a new trigger time
    if (color == "off"){
      color = "green";
    }
    if (color == "green"){
      digitalWrite(red, LOW);
      digitalWrite(yellow, LOW);
      digitalWrite(green, HIGH);
      color = "yellow";
    }
    else if (color == "yellow"){
      digitalWrite(red, LOW);
      digitalWrite(green, LOW);
      digitalWrite(yellow, HIGH);
      color = "red";
    } 
    else if (color == "red"){
      digitalWrite(green, LOW);
      digitalWrite(yellow, LOW);
      digitalWrite(red, HIGH);
      color = "green";
    }                        // Toggle LED state
    
  } 


  delay(100); // This introduces a little pause in each cycle. Probably helps save some power.
}