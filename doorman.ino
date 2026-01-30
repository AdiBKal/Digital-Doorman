#include<HCSR04.h>
#include<BluetoothSerial.h>
#include <ESP32Servo.h>

#define TRIG_PIN 5
#define ECHO_PIN 18
#define LED_PIN 4 
#define THRESHOLD 50
#define doorPin 21
#define touch_pin 13
#define buzzer_pin 2

Servo door;
int doorPos=0;
char data = 0; 
const char *pin = "5678"; 
String device_name = "xyz"; 
BluetoothSerial SerialBT;
UltraSonicDistanceSensor distanceSensor (TRIG_PIN, ECHO_PIN);

void setup() 
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(touch_pin, INPUT);
  pinMode(buzzer_pin,OUTPUT);
  pinMode(doorPin, OUTPUT);
  door.attach(doorPin);        
  Serial.begin(9600);
  SerialBT.begin(device_name); 
  
  #ifdef USE_PIN
  SerialBT.setPin(pin);
  Serial.println("Using PIN");
  #endif
}

void loop() 
{
  int touchvalue = digitalRead(touch_pin);
  if(touchvalue == HIGH)
  {
    Serial.println("bell rang!!!");
    SerialBT.println("bell rang!!!");
    digitalWrite(buzzer_pin, HIGH);
  }
  else
  {
    digitalWrite(buzzer_pin, LOW);
  }
  delay(100);

  float distance = distanceSensor.measureDistanceCm();

  if (distance > 0 && distance < THRESHOLD) 
  {
    digitalWrite(LED_PIN, HIGH);
    Serial.println ("Activity detected near door!!!");
    SerialBT.println("Activity detected near door!!!");
  } 
  else 
  {
    digitalWrite(LED_PIN, LOW); 
  }
  delay(100);

  if (SerialBT.available()) 
  { 
    data = SerialBT.read(); 
    Serial.println(data);   
  
    if (data == '1') 
    {
      for (doorPos = 0; doorPos<=90; doorPos+= 1) 
      { 
        door.write(doorPos); 
        delay(15);
      }
      Serial.println("door opened!!!");
    }
    if (data == '0')
    {
      for(doorPos = 90; doorPos>=0; doorPos-=1)
      {
        door.write(doorPos);
        delay(15);
      }
      Serial.println("door closed!!!");
    }
  }  
}
