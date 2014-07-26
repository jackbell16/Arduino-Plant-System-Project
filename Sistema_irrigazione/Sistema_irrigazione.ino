/**
Questo programma permette di azionare l'irrigazione del giardino, per un determinato periodo 
Author Giacomo Bellazzi
Version 1.0
*/
 
#include <SPI.h>
#include <Ethernet.h>
 
int pinPompa = 9; // pin a cui è connessa la pompa
boolean azionata = false; 
// Mac Address di Arduino
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
 
// Viene inizializzata la libreria Ethernet di Arduino e il webserver gira sulla porta 80
EthernetServer server(80);
 
void setup() {
  pinMode(pinPompa,OUTPUT);
  digitalWrite(pinPompa,LOW);
  Serial.begin(9600);
  // Viene inilizzato il webserver e la connessione di rete
  Ethernet.begin(mac);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}
 
void loop() {
  // Vengono ascoltati nuovi client
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // Finisce una richiesta HTTP
    boolean currentLineIsBlank = true;
    String postText ="";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if(postText.length()<10){
          postText +=c;
        }
        // Se viene completato l'invio della richiesta HTTP, allora il server invia la risposta
        if (c == '\n' && currentLineIsBlank) {
          // Viene fatta una risposta HTTP, in pratica viene creata una pagina WEB in HTML
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // Dopo la risposta la connessione si interrompe
          client.println();
          client.println("<meta charset=UTF-8>"); // serve per inserire i caretteri speciali
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head> <TITLE>Arduino</TITLE> </head>"); // Viene creato il Titolo
          client.println("<body> <h1> Benvenuto nella pagina di controllo del sistema</h1>"); // Viene inserito del testo
          client.println("<h3> Attraverso questa pagina è possibile azionare l'irrigazione, per bagnare una pianta </h3>");
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
    // Se l'utente ha premuto l'icona per accendere il LED
    if(postText.indexOf("?on") >0){
          digitalWrite(pinPompa,HIGH);
          Serial.println("Fai partire ");
          azionata = true;
        }
     // Se l'utente ha premuto l'icona per spegnere il LED
     if(postText.indexOf("?off") >0 ){
       digitalWrite(pinPompa,LOW);
        Serial.println("Spegni LED");
        azionata = false;
       }
       
      // Viene cambiata la pagina WEB a seconda che il LED sia spento, oppure azionata
      if(azionata){
        client.println("<a href=\"./?off\"> <img src = \"http://static.rifaidate.it/impianti/impianti-idraulici/irrigazione-a-pioggia_O2.jpg\"alt = \"Spegni\" ></a>");
          client.println("<h3> Clicca sull'immagine per fermare l'irrigazione </h3>");
        }else{
          client.println("<a href=\"./?on\"> <img src = \"http://static.rifaidate.it/impianti/impianti-idraulici/irrigazione-a-pioggia_O2.jpg\"alt = \"Accendi\" ></a>");
          client.println("<h3> Clicca sull'immagine per far partire l'irrigazione </h3>");
          }   
    delay(1);
    // Viene chiusa la connessione
    client.stop();
    Serial.println("client disconnected");
  }
}
