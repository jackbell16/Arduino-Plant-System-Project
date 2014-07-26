/**
This program allows you to drive a pump, which will take care of a plant, even remotely
Author Giacomo Bellazzi
Version 1.0
*/
 
#include <SPI.h>
#include <Ethernet.h>
 
int pinPompa = 9; // Pump's PIN
boolean azionata = false; // Pump switch ON/OFF 
// Mac Address
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
 
EthernetServer server(80);
 
void setup() {
  pinMode(pinPompa,OUTPUT);
  digitalWrite(pinPompa,LOW);
  Serial.begin(9600);
  Ethernet.begin(mac);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP()); // Arduino IP
}
 
void loop() {
  // New client
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // HTTP request
    boolean currentLineIsBlank = true;
    String postText ="";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if(postText.length()<10){
          postText +=c;
        }
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close"); 
          client.println();
          client.println("<meta charset=UTF-8>");
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head> <TITLE>Arduino</TITLE> </head>"); // Webserver's title
          client.println("<body> <h1> Welcome to the page of the control system of your Garden </h1>"); 
          client.println("<h3> Through this page you can operate irrigation to wet a plant </h3>");
          client.println("</body>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    // Pump switch on-request
    if(postText.indexOf("?on") >0){
          digitalWrite(pinPompa,HIGH);
          Serial.println("Fai partire ");
          azionata = true;
        }
     // Pump switch off-request
     if(postText.indexOf("?off") >0 ){
       digitalWrite(pinPompa,LOW);
        Serial.println("Spegni LED");
        azionata = false;
       }
       
      // The web page changes
      if(azionata){
        client.println("<a href=\"./?off\"> <img src = \"http://static.rifaidate.it/impianti/impianti-idraulici/irrigazione-a-pioggia_O2.jpg\"alt = \"Spegni\" ></a>");
          client.println("<h3> Click on the image to stop the irrigation </h3>");
        }else{
          client.println("<a href=\"./?on\"> <img src = \"http://static.rifaidate.it/impianti/impianti-idraulici/irrigazione-a-pioggia_O2.jpg\"alt = \"Accendi\" ></a>");
          client.println("<h3> Click on the image to start the irrigation </h3>");
          }   
    delay(1);
    // Close connection
    client.stop();
    Serial.println("client disconnected");
  }
}
