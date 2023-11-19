#include <PID_v1.h>
#include <Encoder.h>
#include <NewPing.h>
//Encoder Configuration
Encoder e_right(4, 3);
Encoder e_left(2, 6);
//Motors Configuration
const int MDR = 12;// Right motor direction 
const int MSR = 5; // Right motor Speed
const int MBR = 9; // Right motor Break 
const int MDL = 13; // Left motor
const int MSL = 10;
const int MBL = 8;
//Ultrasonic Configuration 
#define TRIGGER_PIN  8  // Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PIN     7  // Arduino pin tied to echo pin on ping sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned int pingSpeed = 50; // How frequently are we going to send out a ping (in milliseconds). 50ms would be 20 times a second.
unsigned long pingTimer;     // Holds the next ping time.
//PID
double setpointA = 75;
double outputA = 0; 
double setpointB = 75;
double outputB = 0;            // output is position of encoder
double pwmoA=0;
double pwmoB=0;
double positionLeft  = 0;
double positionRight = 0;
double inputA  = 0;
double inputB = 0;
double KpA = 0.9, KiA = 0.7, KdA =0;
double KpB = 0.9, KiB = 0.5, KdB =0;

PID pidL(&inputA, &outputA, &setpointA, KpA, KiA, KdA, DIRECT);
PID pidR(&inputB, &outputB, &setpointB, KpA, KiA, KdA, DIRECT);
double storeB = 0;


void setup() {
  // put your setup code here, to run once:
    initMotors();
   pidL.SetMode(AUTOMATIC);
   pidR.SetMode(AUTOMATIC);

  pidL.SetOutputLimits(0, setpointA);
  pidL.SetSampleTime(50);
  pidR.SetOutputLimits(0, setpointB);
  pidR.SetSampleTime(50);
  pingTimer = millis();

  
  


Serial.begin(115200);
}


void loop() {
  // put your main code here, to run repeatedly:
    while (Serial.available() > 0) {
    if (millis() >= pingTimer) {   // pingSpeed milliseconds since last ping, do another ping.
    pingTimer += pingSpeed;      // Set the next ping time.
    sonar.ping_timer(echoCheck); // Send out the ping, calls "echoCheck" function every 24uS where you can check the ping status.
  }
    String direction="";
    String data = Serial.readStringUntil('\n');
    direction=data;
    if (direction =="S"){
      forwardl(200);
      forwardr(200);
      pidL.Compute();
      pidR.Compute();
      forwardr(outputB);
      forwardl(outputA);
      }
    if (direction =="R"){
      forwardr(0);
      forwardl(200);
      pidL.Compute();
      pidR.Compute();
      forwardr(outputB);
      forwardl(outputA);}
      

    if(direction=="L") {
      forwardl(0);
      forwardr(200);
      pidL.Compute();
      pidR.Compute();
      forwardr(outputB);
      forwardl(outputA);
      } 

 
}}
   long newLeft, newRight;
  newLeft=(e_left.read());
  newRight=(e_right.read());
  inputA=(newLeft);
  inputB=(newRight);


  
  
}

void encoder(){
  long newLeft, newRight;
  newLeft = e_left.read();
  newRight = e_right.read();
  if (newLeft != positionLeft || newRight != positionRight) {
   
    positionLeft = newLeft;
    positionRight = newRight;
    if (Serial.available()) {
    Serial.read();
    Serial.println("a");
    e_left.write(0);
    e_right.write(0);
  }

}}
void forwardr(uint16_t pwm){
  digitalWrite(MDR, LOW);
  analogWrite(MSR, pwm);
}
void forwardl(uint16_t pwm){
  digitalWrite(MDL, HIGH);
  analogWrite(MSL, pwm);
}
void reverser(uint16_t pwm){
  digitalWrite(MDR, LOW);
  analogWrite(MSR, pwm);
}
void reversel(uint16_t pwm){
  digitalWrite(MDL, LOW);  
  analogWrite(MSL, pwm);
}
void braker(){
  digitalWrite(MBR, HIGH);
}
void brakel(){
  digitalWrite(MBL, HIGH);
}
void release_braker(){
  digitalWrite(MBR, LOW);
}
void release_brakel(){
  digitalWrite(MBL, LOW);
}
void hold(){
  braker();
  brakel();
  delay(500);
  release_braker();
  release_brakel();
}
void echoCheck() { // Timer2 interrupt calls this function every 24uS where you can check the ping status.
  // Don't do anything here!
  if (sonar.check_timer()) { // This is how you check to see if the ping was received.
    // Here's where you can add code.
 
    int distance=(sonar.ping_result / US_ROUNDTRIP_CM); // Ping returned, uS result in ping_result, convert to cm with US_ROUNDTRIP_CM.
    if (distance<30){;braker();brakel();}
    else{release_brakel();release_braker();}
  }}

void initMotors(){
  pinMode(MDR, OUTPUT);
  pinMode(MSR, OUTPUT);
  pinMode(MBR, OUTPUT);
  pinMode(MDL, OUTPUT);
  pinMode(MSR, OUTPUT);
  pinMode(MBL, OUTPUT);}