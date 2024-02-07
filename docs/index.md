# MycilaEasyDisplay

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Continuous Integration](https://github.com/mathieucarbou/MycilaEasyDisplay/actions/workflows/ci.yml/badge.svg)](https://github.com/mathieucarbou/MycilaEasyDisplay/actions/workflows/ci.yml)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/mathieucarbou/library/MycilaEasyDisplay.svg)](https://registry.platformio.org/libraries/mathieucarbou/MycilaEasyDisplay)

Arduino / ESP32 library for the JSY-MK-194T single-phase two-way electric energy metering module

The library is designed to offer a dead simple 6 lines of 25 characters each.

```c++
    Mycila::EasyDisplay.setPowerSaveDelay(0);
    Mycila::EasyDisplay.begin(type, clk_pin, data_pin, rotation);

    char lines[MYCILA_DISPLAY_LINE_COUNT][MYCILA_DISPLAY_LINE_LENGTH];
    ...
    Mycila::EasyDisplay.print(lines);
```
