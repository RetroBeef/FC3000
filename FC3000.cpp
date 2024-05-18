#include "FC3000.h"

namespace {
  static BLEUUID serviceUUID("83677baa-3eb8-4866-b6b6-96e5ed5cc48d");
  static BLEUUID writeUUID("f5d2b3fe-e6b5-49b5-aa5f-a00bb4156d1d");
  static BLEUUID readUUID("6f588463-f8f1-44f8-bdae-a1272a1b0f6e");
}

int8_t FC3000::connectToDevice() {
  if (!bleClient->connect(BLEAddress(deviceMac.c_str()))) {
    return -1;
  }

  fc3000Service = bleClient->getService(serviceUUID);
  if (fc3000Service == nullptr) {
    return -2;
  }

  writeCharacteristic = fc3000Service->getCharacteristic(writeUUID);
  if (writeCharacteristic == nullptr) {
    return -3;
  }

  readCharacteristic = fc3000Service->getCharacteristic(readUUID);
  if (readCharacteristic == nullptr) {
    return -4;
  }

  if (readCharacteristic->canNotify()) {
    readCharacteristic->registerForNotify(notificationCallback);
    return 0;
  }else {
    return -5;
  }
}

void FC3000::dimm(const uint8_t percent){
    if(percent<10){
        off();
        return;
    }
    String perc = percent > 100 ? "100" : String((int)percent);
    String command = "2|0|" + deviceId + "|" + perc;
    writeCharacteristic->writeValue((uint8_t*)command.c_str(), command.length(), true);
}

void FC3000::on(){
    String command = "1|0|" + deviceId + "|1";
    writeCharacteristic->writeValue((uint8_t*)command.c_str(), command.length(), true);
}

void FC3000::off(){
    String command = "1|0|" + deviceId + "|0";
    writeCharacteristic->writeValue((uint8_t*)command.c_str(), command.length(), true);
}
