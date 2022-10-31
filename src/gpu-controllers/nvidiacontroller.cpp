#include "gpucontrollers.hpp"
#include <NVCtrl/NVCtrl.h>
#include <NVCtrl/NVCtrlLib.h>
#include <iostream>
#include <string>

int coolerManualControlErrorHandler(Display *, XErrorEvent *e) {
    char errorText[64];
    XGetErrorText(e->display, e->error_code, errorText, 64);
    std::cerr << "Couldn't disable cooler manual control: " << errorText
              << std::endl;
    return 0;
}

NvidiaController::NvidiaController() {
    dpy = XOpenDisplay(nullptr);
    if (!dpy) {
        throw std::runtime_error("Cannot open X display connection.");
    }
    int screen = XDefaultScreen(dpy);
    std::cout << "X Screen: " << screen << std::endl;

    int eventBase, errorBase;
    Bool ret = XNVCTRLQueryExtension(dpy, &eventBase, &errorBase);
    if (ret != True) {
        throw std::runtime_error(
            "NV-Control X extension does not exist on this X display.");
    }

    ret = XNVCTRLIsNvScreen(dpy, screen);
    std::cout << "Screen " << screen << " is NV screen " << ret << std::endl;

    ret = XNVCTRLQueryTargetCount(dpy, NV_CTRL_TARGET_TYPE_GPU, &numGpus);
    if (ret) {
        std::cout << "Number of GPUs: " << numGpus << std::endl;
    }

    ret = XNVCTRLQueryTargetCount(dpy, NV_CTRL_TARGET_TYPE_COOLER, &numCoolers);
    if (ret) {
        std::cout << "Number of coolers: " << numCoolers << std::endl;
    }
    if (!ret || numCoolers < 1) {
        throw std::runtime_error("Couldn't find any coolers.");
    }

    int numThermalSensors;
    ret = XNVCTRLQueryTargetCount(dpy, NV_CTRL_TARGET_TYPE_THERMAL_SENSOR,
                                  &numThermalSensors);
    if (ret) {
        std::cout << "Number of thermal sensors: " << numThermalSensors
                  << std::endl;
    }
    if (!ret || numThermalSensors < 1) {
        throw std::runtime_error("Couldn't find any thermal sensors.");
    }

    thermalSensorId = -1;
    for (int sensor = 0; sensor < numThermalSensors; ++sensor) {
        int attributeValue;
        ret = XNVCTRLQueryTargetAttribute(
            dpy, NV_CTRL_TARGET_TYPE_THERMAL_SENSOR, sensor, 0,
            NV_CTRL_THERMAL_SENSOR_TARGET, &attributeValue);
        if (ret) {
            std::cout << "Sensor " << sensor << " target is " << attributeValue
                      << std::endl;
            if (thermalSensorId < 0 &&
                attributeValue == NV_CTRL_THERMAL_SENSOR_TARGET_GPU) {
                std::cout << "Use readings from thermal sensor " << sensor
                          << std::endl;
                thermalSensorId = sensor;
            }
        }
    }
    if (thermalSensorId < 0 && numThermalSensors) {
        std::cout
            << "Couldn't find a sensor that monitors GPU temperature so sensor "
            << 0 << " is used instead." << std::endl;
        thermalSensorId = 0;
    }

    NVCTRLAttributeValidValuesRec validCoolerSpeedValues;
    ret = XNVCTRLQueryValidTargetAttributeValues(
        dpy, NV_CTRL_TARGET_TYPE_COOLER, 0, 0, NV_CTRL_THERMAL_COOLER_LEVEL,
        &validCoolerSpeedValues);
    if (ret) {
        std::cout << "Valid cooler level range "
                  << validCoolerSpeedValues.u.range.min << " "
                  << validCoolerSpeedValues.u.range.max << std::endl;
    }

    for (int gpu = 0; gpu < numGpus; ++gpu) {
        XNVCTRLSetTargetAttribute(dpy, NV_CTRL_TARGET_TYPE_GPU, gpu, 0,
                                  NV_CTRL_GPU_COOLER_MANUAL_CONTROL,
                                  NV_CTRL_GPU_COOLER_MANUAL_CONTROL_TRUE);
        int manualControl;
        ret = XNVCTRLQueryTargetAttribute(dpy, NV_CTRL_TARGET_TYPE_GPU, gpu, 0,
                                          NV_CTRL_GPU_COOLER_MANUAL_CONTROL,
                                          &manualControl);
        if (ret) {
            std::cout << "GPU " << gpu
                      << " cooler manual control state: " << manualControl
                      << std::endl;
        }
    }
}

NvidiaController::~NvidiaController() {
    XSetErrorHandler(coolerManualControlErrorHandler);
    for (int gpu = 0; gpu < numCoolers; ++gpu) {
        XNVCTRLSetTargetAttribute(dpy, NV_CTRL_TARGET_TYPE_GPU, gpu, 0,
                                  NV_CTRL_GPU_COOLER_MANUAL_CONTROL,
                                  NV_CTRL_GPU_COOLER_MANUAL_CONTROL_FALSE);
        int manualControl;
        Bool ret = XNVCTRLQueryTargetAttribute(
            dpy, NV_CTRL_TARGET_TYPE_GPU, gpu, 0,
            NV_CTRL_GPU_COOLER_MANUAL_CONTROL, &manualControl);
        if (ret) {
            std::cout << "GPU " << gpu
                      << " cooler manual control state: " << manualControl
                      << std::endl;
        }
    }
    XCloseDisplay(dpy);
}

int NvidiaController::getTemperature() {
    int temperature;
    Bool ret = XNVCTRLQueryTargetAttribute(
        dpy, NV_CTRL_TARGET_TYPE_THERMAL_SENSOR, thermalSensorId, 0,
        NV_CTRL_THERMAL_SENSOR_READING, &temperature);
    if (ret) {
        return temperature;
    } else {
        throw std::runtime_error(
            "Couldn't retrieve thermal reading from sensor " +
            std::to_string(thermalSensorId));
    }
}

int NvidiaController::getFanSpeed() {
    int fanSpeed;
    Bool ret =
        XNVCTRLQueryTargetAttribute(dpy, NV_CTRL_TARGET_TYPE_COOLER, 0, 0,
                                    NV_CTRL_THERMAL_COOLER_LEVEL, &fanSpeed);
    if (ret) {
        return fanSpeed;
    } else {
        throw std::runtime_error("Couldn't retrieve fan speed " +
                                 std::to_string(thermalSensorId));
    }
}

void NvidiaController::setFanSpeed(int speed) {
    for (int cooler = 0; cooler < numCoolers; ++cooler) {
        XNVCTRLSetTargetAttribute(dpy, NV_CTRL_TARGET_TYPE_COOLER, cooler, 0,
                                  NV_CTRL_THERMAL_COOLER_LEVEL, speed);
    }
}
