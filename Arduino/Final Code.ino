// Sensor Hr-SR04
#include "NewPing.h"
#define TRIGGER_PIN 7
#define ECHO_PIN 6
#define MAX_DISTANCE 400   
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
float duration, distance;

// Rotary Encoder
#define CLK 11
#define DT 12
#define SW 13
int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;

//L298n
int speed = 0;

//PID
#include <PID_v1.h>

double Setpoint ; // Counter
double Input; // Sensor
double Output=90 ; // FAN
//PID parameters
double Kp=0.01, Ki=0.05, Kd=0.01; 
 
//create PID instance 
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, REVERSE);

void setup() 
{
// Sensor Hr-SR04
    Serial.begin(9600);

// Rotary Encoder
    pinMode(CLK,INPUT);
    pinMode(DT,INPUT);
    pinMode(SW, INPUT_PULLUP);
    lastStateCLK = digitalRead(CLK);

//L298n
  pinMode(8, OUTPUT); //IN2
  pinMode(9, OUTPUT); //IN1
  pinMode(10, OUTPUT); //Enable Pin

//PID   
//  Setpoint = 10;
//  Setpoint = map(Setpoint,30,0,0,255);
//  myPID.SetMode(AUTOMATIC);
//  myPID.SetTunings(Kp, Ki, Kd);
}
 
void loop() 
{
  
// Sensor Hr-SR04
    distance = sonar.ping_cm();
//    Serial.print("Distance = ");
//    Serial.print(distance);
//    delay(500);


// Rotary Encoder     
    currentStateCLK = digitalRead(CLK);
    if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
        if (digitalRead(DT) != currentStateCLK) {
            counter --;
            currentDir ="CCW";
        } else {
            counter ++;
            currentDir ="CW";
        }
        Serial.print("Direction: ");
        Serial.print(currentDir);
        Serial.print(" | Counter: ");
        Serial.println(counter);
    }
    lastStateCLK = currentStateCLK;
    int btnState = digitalRead(SW);
    if (btnState == LOW) {
        if (millis() - lastButtonPress > 50) {
            Serial.println("Button pressed!");
            counter = 0;
        }
        lastButtonPress = millis();
    }
    delay(1);
    // counter = 0-15


//L298n
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
//  digitalWrite(10, HIGH);

//PID
  Setpoint = counter*2.5;
  Setpoint = map(Setpoint,30,0,0,255);
  myPID.SetMode(AUTOMATIC);
  myPID.SetTunings(Kp, Ki, Kd);
  Input = map(distance,0,30,0,255);
  myPID.Compute();
  analogWrite(10, Output);
  Serial.print("input : ");
  Serial.println(Input);
//  Serial.print(" | output : ");
//  Serial.println(Output);
  Serial.print(" |Setpoint : ");  
  Serial.println(Setpoint);
//  delay(500); 
  
}