#include <SoftwareSerial.h>
#define ESP8266_RX 2 //digital pin 2
#define ESP8266_TX 4 //digital pin 4
//set up serial object to communicate with NodeMCU
SoftwareSerial ESPserial(ESP8266_RX, ESP8266_TX);

const int trigPin = 7, echoPin = 8, speakerPin = 3;                    //trigPin is the trigger (output), the other is echo (input)
const int criticalDist = 30;                                          //comments
const int toneMin = 120, toneMax = 1500;
int userDelay;                                                         //delay for alarm to go off

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  Serial.println("Initialized.");
  ESPserial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  userDelay = getDelay();

  delay(userDelay);

  alarm(true);

  while(1){
    if(confirm()){
      alarm(false);
      break;
    }
  }
}

int getDelay(){
  while(1){
    Serial.println("Waiting for data");
    if (ESPserial.available()){
      //set hello to the original single-digit integer that was sent by the NodeMCU
      int hello = ESPserial.read()-48; 
      Serial.print(hello); //for debugging
//    return hello*1000;
      if(hello == 1){
        return 5*1000;
      }
      if(hello == 5){
       return 10*1000;
      }
    }
  }
}

boolean confirm() {
  int counter = 0;      //Counts how many times

  Serial.println("checking 10 times");
  
  for (int a = 0; a < 10; a++) {
    Serial.print("Distance (cm) = ");
    Serial.println(getDist());
    if (getDist() < criticalDist) {
      Serial.println(counter);
      counter++;
    }
  }

  if (counter >= 8) {
    Serial.println(counter);
    return true;
  }
  return false;
}

void alarm(boolean setting){
  if(setting){
    Serial.println("WAKEUP!");
    tone(speakerPin, (toneMin+toneMax)/2);
  }
  else{
    noTone(speakerPin);
  }
}

float getDist() {
  //First turns off trigger to ensure a clean signal. Sends high to trigPin for 10e-6s. Turns off trigger
  //Records the length of the pulse in e-6s; the length of the pulse represents how long it took for the signal to reach the ultrasonic sensor again
  //pulseIn() --> times how long a pulse lasts (either HIGH or LOW, you specify) at a specified pin
  //Performs a calculation for distance in cm and returns the distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  float duration = pulseIn(echoPin, HIGH);
  float distanceCm = ((343 * duration) / 10000) / 2;

  return distanceCm;
}
