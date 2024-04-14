//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//Pins
#define slide A0 //Potentiometer (gyro)
#define motor A1 //Potentiometer (motor)

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";

struct Data_Package {
  byte Spin;
  byte Wheel;
  };
Data_Package data; 

void setup()
{
  radio.begin();
  
  //set the address
  radio.openWritingPipe(address);
  //sets up as trasnsmitter, turns off recieving
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  //Low amplification to lower noise
  radio.setPALevel(RF24_PA_LOW);

  //Pullup resistors for pins... for lesser noise?
  pinMode(slide, INPUT_PULLUP);
}
void loop()
{
  //Read from potentiometers
  data.Spin = map(analogRead(slide), 0, 1023, 0, 255); 
  data.Wheel = map(analogRead(motor), 0, 1023, 0, 255);

  radio.write(&data, sizeof(Data_Package));
}


