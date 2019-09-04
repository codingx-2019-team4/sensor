#include <ESP8266WiFi.h>

const char *ssid="willy";
const char *pass="swc547swc547";
#include <MQ2.h>
int pin=A0;
int lpg,co,smoke;
MQ2 mq2(pin);

#include <DHT.h>
#define PIN 4
#define TYPE DHT22
DHT dht(PIN,TYPE);


const uint16_t port =8090;
const char * host = "192.168.43.34"; //to get IP 
WiFiClient client;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(10);
  Serial.print("Connecting to");
  Serial.println(ssid);

  WiFi.begin(ssid,pass);

  while(WiFi.status() !=WL_CONNECTED ){
    delay(500);
    Serial.print(" ");
  }
  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());
 
  mq2.begin();
  //if(client.connect(host,port)){
  //  Serial.println("Connected to server successful!");
  //  }     "184.106.153.149" or api.thingspeak.com
  //else{
    
  //  Serial.println("no");
  //}    
      
}

void loop() {
  // put your main code here, to run repeatedly:
  float* values=mq2.read(true);
  lpg=mq2.readLPG();
  co=mq2.readCO();
  smoke=mq2.readSmoke();
  Serial.print("get smock");
  delay(500);
 
  float h = dht.readHumidity();
  float t=dht.readTemperature();
  float f=dht.readTemperature(true);
  if(isnan(h)|| isnan(t)||isnan(f)){
    Serial.println("FUCH there is nothing in here");
    return;
  }


   if (client.connect(host,port))    // "184.106.153.149" or api.thingspeak.com
      {  
      Serial.println("Connected to server successful!");
      Serial.print("濕度");
      Serial.print(h,2);
      Serial.print(",");
      Serial.print("溫度");
      Serial.println(t);  
      Serial.print("lpg");
      Serial.println(lpg);
      Serial.print("smoke");
      Serial.println(smoke);
      Serial.print("co");
      Serial.println(co);
      String postStr = "2"  ;
      postStr +=",";
      postStr += String(t);
      postStr +=",";
      postStr += String(smoke);
      
     
 
      client.print(postStr);
      Serial.println(postStr);
      }
      else{
      Serial.println("faild");   
        
        }
      delay(4000);
                        
 
 
   
  

}
