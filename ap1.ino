#include <DHT11.h>
#include <SPI.h>
#include <Ethernet.h>
byte mac[]={ 0x90, 0xA2, 0xDA, 0x00, 0x98, 0x36};
byte ip[]= {192,168,0,200};
EthernetServer server(80);

int pin_SensorTEMHUM=2;
int Pin_ventilador = 5;
int Pin_Motor = 7;


DHT11 dht11(pin_SensorTEMHUM); 
String readString =String(30);
String statusVentilador;
String statusMotor;

void setup() {
  Serial.begin(9600); // Inicializamos la comunicación serie
  Ethernet.begin(mac,ip)  ; // Inicializamos la comunicación Ethernet y el servidor
  pinMode(Pin_ventilador,OUTPUT);
  pinMode(Pin_Motor,OUTPUT);
}

void loop() {
  EthernetClient client = server.available();
  if(client){ 
    int err;
    float temp, hum;
     
       
    while(client.connected()){
      if(client.available()){
        char c = client.read();
        if(readString.length()<30){
          readString+=(c);
        }
        if(c=='\n'){
          if(readString.indexOf("Ventilador")>=0){
            digitalWrite(Pin_ventilador, !digitalRead(Pin_ventilador));
            //digitalWrite(Pin_ventilador, !digitalRead(Pin_Motor));
          }
          if(readString.indexOf("Motor")>0){
            digitalWrite(Pin_Motor, !digitalRead(Pin_Motor));
          }
        
          //Encabezado http padre
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
        client.println("<!doctype html>");
        client.println("<html>");
        client.println("<head>");
        client.println("<title>CLICK</title>");
        client.println("<meta name=\"viewport\" content=\"width=320\">");
        client.println("<meta name=\"viewport\" content=\"width=device-width\">");
        client.println("<meta charset=\"utf-8\">");
        client.println("<meta name=\"viewport\" content=\"initial-scale=1.0, user-scale=no\">");
        client.println("<meta http-equiv=\"refresh\" content=\"2,URL=http://192.168.0.200:80\"");
        client.println("</head>");
        client.println("<body>");
        client.println("<center>");
               
        
        if(digitalRead(Pin_ventilador)){
          statusVentilador="Desactivado";
        }else{
          statusVentilador="Activado";
        }
        if(digitalRead(Pin_Motor)){
          statusMotor="Desactivado";
        }else{          
          statusMotor="Activado";
        }
   
        if((err = dht11.read(hum, temp)) == 0){
            temp;
            hum;
             client.println("<font size=\"5\" face=\" verdana\" color=\"green\"> Temperatura: </font>");
             client.println("<strong>");
             client.println(temp);
             client.println("</strong>");
             client.println();
             client.println("<font size=\"5\" face=\" verdana\" color=\"green\"> Humedad:  </font>");
              client.println("<strong>");
             client.println(hum);
             client.println("</strong>");
             client.println();
             client.println("<br/><br/>");
          }
       else
          {
             client.println();
             client.println("Error Num :");
             client.println(err);
             client.println();
          }
       

       
        
        client.println("<form action=\"Ventilador\" method=\"get\">");
        client.println("<button type=submit style=\"width:200px;\">Ventilador- "+statusVentilador + "</button>");
        client.println("</form> <br />");
        
        client.println("<form action=\"Motor\" method=\"get\">");
        client.println("<button type=submit style=\"width:200px;\">Motor- "+statusMotor + "</button>");
        client.println("</form> <br />");
        client.println("</center>");
        
        readString="";
       
        client.stop();
          
        }
      }
    }
  }
  
}
