#include <DHT11.h>
#include <Arduino.h>
#include <math.h>
#include <string.h>


DHT11::DHT11(int dht11Pin)
{
	this -> dht11Pin = dht11Pin;						// Make the pin number can be accessed wherever in the class
	memset(this->buffer, 0, sizeof(this->buffer));		// Init the buffer

	/* Give the bus a original state */
	pinMode(dht11Pin, OUTPUT);
	digitalWrite(dht11Pin, HIGH);
}


int DHT11::getData(void) 
{
	/* Prepare */
	int dht11Pin = this -> dht11Pin;
	unsigned long waitTime = 0;							// NOTE: time in MICROSECONDS
	unsigned long signalLen = 0;
	int bitCount = 0;
	int checksum = 0;
	long int receivedChecksum = 0;
	long int caculatedChecksum = 0;
	
	/* Wake up DHT11 */
	pinMode(dht11Pin, OUTPUT);
	digitalWrite(dht11Pin, LOW);
	delay(20);
	digitalWrite(dht11Pin, HIGH);
	delayMicroseconds(40);
	digitalWrite(dht11Pin, LOW);

	/* Begin to listen from DHT11 */
	pinMode(dht11Pin, INPUT);
	
	/* Wait for DHT11's reply */
	waitTime = micros();								// Reset the timer
	while (digitalRead(dht11Pin) != HIGH)
		if (micros() - waitTime >= 1000000)
		{
			delay(100);
			return -1;
		}												// Connection time out, give up the connection

	/* Wait until finishing receiving the 80us reply signal */
	waitTime = micros();								// Reset the timer
	while (digitalRead(dht11Pin) != LOW )
	{
		if (micros() - waitTime >= 1000000)
		{
			delay(100);
			return -2;									// Connection time out, give up the connection
		}
	}
	
	/* Receive data, 1 bit each loop */
	for (int bitCount = 0; bitCount < 40; bitCount++)
	{
		/* Wait through the 50us bit signal */
		while (digitalRead(dht11Pin) == LOW)
			;

		/* Receive and  storage the data */
			/* Get the length of the signal */
		waitTime = micros();
		while (digitalRead(dht11Pin) == HIGH)
			;

		signalLen = micros() - waitTime;
			/* Tell the signal means 1 or 0 */
		if (signalLen > 50)
			this -> buffer[bitCount] = 1;				// 1

		if (signalLen < 50)
			this -> buffer[bitCount] = 0;				// 0
	}
	
	for (int i = 0; i < 40; i++)
		Serial.print(this->buffer[i]);

	/* Use the checksum to verify if the data received is correct */
		/* Convert checksum that received from DHT11 */
	receivedChecksum = 0;
	for (int i = 0; i < 8; i ++)
		receivedChecksum += this->buffer[39-i] * pow(2, i);

		/* Calculate checksum according to the data received */
	int tempSum = 0;									// Variable for temporary usage
	for (int i = 0; i < 32;)
		for (int j = 7; j >= 0; j--)
			tempSum += this->buffer[i++] * pow(2, j);

	caculatedChecksum = tempSum % 256;					// Cut off the number higher than 8 bit
		/* Verify */
		
		Serial.print(caculatedChecksum);Serial.println(receivedChecksum);	//debuging message
		
	if	(caculatedChecksum != receivedChecksum)
		return -3;										// Data incorrect
	
	return 0;
}

int DHT11::getTemp(void)
{
	/* Declare variables */
	int temp = 0;
	int returnValue = 0;

	/* Call the getData() function */
	returnValue = getData();
	if (returnValue != 0)
		return returnValue;								// Return the error message

	/* Get the raw temperature out of the buffer and return it */
	for (int i = 7; i >= 0; i--)
		temp += this -> buffer[23-i] * pow(2, i);
	return temp;
}

int DHT11::getHumi(void)
{
	/* Declare variables */
	int humi = 0;
	int returnValue = 0;

	/* Call the getData() function */
	returnValue = getData();
	if (returnValue != 0)
		return returnValue;

	/* Get data from the buffer */
	for (int i = 7; i >= 0; i--)
		humi += this -> buffer[23-i] * pow(2, i);
	return humi;
}
