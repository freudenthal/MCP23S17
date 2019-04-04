#ifndef __MCP23S17
#define __MCP23S17

#include <Arduino.h>
#include <SPI.h>

class MCP23S17
{
	public:
		enum class Registers : uint8_t
		{
			IODIRA = 0, //IO direction, default all 1, 1 is input, 0 is output
			IODIRB = 1,
			IPOLA = 2, //IO polarity, default all 0, 1 is inverted, 0 is normal
			IPOLB = 3,
			GPINTENA = 4, //Interupt on change, default all 0, 1 is active need DEFVAL and INTCON
			GPINTENB = 5,
			DEFVALA = 6, //Default value for GPINTEN, if value on pin is not equal cause interrupt
			DEFVALB = 7,
			INTCONA = 8, //Interupt control, default all 0, if set to 1 use DEFVAL otherwise use previous value change
			INTCONB = 9,
			IOCONA = 10, //Configuration register, see IOCONRegisterMap
			IOCONB = 11,
			GPPUA = 12, //Pull up pin, default all 0, if 1 use 100kohm pullup on pin
			GPPUB = 13,
			INTFA = 14, //Intterupt flag register, enable via GPINTEN, a 1 indicates which pin caused interrupt, read only
			INTFB = 15,
			INTCAPA = 16, //Intterupt capture register, read only updated on interrupt, contains data when interrupt occured
			INTCAPB = 17,
			GPIOA = 18, //Reads the value from the pin, reading may not be as expected if can't drive the pin, writes are sent to the OLAT register
			GPIOB = 19,
			OLATA = 20, //Reads the OLAT setting, and write to the latch controlling the output on the pin
			OLATB = 21,
		};
		enum class IOCONRegisterMap : uint8_t
		{
			BANK = 7, //Addresses are paired if set to 1, default 0, register map herein assumes 0
			MIRROR = 6, //Mirror interrupts, default 0, if 1 then both INT pins are OR'd and activate as pair
			SEQOP = 5, //Sequential write, detfault 0, if 1 then automatically incrememnt address pointer
			DISSLW = 4, //SDA I2C slew rate, default 0, 1 enable slew rate control
			HAEN = 3, //Hardware address, default 0, if 0 then address is 0 regardless of A pins input
			ODR = 2, //Open drain interrupts, default 0, if 1 INT pin is open drain and ignores INTPOL
			INTPOL = 1, //Interrupt polarity, default 0, if 1 interrupt is active high
		};
		MCP23S17(uint8_t SelectPin);
		MCP23S17(uint8_t SelectPin, uint8_t IntAPin, uint8_t IntBPin, uint8_t Address);
		void PinMode(uint8_t Pin, uint8_t Mode);
		void DigitalWrite(uint8_t Pin, uint8_t PinStatus);
		uint8_t DigitalRead(uint8_t Pin);
		SPISettings* GetSPISettings();
	private:
		SPISettings ConnectionSettings;
		const static uint8_t BaseOpcode;
		const static uint8_t ReadWriteBit;
		const static uint8_t BankAPins;
		const static uint8_t BankBPins;
		uint8_t DeviceOpcode;
		uint8_t SelectPin;
		uint8_t IntAPin;
		uint8_t IntBPin;
		uint8_t Address;
		void WriteRegister(Registers RegisterToWrite, uint8_t RegisterContents);
		uint8_t ReadRegister(Registers RegisterToRead);
};

#endif