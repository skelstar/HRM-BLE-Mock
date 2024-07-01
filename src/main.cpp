#include <Arduino.h>
#include <elapsedMillis.h>
#include <M5Stack.h>

#include <Button2.h>

#define HZ1_TOP 128
#define HZ2_TOP 141
#define HZ3_TOP 149
#define HZ4_TOP 159

struct HeartRateRange
{
	uint8_t zone = 0;
	uint8_t bottom = 126;
	uint8_t top = 161;
} heartRateRange;

#include "Lcd.h"
#include "Buttons.h"
#include "Bluetooth.h"

void setup()
{
	Serial.begin(115200);

	M5.begin();
	M5.Power.begin();						// Init Power module
	M5.Power.setWakeupButton(BUTTON_A_PIN); // Set

	Display::initialise();

	Buttons::initialise();

	Bluetooth::initBLEServer();

	Bluetooth::bpm = 120;

	Bluetooth::direction = Bluetooth::UP;
}

elapsedMillis sinceSentBLE = 0;

void loop()
{
	M5.update();

	Buttons::buttonA.loop();
	Buttons::buttonB.loop();
	Buttons::buttonC.loop();

	if (sinceSentBLE > 1000)
	{
		sinceSentBLE = 0;

		Bluetooth::sendHr();

		// Display::updateMainMessage();
		Display::drawHeartRate(Bluetooth::lastBpm);
		// Display::updateMiddleButton();
	}

	delay(10);
}
