#include <X11/Xlib.h>
#include <vector>

class AbstractGpuController {
  public:
    virtual int getTemperature() = 0;
    virtual int getFanSpeed() = 0;
    virtual void setFanSpeed(int) = 0;
    virtual ~AbstractGpuController() {}
};

class NvidiaController : public AbstractGpuController {
  private:
    Display *dpy;
    int thermalSensorId;
    int numCoolers;
    int numGpus;

  public:
    NvidiaController();
    ~NvidiaController();
    int getTemperature();
    int getFanSpeed();
    void setFanSpeed(int);
};
