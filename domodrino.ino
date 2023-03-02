
/*--------------------------------------------------------------------------------------------
  Program:      DOMODRINO : OPEN SOURCE BOX FOR HOME AUTOMATION WITH ARDUINO www.casasolar.com

  Description:  This program is part of a arduino proyect to make a box of sensors and relays
                to be controlled by a home automation system. The objective is replace commercial
                products with open source and cheap components.
  
  Hardware:     Arduino Uno rev3 and Arduino Ethernet shield.
                dht22 am2302 humidity and temperature sensor.
                
  Software:     Developed using Arduino 1:1.0.5 software under ubuntu 13.10
                Should be compatible with Arduino 1.0 +
  
  References:   - WebServer example by David A. Mellis and 
                  modified by Tom Igoe
                - Ethernet library documentation:
                  http://arduino.cc/en/Reference/Ethernet

  Date:        5 February 2014 V. 1.0.5
 
  Author:       Felisuco, http://www.felisuco.com
-------------------------------------------------------------------------------------*/

// imporante se me acaban los 16Ks de sram al compilar 16.656 aunque diga que hay un maximo de 32.256
// http://arduino.cc/es/Tutorial/Memory#.UwFYhtsju1E

#include <SPI.h>
#include <Ethernet.h>
/** INCLUYO LA LIBRERIA DEL SENSOR DHT ****/

/** DEFINIMOS LOS RELES ****/
#define RELAY1  2                        
#define RELAY2  3                        
#define RELAY3  4                        
#define RELAY4  5
#define RELAY5  6                      
#define RELAY6  7                      
#define RELAY7  8                       
#define RELAY8  9
/******* ENTRADA DIGITAL PARA LEER INTERRUPTOR *******************/
//int pinBoton = 10;   // Declaramos la variable pin del Botón
//int estadoBoton = 0;  // Variable para estado del botón

/******* SENSOR DE TEMPEARATURA ANALOGICO LM35 *******/
int entradatemperatura = 0;  //Pin analógico A0 del Arduino donde conectaremos el pin de datos del sensor LM35
float maxC = 0, minC = 100, maxF = 0, minF = 500;  //Variables para ir comprobando maximos y minimos

/******* SENSOR DE TEMPERATURA Y HUMEDAD DHT11 **********/
/*
#include "DHT.h"
#define DHTPIN 10 
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
*/
/******* ARDUINO SHIELD ETHERNET **********************/
// MAC address from Ethernet shield sticker under board, Important!! must be different on each arduino of the network
// ARDUINO 169
/*
byte mac[] = { 0xBE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // ETHERNET NUMBER MUST BE DIFFERENT on each arduino.
IPAddress ip(192, 168, 0, 169); // IP address, may need to change depending on network, and MUST BE DIFFERENT on each arduino.
EthernetServer server(80);  // create a server at port 80
*/
/*
// ARDUINO 170
byte mac[] = { 0xAE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEA };
IPAddress ip(192, 168, 0, 170); // IP address, may need to change depending on network, and MUST BE DIFFERENT on each arduino.
EthernetServer server(80);  // create a server at port 80
*/

// ARDUINO 171
byte mac[] = { 0xAE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE };
IPAddress ip(192, 168, 0, 171); // IP address, may need to change depending on network, and MUST BE DIFFERENT on each arduino.
EthernetServer server(80);  // create a server at port 80


/** CREAMOS LOS STRING PARA PODER ALMACENAR LOS COMANDOS WEB *****/
int tamanyostring = 100;
String readString = String(tamanyostring); //string for fetching data from address
///////////////////////
 String teststring = String(tamanyostring);
 String finalstring = String(tamanyostring);
 String flag = String(2);
 int ind1 = 0;
 int ind2 = 0;
 int pos = 0;
 //////////////////////

