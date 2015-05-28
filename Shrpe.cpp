/*
  Shrpe.cpp
  - Library for usage together with the SHRPE shield.
  Authors: Andreas Drotth & Soded Alatia, 
  Created Date: April 9, 2015.
  [license, domain etc?]
*/

#include "Shrpe.h"
#include "Arduino.h"
#include "packet_framing_library\CRC_16.cpp"
#include "packet_framing_library\Framing.cpp"
#include "packet_framing_library\Timer.cpp"

void shrpe_irq_handler();

volatile boolean flag = false;

Framing framing;

Shrpe::Shrpe()
{
	
}

void Shrpe::begin()
{
  delay(6000); // wait for the shield to boot up
  Serial.begin(115200);
/*   byte dummyBuffer[Serial.available()];
  if (Serial.available() > 0){
	  byte dummyBuffer[Serial.available()];
	  Serial.readBytes(dummyBuffer, Serial.available());
  } */
  framing.setTimout(0.1);
  pinMode(2, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  digitalWrite(2, HIGH);
  attachInterrupt(0, shrpe_irq_handler, FALLING);
}

void Shrpe::uploadObject(uint8_t data_byte)
{
  uint8_t data_array[1] = {data_byte};
  uploadObject(data_array, 1);
}

void Shrpe::uploadObject(uint8_t array[], uint8_t size)
{
  //Serial.write(19); //dummy to notice shield we're about to send
  delay(10);
  framing.sendFramedData(array, size);
}

uint8_t Shrpe::getData(void)
{
   if (flag){
	   flag = false;
	   digitalWrite(13, !digitalRead(13));
	   return 1;
   } else {
	   return 0;
   }
}

void shrpe_irq_handler(){
	flag = true;
	//digitalWrite(13, !digitalRead(13));
	int bytes_available = Serial.available();
	//Serial.print("Bytes available: ");
    //Serial.println(bytes_available, DEC);
	
	if (bytes_available){
		//char buffer[bytes_available +1];
		//memset(buffer, 0, sizeof(buffer));
		//Serial.readBytes(buffer, bytes_available);
		//char incoming = Serial.read();
		//Serial.print("Recieved: ");
		//Serial.println(incoming);
		//Serial.println(buffer);
		
		uint8_t buffer[bytes_available];
		Serial.readBytes(buffer, bytes_available);
		if (buffer[0] == 55){
			digitalWrite(13, !digitalRead(13));
		}
	}
}

