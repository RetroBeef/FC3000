#include <CommandParser.h>
#include <BLEDevice.h>

#include <FC3000.h>

BLEClient* bleClient = 0;
FC3000* growlight = 0;

void setup() {
  Serial.begin(115200);

  BLEDevice::init("");
  bleClient = BLEDevice::createClient();

  growlight = new FC3000(bleClient, "08:d1:f9:65:29:ee", [](BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify){
    char data[length + 1];
    data[length] = '\0';
    memcpy(data, pData, length);
    char* cmdStart = strstr(data, "cmd");
    if (cmdStart != NULL) {//no status if timer disabled?
      cmdStart += 4;
      int state, dimmPercentage, dc1, dc2;
      unsigned long localTime;
      unsigned long dc3;
      sscanf(cmdStart, "%d|%d|%d|%d|%ld|%lu", &state, &dimmPercentage, &dc1, &dc2, &localTime, &dc3);
      Serial.printf("state(%u), dimmPercentage(%u), localUnixTime(%u)\r\n", state, dimmPercentage, localTime);
    }else{
      Serial.println(data);
    }
  });
  if(growlight->connectToDevice() != 0){
    Serial.println("could not connect to device");
  }else{
    Serial.println("connected");
  }
}

void loop() {
  if (Serial.available()) {
    String s = Serial.readString();
    if(s.length()){
        if(s.indexOf("on")>-1){
            Serial.println("turning on grow light");
            growlight->on();
        }else if(s.indexOf("off")>-1){
            Serial.println("turning off grow light");
            growlight->off();
        }else{
            uint8_t percent = (uint8_t)s.toInt();
            Serial.printf("dimming grow light to %u%%\r\n", percent);
            growlight->dimm(percent);
        }
    }
  }
  if (!growlight->isConnected()) {
    if(growlight->connectToDevice() != 0){
      Serial.println("could not connect to device");
    }else{
    Serial.println("connected");
  }
  }
}