void setup()
{
    // INICIO ETHERNET SHIELD
    Ethernet.begin(mac, ip);  // initialize Ethernet device
    server.begin();           // start to listen for clients
    // INICIO ENTRADA DIGITAL PARA PULSADOR COMO ENTRADA
    //pinMode(pinBoton, INPUT); // CON LA ETHERNET SHIELD AL PONER ESTO COMO INPUT PETA, PERO SI NO USAMOS LA ETHERNET FUNCIONA, Y CON EL DHT11 LO MISMO

    // INICIAMOS LOS RELAYS CADA UNO TIENE UNA ENTRADA
    pinMode(RELAY1, OUTPUT); 
    digitalWrite(RELAY1, HIGH);    
    pinMode(RELAY2, OUTPUT);
    digitalWrite(RELAY2, HIGH);
    pinMode(RELAY3, OUTPUT);
    digitalWrite(RELAY3, HIGH);
    pinMode(RELAY4, OUTPUT);
    digitalWrite(RELAY4, HIGH);
    pinMode(RELAY5, OUTPUT); 
    digitalWrite(RELAY5, HIGH);    
    pinMode(RELAY6, OUTPUT);
    digitalWrite(RELAY6, HIGH);
    pinMode(RELAY7, OUTPUT);
    digitalWrite(RELAY7, HIGH);
    pinMode(RELAY8, OUTPUT);
    digitalWrite(RELAY8, HIGH);
       
}

