#include "pin.h"


Pin::Pin(Gpio *gpio, uint8_t pinnum)
{
	mGpio = gpio;
	mPinNum = pinnum;
	val = false;
}

Pin::Pin(Gpio *gpio, uint8_t pinnum, PinMode m)
{
	mGpio = gpio;
	mPinNum = pinnum;
	val = false;
	mode(m);
}

Pin::~Pin()
{
}

bool Pin::mode(PinMode m)
{
	mPinMode = m;
	return mGpio->pinMode(mPinNum, mPinMode);
}

bool Pin::value()
{
	return mGpio->digitalRead(mPinNum);
}

bool Pin::value(bool v)
{
	return mGpio->digitalWrite(mPinNum, v);
}

bool Pin::on()
{
	return mGpio->digitalWrite(mPinNum, true);
}

bool Pin::off()
{
	return mGpio->digitalWrite(mPinNum, false);
}
