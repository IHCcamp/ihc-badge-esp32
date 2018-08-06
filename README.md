# IHC 2018 Badge - ESP32 firmware

## Compiling for ESP32
Make sure that you have the Xtensa toolchain and ESP-IDF installed ([instructions](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html)).
The build was tested with esp-idf at commit `be81d2c16d7f4caeea9ceb29fece01510664caf3`, but it should work with master (if not, open an issue so we can look at it).

After setting up the toolchain, compile the firmware with
```
make defconfig
make -j4
```
Then, turn on your badge while holding the `*` key. The screen backlight should start flashing, this indicates that you can load the firmware on the ESP32 with
```
make flash
```
After flashing, turn the badge off and on again to start using the new firmware.

## Compiling the emulator
Make sure that you have libsdl1.2-dev and cmake installed, then from the root of the project
```
mkdir emulator-build
cd emulator-build
cmake ..
make -j4
./Linux/ihc-badge-linux
```