void loop()
{
    // EMPEZAMOS EL LOOP DEL MICROPROCESADOR
    EthernetClient client = server.available();  // try to get client

    if (client) {  // got client?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                

              // Vamos almacenando lo que nos envia el cliente en un string caracter a caracter hasta 100...
              //read char by char HTTP request
              if (readString.length() < tamanyostring) {
              //store characters to string 
              readString += c; 
              }

 
                
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");  // the connection will be closed after completion of the response
                    client.println();
                    // VAMOAS A INTENTAR LEER PARAMETROS DEL CLIENTE
                    //GET /?accion=helloworld&enviar=si HTTP/1.1
                    
                    if(readString.indexOf("accion")>0) {
                            ind1 = readString.indexOf('=');
                            ind2 = readString.indexOf('&');
                            finalstring = readString.substring(ind1+1, ind2);
                            //client.println(finalstring+"<br>");
                            
                            // PROCESAMOS LAS ACCIONES QUE NOS ENVIE EL SISTEMA
                            /*if(finalstring == "holamundo")
                            {
                                client.println("Hola mundo"); 
                            }*/
                            /*else if(finalstring == "entrada")
                            {
                                 //estadoBoton = digitalRead(pinBoton);
                                 //client.print(estadoBoton);
                            }*/
                            if(finalstring == "temperatura")
                            {
                              //client.println("Hola mundosss"); 
                              float gradosC;//Declaramos estas variables tipo float para guardar los valores de lectura
                              gradosC = (5.0 * analogRead(entradatemperatura) * 100.0) / 1024;  //Esta es la funcion con la que obtenemos la medida del sensor
                              client.print(gradosC);
                              
                            }
                            else if(finalstring == "rele1on")
                            {
                                 digitalWrite(RELAY1,LOW);  
                            }
                            else if(finalstring == "rele1off")
                            {
                                 digitalWrite(RELAY1,HIGH); 
                            }
                            else if(finalstring == "rele2on")
                            {
                                 digitalWrite(RELAY2,LOW);  
                            }
                            else if(finalstring == "rele2off")
                            {
                                 digitalWrite(RELAY2,HIGH);  
                            }
                            else if(finalstring == "rele3on")
                            {
                                 digitalWrite(RELAY3,LOW);  
                            }  
                            else if(finalstring == "rele3off")
                            {
                                 digitalWrite(RELAY3,HIGH);  
                            } 
                            else if(finalstring == "rele4on")
                            {
                                 digitalWrite(RELAY4,LOW);  
                            }
                            else if(finalstring == "rele4off")
                            {
                                 digitalWrite(RELAY4,HIGH);  
                            }
                           else if(finalstring == "rele5on")
                            {
                                 digitalWrite(RELAY5,LOW);  
                            }  
                            else if(finalstring == "rele5off")
                            {
                                 digitalWrite(RELAY5,HIGH);  
                            }   
                            else if(finalstring == "rele6on")
                            {
                                 digitalWrite(RELAY6,LOW);  
                            }  
                            else if(finalstring == "rele6off")
                            {
                                 digitalWrite(RELAY6,HIGH);  
                            }   
                            else if(finalstring == "rele7on")
                            {
                                 digitalWrite(RELAY7,LOW);  
                            }  
                            else if(finalstring == "rele7off")
                            {
                                 digitalWrite(RELAY7,HIGH);  
                            }   
                            else if(finalstring == "rele8on")
                            {
                                 digitalWrite(RELAY8,LOW);  
                            }  
                            else if(finalstring == "rele8off")
                            {
                                 digitalWrite(RELAY8,HIGH);  
                            }                    
                             
                    }
                    else // MOSTRAMOS EL MENU WEB
                    {
                         // SACAMOS LA WEB ENTERITA CON TODA LA INFO
          	        //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
                        client.println("<!DOCTYPE HTML>");
                        client.println("<html>");
                        client.println("<head>");
                        client.println("<title>MODULO DE DOMOTICA CON ARDUINO BY FELIX MORENO</title>");
                        client.println("</head>");
                        client.println("<body>");
                        
                        client.println("SISTEMA DE DOMOTICA 1.0<bR>");
/*                        client.println("<A HREF=\"http://www.casasolar.com\">www.casasolar.com</a><bR>");
                       
                       
                        
*/
/*                        client.println("Estado entrada 1: Deshabilitado");
                        estadoBoton = digitalRead(pinBoton);
                        client.print(estadoBoton); 
*/
                        client.print("<BR>Temperatura LM35: ");
                        float gradosC;  //Declaramos estas variables tipo float para guardar los valores de lectura
                        gradosC = (5.0 * analogRead(entradatemperatura) * 100.0) / 1024;  //Esta es la funcion con la que obtenemos la medida del sensor
                        client.print(gradosC);   
                        client.print("<br>");
                        // TEMPERATURA Y HUMEDAD
                        /*                   
                        float h = dht.readHumidity();
                        // Read temperature as Celsius
                        float t = dht.readTemperature();
                        // Read temperature as Fahrenheit
                        float f = dht.readTemperature(true);

                        if (isnan(h) || isnan(t) || isnan(f)) {
                          client.print("Failed to read from DHT sensor!");
                        }
                        else
                        {                      
                          
                          client.print("<BR>HUMEDAD DHT11:");
                          client.print(h);
                          client.print("<BR>TEMPERATURA DHT11:");
                          client.print(t);
                                    client.print("<BR>TEMPERATURA DHT11:");
                          client.print(t);
                        }*/
                   
                        client.print("<br><a href=\"?accion=rele1on&enviar\">RELE1 ON</A> - ");
                        client.print("<a href=\"?accion=rele1off&enviar\">RELE1 OFF</A>");
                        client.print("<br>");  
                        client.print("<a href=\"?accion=rele2on&enviar\">RELE2 ON</A> - ");
                        client.print("<a href=\"?accion=rele2off&enviar\">RELE2 OFF</A>");
                        client.print("<br>");  
                        client.print("<a href=\"?accion=rele3on&enviar\">RELE3 ON</A> - ");
                        client.print("<a href=\"?accion=rele3off&enviar\">RELE3 OFF</A>");
                        client.print("<br>");  
                        client.print("<a href=\"?accion=rele4on&enviar\">RELE4 ON</A> - ");
                        client.print("<a href=\"?accion=rele4off&enviar\">RELE4 OFF</A>");                    
                        client.print("<br>");  
                        client.print("<a href=\"?accion=rele5on&enviar\">RELE5 ON</A> - ");
                        client.print("<a href=\"?accion=rele5off&enviar\">RELE5 OFF</A>"); 
                        client.print("<br>");  
                        client.print("<a href=\"?accion=rele6on&enviar\">RELE6 ON</A> - ");
                        client.print("<a href=\"?accion=rele6off&enviar\">RELE6 OFF</A>"); 
                        client.print("<br>");  
                        client.print("<a href=\"?accion=rele7on&enviar\">RELE7 ON</A> - ");
                        client.print("<a href=\"?accion=rele7off&enviar\">RELE7 OFF</A>"); 
                        client.print("<br>");  
                        client.print("<a href=\"?accion=rele8on&enviar\">RELE8 ON</A> - ");
                        client.print("<a href=\"?accion=rele8off&enviar\">RELE8 OFF</A>"); 
                       client.print("<br>");  
                       
                        client.print("<a href=\"?accion=temperatura&enviar\">TEMPERATURA LM35</A>");
/*                        client.print("<br>");
                        client.print("<a href=\"?accion=entrada&enviar\">ENTRADA BOTON</A>");
                        client.print("<br>");
                        client.print("<a href=\"?accion=humedad&enviar\">HUMEDAD DHT11</A>"); 
                        client.print("<br>");
                        client.print("<a href=\"?accion=holamundo&enviar\">HOLA MUNDO</A>"); 
*/
                        client.println("<br></body>");
                        client.println("</html>");                                            
                    }  
                    //Clearing string for next read - Limpiamos el string
                    readString="";
                    teststring="";
                    finalstring="";
                    

                    break;
                }
                // every line of text received from the client ends with \r\n
                if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
            } // end if (client.available())
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } // end if (client)
}
