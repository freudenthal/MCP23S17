#include <MCP23S17.h>

static uint8_t BaseOpcode = 64;
static uint8_t ReadWriteBit = 0;
static uint8_t BankAPins = 7;
static uint8_t BankBPins = 15;

MCP23S17::MCP23S17(uint8_t _SelectPin) : MCP23S17(_SelectPin, 255, 255, 0)
{

}

MCP23S17::MCP23S17(uint8_t _SelectPin, uint8_t _IntAPin, uint8_t _IntBPin, uint8_t _Address)
{
	SelectPin = _SelectPin;
	IntAPin = _IntAPin;
	IntBPin = _IntBPin;
	Address = _Address;
	pinMode(SelectPin, OUTPUT);
	digitalWrite(SelectPin, HIGH);
	if (IntAPin != 255)
	{
		pinMode(IntAPin, INPUT);
	}
	if (IntBPin != 255)
	{
		pinMode(IntBPin, INPUT);
	}
	ConnectionSettings = SPISettings(10000000, MSBFIRST, SPI_MODE0);
	DeviceOpcode = BaseOpcode | Address;
}

void MCP23S17::pinMode(uint8_t Pin, uint8_t Mode)
{
	//Mode 0 is input, 1 is output, 2 is pullup
	Mode = constrain(Mode,0,2);
	Pin = constrain(Pin,0,BankBPins);
	bool SetAsInput = (Mode != 1);
	bool EnablePullup = (Mode == 2);
	Registers RegisterToWrite = (Pin <= BankAPins) ? Registers::IODIRA : Registers::IODIRB;
	uint8_t RegisterContents = ReadRegister(RegisterToWrite);
	uint8_t BitToChange = (Pin <= BankAPins) ? Pin : (Pin - 8);
	bitWrite(RegisterContents, BitToChange, SetAsInput);
	WriteRegister(RegisterToWrite, RegisterContents);
	if (EnablePullup)
	{
		RegisterToWrite = (Pin <= BankAPins) ? Registers::GPPUA : Registers::GPPUB;
		RegisterContents = ReadRegister(RegisterToWrite);
		bitWrite(RegisterContents, BitToChange, EnablePullup);
		WriteRegister(RegisterToWrite, RegisterContents);
	}
}

void MCP23S17::digitalWrite(uint8_t Pin, uint8_t PinStatus)
{
	Registers RegisterToWrite = (Pin <= BankAPins) ? Registers::OLATA : Registers::OLATB;
	uint8_t RegisterContents = ReadRegister(RegisterToWrite);
	uint8_t BitToChange = (Pin <= BankAPins) ? Pin : (Pin - 8);
	bitWrite(RegisterContents, BitToChange, PinStatus);
	WriteRegister(RegisterToWrite, RegisterContents);
}

uint8_t MCP23S17::digitalRead(uint8_t Pin)
{
	Registers RegisterToRead = (Pin <= BankAPins) ? Registers::OLATA : Registers::OLATB;
	uint8_t BitToRead = (Pin <= BankAPins) ? Pin : (Pin - 8);
	uint8_t RegisterContents = ReadRegister(RegisterToRead);
	return bitRead(RegisterContents, BitToRead);
}

void WriteRegister(uint8_t RegisterToWrite, uint8_t RegisterContents)
{
	uint8_t OpCode = DeviceOpcode;
	bitWrite(OpCode,0,false);
	SPI.beginTransaction(ConnectionSettings);
	SPI.transfer( 0 );
	digitalWrite(SelectPin,LOW);
	SPI.transfer( OpCode );
	SPI.transfer( RegisterToWrite );
	SPI.transfer( RegisterContents );
	digitalWrite(SelectPin, HIGH);
	SPI.endTransaction();
}

uint8_t ReadRegister(uint8_t RegisterToRead)
{
	uint8_t OpCode = DeviceOpcode;
	bitWrite(OpCode,0,true);
	SPI.beginTransaction(ConnectionSettings);
	SPI.transfer( 0 );
	digitalWrite(SelectPin,LOW);
	SPI.transfer( OpCode );
	SPI.transfer( RegisterToRead );
	uint8_t RegisterContents = SPI.transfer( 0 );
	digitalWrite(SelectPin, HIGH);
	SPI.endTransaction();
	return RegisterContents;
}

SPISettings* MCP23S17::GetSPISettings()
{
	return &ConnectionSettings;
}