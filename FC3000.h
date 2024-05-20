#pragma once
#include <Arduino.h>
#include "BLEDevice.h"
#include <functional>

class FC3000 {
  private:
  protected:
    BLEClient* bleClient;

    BLERemoteCharacteristic* writeCharacteristic = 0;
    BLERemoteCharacteristic* readCharacteristic = 0;
    BLERemoteService* fc3000Service = 0;

    String deviceMac;
    String deviceId;//todo: get this from name in advertisement (second part of it is also in notifications)

    std::function<void(BLERemoteCharacteristic*, uint8_t*, size_t, bool)> notificationCallback;
  public:
    FC3000(BLEClient* bleClient, const String& deviceMac, const std::function<void(BLERemoteCharacteristic*, uint8_t*, size_t, bool)>& notificationCallback) : bleClient(bleClient), deviceMac(deviceMac), notificationCallback(notificationCallback) {
      deviceId = "20631|08D1F96529EC";
    }
    virtual ~FC3000() {}
    int8_t connectToDevice();
    bool isConnected() {
      return bleClient->isConnected();
    }
    void setTime(const uint32_t& unixtime);
    void setTimer(const String& startTimeHHMM, const uint16_t durationMinutes);
    void dimm(const uint8_t percent);
    void on();
    void off();
};
