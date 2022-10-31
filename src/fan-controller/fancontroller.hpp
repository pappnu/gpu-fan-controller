#include "gpucontrollers.hpp"
#include <atomic>
#include <thread>
#include <vector>

class FanController {
  public:
    FanController(std::vector<int> &temperatures, std::vector<int> &fanSpeeds,
                  int &interval, int &hysteresis);
    ~FanController();
    void start();
    void stop();

  private:
    AbstractGpuController *ctrller;
    std::vector<int> temperatures;
    std::vector<int> fanSpeeds;
    int interval;
    int hysteresis;
    int previousTemperature = 0;
    int previousFanSpeed = 0;
    std::thread thrd;
    std::atomic<bool> loop;
};
