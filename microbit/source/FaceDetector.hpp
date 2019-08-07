#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include "MicroBit.h"
#include "MicroBitConfig.h"

const int face_calibration[8][3] = {
    {0,1,2},
    {3,4,5},
    {6,7,8},
    {9,10,11},
    {12,13,14},
    {15,16,17},
    {18,19,20},
    {21,22,23}
};

class FaceDetector {
public:

    FaceDetector(MicroBitStorage &_storage, MicroBitAccelerometer &_accelerometer, MicroBitDisplay &_display);

    void start();
    void stop();
    void loadCalibration();
    void calibrate(MicroBitDisplay &display, MicroBitMessageBus &msg_bus);
    void refreshAccel(MicroBitEvent);
    void calibrateFace(uint8_t face);
    uint8_t getFace();

private:

    MicroBitStorage &storage;
    MicroBitAccelerometer &accelerometer;

    MicroBitDisplay &display;

    uint8_t currentFace = -1;
    
    int faces[8][3];
    int acc_data_buffer[3];
};


#endif /* FACEDETECTOR_H */
