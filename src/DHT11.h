#ifdef DHT11
#define DHT11


class DHT11
{
	Public:
		DHT11(int dht11Pin);	// Constructor

		int getTemp(void);	// Function that process the raw temperature data to the final result and return it
		int getHumi(void);	// Function that process the raw humidity data to the final result and return it 
	Private:
		int dht11Pin = 0;	// Variable that storage the pin number of DHT11

		int rawTemp = 0;	// Variable that storage the raw temperature data
		int rawHumi = 0;	// Variable that storage the raw humidity data

		int getData(void);	// Function that connect with the hardware,  
					// and put the raw data into these two variables above
};

#endif
