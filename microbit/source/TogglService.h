#ifndef TOGGLSERVICE_H
#define TOGGLSERVICE_H

#include "MicroBit.h"
#include "MicroBitConfig.h"
#include "ble/BLE.h"
#include "MicroBitAccelerometer.h"
#include "EventModel.h"

#include "FaceDetector.hpp"

// TogglServis event source ID
const uint16_t TOGGL_SERVICE=0x4bc0;
const uint16_t TOGGL_CALIBRATE_FACE=0x4bc1;

// UUIDs for our service and characteristics
extern const uint8_t  TogglServiceUUID[];
extern const uint8_t  TogglServiceDataUUID[];
extern const uint8_t  TogglServiceCalibrateUUID[];

/**
  * Class definition for a MicroBit BLE Accelerometer Service.
  * Provides access to live accelerometer data via Bluetooth, and provides basic configuration options.
  */
class TogglService
{
    public:

    /**
      * Constructor.
      * Create a representation of the AccelerometerService
      * @param _ble The instance of a BLE device that we're running on.
      * @param _accelerometer An instance of MicroBitAccelerometer.
      */
  TogglService(BLEDevice &_ble, MicroBit &_uBit);

    void onConnected(MicroBitEvent e);
    void onDisconnected(MicroBitEvent e);

    private:

    /**
      * Callback. Invoked when any of our attributes are written via BLE.
      */
    void onDataWritten(const GattWriteCallbackParams *params);

    /**
     * Accelerometer update callback
     */
    void accelerometerUpdate(MicroBitEvent e);

    BLEDevice &ble;
    MicroBitDisplay &display;

    FaceDetector faceDetector;

    uint16_t accelerometerDataBuffer[3];
    uint8_t togglProjectId;
    uint8_t faceToCalibrate;

    // Handles to access each characteristic when they are held by Soft Device.
    GattAttribute::Handle_t projectIdCharacteristicHandle;
    GattAttribute::Handle_t calibrateCharacteristicHandle;

};



#endif /* TOGGLSERVICE_H */
