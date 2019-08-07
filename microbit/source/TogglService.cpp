#include "MicroBitConfig.h"
#include "ble/UUID.h"
#include "debug.h"

#include "TogglService.h"

/**
  * Constructor.
  * Create a representation of the AccelerometerService
  * @param _ble The instance of a BLE device that we're running on.
  * @param _accelerometer An instance of MicroBitAccelerometer.
  */
TogglService::TogglService(BLEDevice &_ble, MicroBit &_uBit) :
    ble(_ble), display(_uBit.display), faceDetector(_uBit.storage, _uBit.accelerometer, _uBit.display)
{
    // Create the data structures that represent each of our characteristics in Soft Device.
    
    GattCharacteristic projectIdCharacteristic(TogglServiceDataUUID, &togglProjectId, 1, 1, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY);
    GattCharacteristic calibrateCharacteristic(TogglServiceCalibrateUUID, &faceToCalibrate, 1, 1, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE);

    // Initialise our characteristic values.
    togglProjectId=0;

    GattCharacteristic *characteristics[] = {&projectIdCharacteristic, &calibrateCharacteristic};
    GattService service(TogglServiceUUID, characteristics, sizeof(characteristics) / sizeof(GattCharacteristic *));

    ble.addService(service);

    projectIdCharacteristicHandle = projectIdCharacteristic.getValueHandle();
    calibrateCharacteristicHandle = calibrateCharacteristic.getValueHandle();

    ble.gattServer().write(projectIdCharacteristicHandle, &togglProjectId, sizeof(togglProjectId));

    ble.gattServer().onDataWritten(this, &TogglService::onDataWritten);

    display.printChar('Y');
}

void TogglService::onConnected(MicroBitEvent) {
    display.printChar('C');
    faceDetector.start();
    debug.printf("Connected\r\n");
}

void TogglService::onDisconnected(MicroBitEvent) {
    display.printChar('X');
    faceDetector.stop();
    debug.printf("disconnected\r\n");
}

/**
  * Callback. Invoked when any of our attributes are written via BLE.
  */
void TogglService::onDataWritten(const GattWriteCallbackParams *params) {
    if (params->handle == calibrateCharacteristicHandle) {
        uint8_t faceId = *(params -> data);

        debug.printf("Data written: %d\r\n", faceId);
    
        faceDetector.calibrateFace(faceId);
    }
}

const uint8_t  TogglServiceUUID[] = {
    0xe9,0x5d,0x08,0x53,0x25,0x1d,0x47,0x0a,0xa0,0x62,0xfa,0x19,0x22,0xdf,0xa9,0xa8
};

const uint8_t  TogglServiceDataUUID[] = {
    0xe9,0x5d,0xcb,0x4b,0x25,0x1d,0x47,0x0a,0xa0,0x62,0xfa,0x19,0x22,0xdf,0xa9,0xa8
};

const uint8_t  TogglServiceCalibrateUUID[] = {
    0xe9,0x5d,0xa4,0x28,0x25,0x1d,0x47,0x0a,0xa0,0x62,0xfa,0x19,0x22,0xdf,0xa9,0xa8
};
