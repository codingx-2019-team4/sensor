#include <ESP8266WiFi.h>
#include <MQ2.h>

#include <DHT.h>
#define PIN 4
#define TYPE DHT22
DHT dht(PIN,TYPE);
const char *ssid="willy";
const char *pass="swc547swc547";
String apiKey = "Y8B6UPJ7A6UKJUI1";     //thingsspeak  Enter your Write API key from ThingSpeak  
const char* server = "api.thingspeak.com";     //thingsspeak  
WiFiClient client;
int pin=A0;
int lpg,co,smoke;

MQ2 mq2(pin);
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
  Serial.print(" ");
  Serial.print("WiFi connected");
  mq2.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  float* values=mq2.read(true);
  lpg=mq2.readLPG();
  co=mq2.readCO();
  smoke=mq2.readSmoke();
  delay(1000);
  Serial.print(lpg);
  Serial.print(smoke);
  Serial.println(co);
  float h = dht.readHumidity();
  float t=dht.readTemperature();
  float f=dht.readTemperature(true);
  if(isnan(h)|| isnan(t)||isnan(f)){
    Serial.println("FUCH there is nothing in here");
    return;
  }
  Serial.print("濕度");
  Serial.print(h,2);
  Serial.print(",");
  Serial.print("溫度");
  Serial.println(t,2);

  
  if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
      {  
                            
       String postStr = apiKey;
       postStr +="&field1=";
       postStr += String(t);
       postStr +="&field2=";
       postStr += String(h);
       postStr += "\r\n\r\n";
 
       client.print("POST /update HTTP/1.1\n");
       client.print("Host: api.thingspeak.com\n");
       client.print("Connection: close\n");
       client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
       client.print("Content-Type: application/x-www-form-urlencoded\n");
       client.print("Content-Length: ");
       client.print(postStr.length());
       client.print("\n\n");
       client.print(postStr);
 
       Serial.print("溫度: ");
       Serial.print(t);
       Serial.print(" smoke ");
       Serial.print(smoke);
       Serial.println("%. Send to Thingspeak.");
                        }
  client.stop();
  delay(3000);
 

}
