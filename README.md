# IHC 2018 Badge - ESP32 firmware
**NOTE** all of the following commands should be issued from the root of the project.


## Compiling for ESP32
Make sure that you have the [ESP-IDF toolchain installed](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html#get-started-get-esp-idf), specifically checkout esp-idf at commit `be81d2c16d7f4caeea9ceb29fece01510664caf3`, then
```
make -j4
make flash
```

## Compiling the emulator
Make sure that you have libsdl1.2-dev and cmake installed, then
```
mkdir emulator-build
cd emulator-build
cmake ..
make -j4
./Linux/ihc-badge-linux
```
