#include "fancontroller.hpp"
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

FanController::FanController(std::vector<int> &temperatures,
                             std::vector<int> &fanSpeeds, int &interval,
                             int &hysteresis)
    : temperatures{temperatures}, fanSpeeds{fanSpeeds}, interval{interval},
      hysteresis{hysteresis}, loop{false} {}

FanController::~FanController() {
    if (loop.load(std::memory_order_acquire)) {
        stop();
    }
}

void FanController::start() {
    if (loop.load(std::memory_order_acquire)) {
        return;
    }
    ctrller = new NvidiaController();
    previousFanSpeed = ctrller->getFanSpeed();
    previousTemperature = ctrller->getTemperature();
    loop.store(true, std::memory_order_release);
    thrd = std::thread([this]() {
        while (loop.load(std::memory_order_acquire)) {
            int currentTemp = ctrller->getTemperature();
            std::cout << "Current temperature: " << currentTemp << std::endl;

            int newFanSpeed;
            if (currentTemp < temperatures.at(0)) {
                if (currentTemp < temperatures.at(0) - hysteresis) {
                    newFanSpeed = 0;
                } else {
                    newFanSpeed = previousFanSpeed;
                }
            } else {
                int tempIndex = -1;
                for (int temp : temperatures) {
                    if (currentTemp < temp) {
                        break;
                    }
                    ++tempIndex;
                }
                newFanSpeed = fanSpeeds.at(tempIndex);
            }

            if (newFanSpeed != previousFanSpeed) {
                ctrller->setFanSpeed(newFanSpeed);
                previousFanSpeed = newFanSpeed;
                previousTemperature = currentTemp;
                std::cout << "Set fan speed to " << newFanSpeed << std::endl;
            } else {
                std::cout << "Maintain fan speed of " << previousFanSpeed
                          << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
    });
}

void FanController::stop() {
    loop.store(false, std::memory_order_release);
    delete ctrller;
    if (thrd.joinable()) {
        thrd.join();
    }
}
