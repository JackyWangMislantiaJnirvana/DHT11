#include <math.h>
#include <DHT11.h>
#include <string.h>

#define BUS_HIGH LOW
#define BUS_LOW HIGH

DHT11::DHT11(int dht11Pin)
{
	this -> dht11Pin = dht11Pin;							// Make the pin number can be accessed wherever in the class
	memset(this->buffer, 0, sizeof(this->buffer));
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
	digitalWrite(dht11Pin, BUS_HIGH);
	delay(20);
	digitalWrite(dht11Pin, BUS_LOW);

	/* Begin to listen from DHT11 */
	pinMode(dht11Pin, INPUT);

	/* Wait for DHT11's reply */
	waitTime = micros();								// Reset the timer
	while (digitalRead(dht11Pin) == BUS_LOW)
		if (micros() - waitTime >= 1000)
		{
			delay(100);
			return -1;
		}												// Connection time out, give up the connection

	/* Wait until finishing receiving the 80us reply signal */
	waitTime = micros();								// Reset the timer
	while ( digitalRead(dht11Pin) == BUS_HIGH )
	{
		if (micros() - waitTime >= 1000)
			delay(100);
			return -2;									// Connection time out, give up the connection
	}
	
	/* Receive data, 1 bit each loop */
	for (int bitCount = 0; bitCount < 40;)
	{
		/* Wait through the 50us bit signal */
		while (digitalRead(dht11Pin) == BUS_HIGH);		// Empty block

		/* Receive and  storage the data */
			/* Get the length of the signal */
		waitTime = micros();
		while (digitalRead(dht11Pin) == BUS_LOW);			// Empty block

		signalLen = micros() - waitTime;
			/* Tell the signal means 1 or 0 */
		if (signalLen > 20 && signalLen < 50)
			this -> buffer[bitCount++] = 1;				// 1

		if (signalLen > 50 && signalLen < 80)
			this -> buffer[bitCount++] = 0;				// 0
	}

	/* Use the checksum to verify if the data received is correct */
		/* Convert checksum that received from DHT11 */
	for (int i = 0; i < 8; i ++)
		receivedChecksum += this->buffer[39-i] * pow(2, i);

		/* Calculate checksum according to the data received */
	int tempSum = 0;									// Variable for temporary usage
	for (int i = 0; i < 32; i++)
		for (int j = 7; j >= 0; j --)
			tempSum += this->buffer[i] * pow(2, j);

	caculatedChecksum = tempSum % 256;					// Cut off the number higher than 8 bit
		/* Verify */
	if	(caculatedChecksum != receivedChecksum)
		return -3;										// Data incorrect
}

int DHT11::getTemp(void)
{
	/* Declare variables */
	int temp = 0;
	int returnValue = 0;

	/* Get data from DHT11 */
	returnValue = getData();
	if (returnValue == -1 || returnValue == -2 || returnValue == -3)
		return returnValue;								// Return the error message

	/* Get the raw temperature out of the buffer and return it */
	for (int i = 0; i < 7; i++)
	{
		Temp += this -> buffer[23-i] * pow(2, i);
	}
	return temp;
}

int DHT11::getHumi(void)
{
	// To-Do: humidity data calculation
}
