#pragma once
#include <Button2.h>
#include <M5Stack.h>
#include "Lcd.h"

namespace Buttons
{
    Button2 buttonA;
    Button2 buttonB;
    Button2 buttonC;

    // prototypes
    void setZone(uint8_t zone);

    void buttonAPressed_cb(Button2 &btn)
    {
        Serial.println("Button A pressed");

        M5.Power.deepSleep(0);
    }

    void buttonBPressed_cb(Button2 &btn)
    {
        Serial.println("Button B pressed");

        switch (heartRateRange.zone)
        {
        case 2:
            setZone(3);
            break;
        case 3:
            setZone(2);
            break;
        default:
            setZone(2);
            break;
        }
    }

    void buttonCPressed_cb(Button2 &btn)
    {
        Serial.println("Button C pressed");
    }

    void initialise()
    {
        buttonA.begin(BUTTON_A_PIN);
        buttonB.begin(BUTTON_B_PIN);
        buttonC.begin(BUTTON_C_PIN);

        buttonA.setPressedHandler(buttonAPressed_cb);
        buttonB.setPressedHandler(buttonBPressed_cb);
        buttonC.setPressedHandler(buttonCPressed_cb);
    }

    void setZone(uint8_t zone)
    {
        switch (zone)
        {
        case 2:
            heartRateRange.zone = zone;
            heartRateRange.bottom = HZ1_TOP - 3;
            heartRateRange.top = HZ2_TOP + 3;
            Display::updateMainMessage("Zone 2");
            Display::updateMiddleButton("Zone 3");
            break;
        case 3:
            heartRateRange.zone = zone;
            heartRateRange.bottom = HZ2_TOP - 3;
            heartRateRange.top = HZ3_TOP + 3;
            Display::updateMainMessage("Zone 3");
            Display::updateMiddleButton("Zone 2");
            break;
        }

        Serial.printf("Heartrate range set to %d -> %d \n",
                      heartRateRange.bottom, heartRateRange.top);
    }
}