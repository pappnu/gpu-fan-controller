# gpu-fan-controller

A program for controlling the fan speed of a Nvidia GPU on linux.

## Usage

An example command:
```
gpu-fan-controller --temperatures 55,59,63 --fan_speeds 33,39,45 --interval 5000 --hysteresis 5 --verbose
```

When the GPU's temperature is the same as or greater than a temperature in the temperatures array starting from the end the fan speed at the same index in the fan speeds array is picked. Otherwise the fan speed is set to 0.

`--interval`: Determines the interval at which the fan speed is updated in milliseconds.

`--hysteresis`: Determines how much the temperature has to drop before the fan's speed may decrease. Use 0 to disable.

`--verbose` (optional): Log actions to stdout.

## Dependencies

- [XNVCtrl](https://github.com/NVIDIA/nvidia-settings/tree/main/src/libXNVCtrl)
- X11

## Build

Release:
```
cmake --preset=release
cmake --build --preset=release
```

Debug:
```
cmake --preset=debug
cmake --build --preset=debug
```

## License

MIT
