#pragma once
#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "BLE2902.h"

#define heartRateService BLEUUID((uint16_t)0x180D)

namespace Bluetooth
{
    BLECharacteristic heartRateMeasurementCharacteristics(BLEUUID((uint16_t)0x2A37), BLECharacteristic::PROPERTY_NOTIFY);
    BLECharacteristic sensorPositionCharacteristic(BLEUUID((uint16_t)0x2A38), BLECharacteristic::PROPERTY_READ);
    BLEDescriptor heartRateDescriptor(BLEUUID((uint16_t)0x2901));
    BLEDescriptor sensorPositionDescriptor(BLEUUID((uint16_t)0x2901));

    byte flags = 0b00111110;
    byte bpm;
    byte heart[8] = {0b00001110, 60, 0, 0, 0, 0, 0, 0};
    byte hrmPos[1] = {2};

    enum Direction
    {
        UP,
        DOWN,
    } direction;

    bool _BLEClientConnected = false;

    class MyServerCallbacks : public BLEServerCallbacks
    {
        void onConnect(BLEServer *pServer)
        {
            Serial.println("Something has connected: onConnect");
            _BLEClientConnected = true;
        };

        void onDisconnect(BLEServer *pServer)
        {
            Serial.println("Something has disconnected: onDisconnect");
            _BLEClientConnected = false;
        }
    };

    void initBLEServer()
    {
        BLEDevice::init("HRM-BLE-Mock");
        // Create the BLE Server
        BLEServer *pServer = BLEDevice::createServer();
        pServer->setCallbacks(new MyServerCallbacks());

        // Create the BLE Service
        BLEService *pHeart = pServer->createService(heartRateService);

        pHeart->addCharacteristic(&heartRateMeasurementCharacteristics);
        heartRateDescriptor.setValue("Rate from 0 to 200");
        heartRateMeasurementCharacteristics.addDescriptor(&heartRateDescriptor);
        heartRateMeasurementCharacteristics.addDescriptor(new BLE2902());

        pHeart->addCharacteristic(&sensorPositionCharacteristic);
        sensorPositionDescriptor.setValue("Position 0 - 6");
        sensorPositionCharacteristic.addDescriptor(&sensorPositionDescriptor);

        pServer->getAdvertising()->addServiceUUID(heartRateService);

        pHeart->start();
        // Start advertising
        pServer->getAdvertising()->start();
    }

    void sendHr()
    {
        heart[1] = (byte)bpm;
        int energyUsed = 3000;
        heart[3] = energyUsed / 256;
        heart[2] = energyUsed - (heart[2] * 256);
        Serial.println(bpm);

        Bluetooth::heartRateMeasurementCharacteristics.setValue(heart, 8);
        Bluetooth::heartRateMeasurementCharacteristics.notify();

        Bluetooth::sensorPositionCharacteristic.setValue(hrmPos, 1);

        if (direction == UP)
        {
            if (bpm > heartRateRange.top)
            {
                bpm = heartRateRange.top - 1;
                direction = DOWN;
            }
            bpm++;
        }
        else if (direction == DOWN)
        {
            if (bpm < heartRateRange.bottom)
            {
                bpm = heartRateRange.bottom + 1;
                direction = UP;
            }
            bpm--;
        }
    }
}