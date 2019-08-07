#include "FaceDetector.hpp"
#include "debug.h"

const char* face_names[] = {"1", "2", "3", "4", "5", "6", "7", "8"};

FaceDetector::FaceDetector(MicroBitStorage &_storage, MicroBitAccelerometer &_accelerometer, MicroBitDisplay &_display) :
    storage(_storage),
    accelerometer(_accelerometer),
    display(_display)
{
    loadCalibration();
}

void FaceDetector::loadCalibration() {
    debug.printf("Loading calibration\r\n");
    for (int i=0; i<8; ++i) {
        const int *calib = face_calibration[i];
        for (int j=0; j<3; ++j) {
            faces[i][j] = calib[j];
        }
        debug.printf("Face %d: %d %d %d\r\n", i, calib[0], calib[1], calib[2]);
    }
}

void FaceDetector::start() {
    if (EventModel::defaultEventBus) {
         EventModel::defaultEventBus->listen(
             MICROBIT_ID_ACCELEROMETER,
             MICROBIT_ACCELEROMETER_EVT_DATA_UPDATE,
             this,
             &FaceDetector::refreshAccel,
             MESSAGE_BUS_LISTENER_IMMEDIATE);
    }
    debug.printf("FaceDetector started\r\n");
}

void FaceDetector::stop() {
    if (EventModel::defaultEventBus) {
         EventModel::defaultEventBus->ignore(
             MICROBIT_ID_ACCELEROMETER,
             MICROBIT_ACCELEROMETER_EVT_DATA_UPDATE,
             this,
             &FaceDetector::refreshAccel);
    }
    debug.printf("FaceDetector stopped\r\n");
}

void FaceDetector::calibrateFace(uint8_t face) {
    debug.printf("Face %s calibrated: %d %d %d\r\n", face_names[face], acc_data_buffer[0], acc_data_buffer[1], acc_data_buffer[2]);
    
    for (int i=0; i<3; ++i) {
        faces[face][i]=acc_data_buffer[i];
    }

    //int rs = storage.put("3", (uint8_t*)acc_data_buffer, 3*sizeof(int));
    //debug.printf("Storage result: %d\n", rs);
    
    display.printChar('0'+face);

    debug.printf("Current calibration\r\n");
    for (int i=0; i<8; ++i) {
        debug.printf("{%d,%d,%d},\r\n", faces[i][0], faces[i][1], faces[i][2]);
    }
    debug.printf("\r\n");
}

void FaceDetector::refreshAccel(MicroBitEvent) {
    acc_data_buffer[0]=accelerometer.getX();
    acc_data_buffer[1]=accelerometer.getY();
    acc_data_buffer[2]=accelerometer.getZ();

    uint8_t face = getFace();

    if (currentFace != face) {
        debug.printf("Changed face from %d to %d\r\n", currentFace, face);
        display.printChar('0'+face);
    }

    currentFace = face;
}

uint8_t FaceDetector::getFace() {
    int t, d, mind=1<<30;
    uint8_t face=-1;

    for (uint8_t i = 0; i<8; i++) {
        d=0;
        for (uint8_t j=0; j<3; j++) {
            t=acc_data_buffer[j]-faces[i][j];
            d+=t*t;
        }
        
        if (d<mind) {
            mind=d;
            face=i;
        }
    }

    return face;
}
