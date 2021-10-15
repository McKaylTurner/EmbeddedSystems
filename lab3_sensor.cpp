#include <ESP8266WiFi.h> 
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#define ECHO D5                 // attach pin D2 Arduino to pin Echo of HC-SR04
#define TRIG D6                 //attach pin D3 Arduino to pin Trig of HC-SR04

const char* wifiSSID = "BYU-WiFi";    // In order for this to work, you MUST specify the SSID for your wifi
const char* wifiPSK  = "";            // And the preshared key (wifi password)

WiFiClient wifiClient;

char   HOST_NAME[] = "http://10.37.50.141/"; //ip of server
int distance_cm = 0;




long duration;                  // variable for the duration of sound wave travel

void setup() {


  pinMode(TRIG, OUTPUT);        // Sets the TRIG as an OUTPUT
  pinMode(ECHO, INPUT);         // Sets the ECHO as an INPUT
  Serial.begin(9600);           // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("HC-SR04 Distance Sensor"); // print some text in Serial Monitor

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

}

void loop() {
   HTTPClient http;
  
  digitalWrite(TRIG, LOW);    // Turn off the trigger and let things quiet down
  delay(20);                  // Let it sit for 20 milliseconds
  digitalWrite(TRIG, HIGH);   // on the trigger to start measurement
  delayMicroseconds(15);      // Send a very short pulse (15us)
  digitalWrite(TRIG, LOW);    // Turn off TRIG pin - this will start the ECHO pin high
  duration = pulseIn(ECHO, HIGH);   // Reads ECHO, returns the travel time in microseconds
  
  Serial.print("Distance: "); // debug out
  Serial.print(duration*0.013504/2); // Calculate the distance in inches (13,504 in/s)
  Serial.print("in. [");
  Serial.print(duration*0.0343/2);   // Calculate the distance in cm (34,300 cm/s)
  Serial.print("cm]\n");
  distance_cm = (duration*0.0343/2);
  
  //green
  if (distance_cm > 30) {
    //green on
    http.begin(wifiClient, "http://10.37.50.141/green=on");
    http.GET();
    http.end();

    Serial.println("green");
  }

  

  //yellow
  if ((distance_cm < 30) && (distance_cm > 20)) {
    //yellow on
    http.begin(wifiClient, "http://10.37.50.141/yellow=on");
    http.GET();
    http.end();

    Serial.println("yellow");
    
  }

  //turn on red
  if ((distance_cm < 20) && (distance_cm > 10)){

    //redoff
    http.begin(wifiClient, "http://10.37.50.141/red=on");
    http.GET();
    http.end();

    Serial.println("red");
    
  }

  //blink
  if (distance_cm < 10) {

    http.begin(wifiClient, "http://10.37.50.141/backUp=on");
    http.GET();
    http.end();

    Serial.println("blink");
   
  }
  delay(1000);                // Loop every 1 second
}
