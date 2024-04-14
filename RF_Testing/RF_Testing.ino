//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define BAUDRATE 9600
#define CE 9
#define CSN 8

// Motor pinout
#define in1 6

// Servo pinout
#define servo_pin A0

// Create an RF24 object
RF24 radio(CE, CSN);

// inputs
int inputs[] = {0,1};

// Create servo object
Servo servo;

// Unique address through which two modules communicate.
const byte address[6] = "00001";

int raw_analog_reading_x, raw_analog_reading_y;

void setup() {

  Serial.begin(BAUDRATE);

  radio.begin();
  
  //set the address
  radio.openReadingPipe(0, address);
  
  //Set module as receiver
  radio.startListening();

  // Set all the motor control pins to outputs
  pinMode(in1, OUTPUT);


  // Attach servo pin to Servo object
  servo.attach(servo_pin);
  
}

// Receives the x and y inputs from the remote
void receive_thumbstick_inputs() {

  // int inputs[2];
  // inputs[0] = 0;
  // inputs[1] = 1;
  // Read the data if available in buffer
  if(radio.available()) {
    radio.read(&inputs, sizeof(inputs));
  }

  // Refresh current joystick input readings
  raw_analog_reading_x = inputs[0];
  raw_analog_reading_y = inputs[1];

  Serial.print("Reading ");
  Serial.print(radio.available());
  if (!radio.available()){
    radio.begin();
  
    //set the address
    radio.openReadingPipe(0, address);
    
    //Set module as receiver
    radio.startListening();
    //delay(5000);
    Serial.println("reset");
  }
  
}

// Maps current reading of x thumbstick to servo arm
void map_servo() {

  // Tweak joystick min and max mapped values as necessary
  int servo_pos = map(raw_analog_reading_x, 0, 735, 0, 180);
  
  servo.write(servo_pos);
  
}

// Maps current reading of y thumbstick to motor
void map_motomoto() {
  int motor_speed = map(raw_analog_reading_y, 0, 734, 50, 255);
//  int motor_speed = 100;  // Feel free to tweak. For PWM maximum possible values are 0 to 255


  analogWrite(in1, motor_speed);
  // Serial.println(motor_speed);
  
}

void loop() {

  receive_thumbstick_inputs();
  
  map_servo();

  map_motomoto();

  Serial.print("  (X,Y)=(");
  Serial.print(raw_analog_reading_x);
  Serial.print(",");
  Serial.print(raw_analog_reading_y);
  Serial.println(")");
  delay(10);
 
}