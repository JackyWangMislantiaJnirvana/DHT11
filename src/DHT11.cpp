#include <DHT11.h>

#define BUS_HIGH LOW
#define BUS_LOW HIGH

DHT11::DHT11(int dhtPin) 
{
	this -> dhtPin = dhtPin;	// Make the pin number can be accessed wherever in the class
}


int DHT11::getData(void) 
{
	/* Prepare */
	int dhtPin = this -> dhtPin;
	byte buffer[40] = {};		// Buffer for the data that recieved from DHT11
	int waitTime = 0;		// NOTE: time in MICROSECONDS

	/* Wake up DHT11 */
	pinMode(dhtPin, OUTPUT);
	digitalWrite(dhtPin, BUS_HIGH);
	delay(18);
	digitalWrite(dhtPin, BUS_LOW);

	/* Wait for DHT11's reply */
	pinMode(dhtPin, INPUT);
	waitTime = 0;			// Reset the timer
	while ( digitalRead(dhtPin) == BUS_LOW )
	{
		waitTime ++;		// Increase the timer

		if waitTime >= 1000
			return -1;	// Connection time out
	}
	
	/* Recieve the 80us confirn signal */
	while ()
	



}

int DHT11::getTemp(void) {}

int DHT11::getHumi(void) {}
