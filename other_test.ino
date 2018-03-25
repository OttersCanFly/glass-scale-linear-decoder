/*
 Name:		other_test.ino
 Created:	3/24/2018 10:48:41 AM
 Author:	Syx
*/

#include <SPI.h>

String s = "";


char axis[28];
double hld_x = 9;
double hld_y = 9;
double hld_z = 9;

int chipSelectPin1 = 10;
int chipSelectPin2 = 9;
int chipSelectPin3 = 8;




void setup()
{
	Serial.begin(115200);
	

	//pinMode(chipSelectPin1, OUTPUT);
	//pinMode(chipSelectPin2, OUTPUT);
	

	LS7366_Init();

	Serial.flush();

	while (!Serial) {
		;
	}

	delay(100);
}


void loop()
{
	long encoder1Value;
	long encoder2Value;
	long encoder3Value;

	

	
	if (Serial.available())
	{
		s = Serial.readStringUntil('\r');

		
	}

	if (s == "Now" || s == "")
	{

		encoder1Value = getEncoderValue(1);
		encoder2Value = getEncoderValue(2);
		encoder3Value = getEncoderValue(3);

		if (hld_x != encoder1Value || hld_y != encoder2Value || encoder3Value != hld_z)
		{
			hld_x = encoder1Value;
			hld_y = encoder2Value;
			hld_z = encoder3Value;
			sprintf(axis, "%08d^%08d^%08d^", encoder1Value, encoder2Value, 0);

			Serial.println(axis);
		}
	}
}



 //*****************************************************  
long getEncoderValue(int encoder)
//*****************************************************
{
	unsigned int count1Value, count2Value, count3Value, count4Value;
	long result;

	selectEncoder(encoder);

	SPI.transfer(0x60); // Request count
	count1Value = SPI.transfer(0x00); // Read highest order byte
	count2Value = SPI.transfer(0x00);
	count3Value = SPI.transfer(0x00);
	count4Value = SPI.transfer(0x00); // Read lowest order byte

	deselectEncoder(encoder);

	result = ((long)count1Value << 24) + ((long)count2Value << 16) + ((long)count3Value << 8) + (long)count4Value;

	return result;
}//end func

 //*************************************************
void selectEncoder(int encoder)
//*************************************************
{
	switch (encoder)
	{
	case 1:
		bitClear(PORTB, 2);
		break;
	case 2:
		bitClear(PORTB, 1);
		break;
	case 3:
		bitClear(PORTB, 0);
		break;
	}//end switch

}//end func

 //*************************************************
void deselectEncoder(int encoder)
//*************************************************
{
	switch (encoder)
	{
	case 1:
		bitSet(PORTB, 2);
		break;
	case 2:
		bitSet(PORTB, 1);
		break;
	case 3:
		bitSet(PORTB, 0);
		break;
	}//end switch

}//end func



 // LS7366 Initialization and configuration
 //*************************************************
void LS7366_Init(void)
//*************************************************
{
	// SPI initialization
	SPI.begin();
	
	bitClear(PORTB, 2);
	SPI.transfer(0x88);
	SPI.transfer(0x03);
	bitSet(PORTB, 2);

	bitClear(PORTB, 1);
	SPI.transfer(0x88);
	SPI.transfer(0x03);
	bitSet(PORTB, 1);

	bitClear(PORTB, 0);
	SPI.transfer(0x88);
	SPI.transfer(0x03);
	bitSet(PORTB, 0);

}//end func


