#ifdef DHT11
#define DHT11

class DHT11
{
	public:
		DHT11(int dht11Pin);	// Constructor

		int getTemp(void);		// Function that process the raw temperature data to the final result and return it
		int getHumi(void);		// Function that process the raw humidity data to the final result and return it
	private:
		int buffer[40];			// Buffer for the data that received from DHT11
		int dht11Pin;			// Variable that storage the pin number of DHT11

		int getData(void);		// Function that connect with the hardware,
								// and put the raw data into these two variables above
};

#endif
