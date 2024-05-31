#include "esphome.h"

byte TempHi;              // Variable hold data high byte
byte TempLo;              // Variable hold data low byte
signed int P_N;              // Bit flag for Positive and Negative
unsigned short rawtemp;
//void Cal_Temp();
const int I2C_address = 0x18;  // I2C write address

//Have this locally for your file:
static const char *TAG = "This code area name";

class GDK101: public PollingComponent, public Sensor {
 public:
  // constructor
  GDK101() : PollingComponent(60000) {} // 60s 
  
	int addr = 0x18;
	int day,hour,min,sec = 0;
	byte buffer[2] = {0,0};
	int status = 0;
  
  void Gamma_Mod_Read(int cmd)
  {
	float value = 0.0f;
	  
 /* Begin Write Sequence */
	Wire.beginTransmission(I2C_address);
	Wire.write(cmd);
	Wire.endTransmission();
	/* End Write Sequence */
	delay(10);
	/* Begin Read Sequence */
	Wire.requestFrom(I2C_address, 2);
	byte i = 0;
	while(Wire.available())
	{
	buffer[i] = Wire.read();
	i++;
	}
	/* End Read Sequence */

	value = buffer[0] + (float)buffer[1]/100;
	
	ESP_LOGD(TAG, "Value of my sensor: ");
	ESP_LOGD(TAG, "%f", value);
	ESP_LOGD(TAG, "uSv/h");
	
	if(0xB2 ==cmd) // 10 min avg
	{
		publish_state(float(value));
	}
	
  } 
  
  
  void setup() override 
  {
	//ESP_LOGI("GDK101 setup executed");
	ESP_LOGD(TAG, "GDK101 -  start setup()");
	 
    // This will be called by App.setup()
    Wire.begin();
    delay(1000);
	
	//Read Firmware Version
	Gamma_Mod_Read(0xB4);
	//Reset before operating the sensor
	Gamma_Mod_Read(0xA0);
	
	/*
    Wire.beginTransmission(I2C_address);
    Wire.write(1);             // Setup configuration register
    Wire.write(0x60);          // 12-bit
    Wire.endTransmission();
    Wire.beginTransmission(I2C_address);
    Wire.write(0);             // Setup Pointer Register to 0
    Wire.endTransmission();
	*/
	ESP_LOGD(TAG, "GDK101 -  end setup()");
  }
  
  void update() override 
  {
	 
	ESP_LOGI(TAG, "GDK101 - update()");
	Gamma_Mod_Read(0xB3);
	  /*
    // This will be called every "update_interval" milliseconds.
    Wire.requestFrom(I2C_address, 2);
    TempHi = Wire.read();       // Read temperature high byte
    TempLo = Wire.read();       // Read temperature low byte
    //Cal_Temp ();
    // Display temperature 
    publish_state(float(rawtemp)*float(0.0625)*float(P_N));
	
	ESP_LOGI(TAG, "Value1:");
	ESP_LOGI(TAG, "Value of my sensor: %i", TempHi);
	ESP_LOGI(TAG, "Value2:");
	ESP_LOGI(TAG, "Value of my sensor: %i", TempLo);
	*/
  }
  
  


  
  
};
/*
void Cal_Temp()
{
  if (TempHi&0x80)          // If bit7 of the TempHi is HIGH then the temperature is negative
    P_N = -1;
  else                      // Else the temperature is positive
    P_N = 1;

  TempHi = TempHi & 0x7F;   // Remove sign
  TempLo = TempLo & 0xF0;   // Filter out last nibble
  rawtemp = TempHi << 4;
   TempLo = TempLo >> 4;      // Shift right 4 times
   rawtemp = rawtemp | TempLo;
}*/