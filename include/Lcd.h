#pragma once

#include <Arduino.h>
#include <M5Stack.h>

#include "Bluetooth.h"

namespace Display
{
#define LCD_WIDTH 340
#define LCD_HEIGHT 240
#define TEXT_SIZE_M 2
#define TEXT_SIZE_LABEL 1
#define FONT_NUM 4

    enum ButtonNumber
    {
        LEFT_BUTTON = 1,
        MIDDLE_BUTTON,
        RIGHT_BUTTON,
    };

    // prototypes
    void updateMainMessage(const char *message);
    void drawButtonLabel(ButtonNumber button, const char *label);
    void drawHeartRate(byte heartrate);
    // uint8_t messageLength(const char *message);

    void initialise()
    {
        M5.Lcd.begin();
        M5.Lcd.fillScreen(RED);
        updateMainMessage("ALL Zones");
        drawHeartRate(Bluetooth::bpm);

        // drawButtonLabel(1, "");
        drawButtonLabel(ButtonNumber::MIDDLE_BUTTON, "Zone 2");
        drawButtonLabel(ButtonNumber::RIGHT_BUTTON, "X");
    }

    void updateMiddleButton(const char *label)
    {
        drawButtonLabel(ButtonNumber::MIDDLE_BUTTON, label);
    }

    void updateMainMessage(const char *message)
    {
        M5.Lcd.clearDisplay(RED);
        M5.Lcd.setTextSize(TEXT_SIZE_M);
        M5.Lcd.setTextDatum(CC_DATUM);
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.drawString(message, LCD_WIDTH / 2, (LCD_HEIGHT / 2) - 30, FONT_NUM);
    }

    void drawHeartRate(byte heartrate)
    {
        M5.Lcd.fillRect(0, (LCD_HEIGHT / 2), LCD_WIDTH, 30, BLACK);

        char buffer[7];
        uint8_t length = sprintf(buffer, "HR %d", heartrate);
        // printf("drawHeartRate: %s \n", buffer);
        M5.Lcd.setTextSize(TEXT_SIZE_LABEL);
        M5.Lcd.setTextDatum(CC_DATUM);
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.drawString(buffer, LCD_WIDTH / 2, (LCD_HEIGHT / 2) + 15, FONT_NUM);
    }

    void drawButtonLabel(ButtonNumber button, const char *label)
    {
        uint8_t bcDatumX = 0;
        uint8_t triangleSize = 20;
        switch (button)
        {
        case ButtonNumber::LEFT_BUTTON:
            bcDatumX = 0 + triangleSize + 5;
            break;
        case ButtonNumber::MIDDLE_BUTTON:
            bcDatumX = LCD_WIDTH / 2;
            break;
        case ButtonNumber::RIGHT_BUTTON:
            bcDatumX = LCD_WIDTH - triangleSize + 5;
            break;
        }

        M5.Lcd.fillTriangle(bcDatumX, LCD_HEIGHT,
                            bcDatumX - triangleSize, LCD_HEIGHT - triangleSize,
                            bcDatumX + triangleSize, LCD_HEIGHT - triangleSize,
                            WHITE);

        M5.Lcd.setTextSize(TEXT_SIZE_LABEL);
        M5.Lcd.setTextDatum(BC_DATUM);
        M5.Lcd.drawString(label, bcDatumX, LCD_HEIGHT - triangleSize - 5, FONT_NUM);
    }

    // uint8_t messageLength(const char *message)
    // {
    //     uint8_t length = 0;
    //     while (message[length] != '\0')
    //         length++;
    //     return length;
    // }
}