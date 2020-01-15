// Include the ESP32 Arduino Servo Library instead of the original Arduino Servo Library
#include <ESP32_Servo.h> 
#include <WiFi.h>


/******* WiFi *******/
// Replace with your network credentials
const char* ssid     = "xxxxxx";
const char* password = "xxxxxx";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
/***********************/

/**** L293D (DC Motors) ****/
#define MR_EN 32  // control speed(PWM)
#define MR_C1 14  // control direction
#define MR_C2 15  // control direction

#define ML_EN 33  // control speed(PWM)
#define ML_C1 27  // control direction
#define ML_C2 12  // control direction
 
// Auxiliar variables to store the current output state
int motorStateInt = 0; 
String motorState;
/***********************/

/**** Servos(Moving EyeBall) ****/
// GPIO pin used to connect the servo control (digital out)
int upDownServoPin = 21; 
int rightLeftServoPin =  13;
Servo upDownServo; //Eye Lid
Servo rightLeftServo; //Eye Ball

//define position name and value
#define left 0
#define right 180
#define middle 70
#define closed 50
#define fullOpen 180
#define waitTime 750
/***********************/


void setup() {
  Serial.begin(115200);
  
/**** Servos (Moving EyeBall) ****/
  rightLeftServo.attach(rightLeftServoPin);   
  upDownServo.attach(upDownServoPin);   
                                         
  //starting position of the servo motors
  delay(10);
  upDownServo.write(fullOpen);
  rightLeftServo.write(middle);
/***********************/

/**** L293D (DC Motors) ****/
  pinMode(MR_EN, OUTPUT);
  pinMode(MR_C1, OUTPUT);
  pinMode(MR_C2, OUTPUT);

  pinMode(ML_EN, OUTPUT);
  pinMode(ML_C1, OUTPUT);
  pinMode(ML_C2, OUTPUT);

  digitalWrite(MR_EN, HIGH);
  digitalWrite(ML_EN, HIGH);

  stopAll(); //Stop Motors
/***********************/

/***************** WiFi *****************/
 // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
/*******************************************/ 
}



void loop() {
  upDownServo.write(fullOpen);
/********************** WiFi Connection ***********************************/
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
/*****************************************************************************/          

/********************** Webpage GET Request ***********************************/
            // turns the GPIOs on and off
            if (header.indexOf("GET /motor/stop") >= 0) {
              motorStateInt = 0; 
            }else if (header.indexOf("GET /motor/forward") >= 0) {
              motorStateInt = 1; 
            } else if (header.indexOf("GET /motor/backward") >= 0) {
              motorStateInt = 2;
            } else if (header.indexOf("GET /motor/go-left") >= 0) {
              motorStateInt = 3;
            } else if (header.indexOf("GET /motor/go-right") >= 0) {
              motorStateInt = 4;
            }  
/*****************************************************************************/ 

/********************** Movement Conditions ***********************************/
            switch (motorStateInt) {
              case 0:{
                motorState = "stop";
                stopAll();
                lookAround();
                break;
              }
              case 1:
                goForward();
                motorState = "forward";
                break;
              case 2:
                goBackward();
                motorState = "backward";
                break;
              case 3:
                goLeft();
                motorState = "go-left";
                break;
              case 4:
                goRight();
                motorState = "go-right";
                break;
              default:
                // if nothing else matches, do the default, default is optional
                stopAll();
                break;
            }
/*****************************************************************************/ 
            
            displyWebPage(client);
            displayMotorButtonStop(client, motorState);
            displayMotorButtonFB(client, motorState);   
            displayMotorButtonRL(client, motorState);             
            
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}


/*------------------------ Functions ------------------------*/ 
/********** DC Motor Movement ***********/ 
void goBackward(){
  leftMotorForward();
  rightMotorForward();
}
void goForward(){
  leftMotorBackward();
  rightMotorBackward();
}
void goRight(){
  rightMotorForward();
  leftMotorBackward();
}
void goLeft(){
  leftMotorForward();
  rightMotorBackward();
}
void stopAll(){
  leftMotorStop();
  rightMotorStop();
}
void leftMotorForward(){
  digitalWrite(ML_C1, HIGH);
  digitalWrite(ML_C2, LOW);
}
void leftMotorBackward(){
  digitalWrite(ML_C1, LOW);
  digitalWrite(ML_C2, HIGH);
}
void leftMotorStop(){
  digitalWrite(ML_C1, LOW);
  digitalWrite(ML_C2, LOW);
}
void rightMotorStop(){
  digitalWrite(MR_C1, LOW);
  digitalWrite(MR_C2, LOW);
}
void rightMotorForward(){
  digitalWrite(MR_C1, HIGH);
  digitalWrite(MR_C2, LOW);
}
void rightMotorBackward(){
  digitalWrite(MR_C1, LOW);
  digitalWrite(MR_C2, HIGH);
}
/*********************************************/

/************ Moving EyeBall ****************/
void lookAround(){
  delay(waitTime);
  upDownServo.write(closed);
  delay(waitTime);
  upDownServo.write(fullOpen);
  delay(waitTime);
  rightLeftServo.write(right);
  delay(waitTime);
  rightLeftServo.write(left);
  delay(waitTime);
  rightLeftServo.write(middle);
  upDownServo.write(fullOpen);
}
/*****************************************************/ 

/**************** Webpage Display ********************/
void displyWebPage(WiFiClient c){
  // Display the HTML web page
  c.println("<!DOCTYPE html><html>");
  c.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  c.println("<link rel=\"icon\" href=\"data:,\">");
  // CSS to style the on/off buttons 
  // Feel free to change the background-color and font-size attributes to fit your preferences
  c.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  c.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
  c.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  c.println(".button2 {background-color: orange;}");
  c.println(".button3 {background-color: red;}");
  c.println(".button4 {background-color: gray;}</style></head>");
  
  // Web Page Heading
  c.println("<body><h1>My WiFi Robot</h1>");
}

void displayMotorButtonStop(WiFiClient c, String pinState){ 
  // If the motorState is "stop", it hides the "Stop" button 
  if(pinState == "stop"){
    c.println("<h2>Safe Driving</h2>");
  }else{ 
    c.println("<p><a href=\"/motor/stop\"><button class=\"button button3\">Stop</button></a></p>");
  }
}

void displayMotorButtonFB(WiFiClient c, String pinState){
  // Display current state, and Forward/Backward button 
  c.println("<p>GPIO Motor - State " + pinState + "</p>"); 
  // If the motorState is "forward", it displays the "Backward" button  
  if(pinState == "forward"){
    c.println("<p><a href=\"/motor/backward\"><button class=\"button\">Backward</button></a></p>");
  }else{ 
    c.println("<p><a href=\"/motor/forward\"><button class=\"button button2\">Forward</button></a></p>");
  }
}

void displayMotorButtonRL(WiFiClient c, String pinState){
  // Display current state, and Turn Left/Right button
  c.println("<p>GPIO Motor - State " + pinState + "</p>");
  // If the motorState is "go-right", it displays the "Turn Left" button     
  if(pinState == "go-right"){
    c.println("<p><a href=\"/motor/go-left\"><button class=\"button\">Turn Left</button></a></p>");
  }else{ 
    c.println("<p><a href=\"/motor/go-right\"><button class=\"button button2\">Turn Right</button></a></p>");
  }
}
/*********************************************************************/ 
