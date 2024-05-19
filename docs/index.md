# MycilaEasyDisplay

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Continuous Integration](https://github.com/mathieucarbou/MycilaEasyDisplay/actions/workflows/ci.yml/badge.svg)](https://github.com/mathieucarbou/MycilaEasyDisplay/actions/workflows/ci.yml)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/mathieucarbou/library/MycilaEasyDisplay.svg)](https://registry.platformio.org/libraries/mathieucarbou/MycilaEasyDisplay)

Easy to use Arduino / ESP32 library for SH1106, SH1107, SSD1306 OLED I2C displays, virtual displays and carousels

## Features

- Virtual Displays
- Carousel mode: Swap between displays, each display having a configured display time
- Power save mode
- Customizable display font, lines and columns
- Default home display of 7 lines of 25 characters each, several virtual displays of 6, 5, 4, 3 lines

## Examples

### Simple display

```c++
    Mycila::EasyDisplay display;
    
    display.begin();
    display.setActive(true);

    display.home.printf("%-6.6s %-3.3s %-8.8s %02ld:%02ld\n", "YaSolR", "Pro", "v1.2.3", random(10, 30), random(10, 30));
    display.home.printf("%-17.17s %-5.5s C\n", WiFi.localIP().toString().c_str(), "24.05");
    display.home.printf("G: %5ld W      R: %4ld W\n", random(1000, 2000), random(1000, 2000));
    display.home.printf("1\n");
    display.home.printf("2\n");
    display.home.printf("3\n");
    display.home.printf("4\n");
    display.home.printf("5\n");
    
    display.display();
```

### Virtual displays

```c++
#include <MycilaEasyDisplay.h>
#include <WiFi.h>

Mycila::EasyDisplay display;

Mycila::VirtualDisplay7x25 virtual1;
Mycila::VirtualDisplay6x21 virtual2;
Mycila::VirtualDisplay5x18 virtual3;
Mycila::VirtualDisplay5x16 virtual4;
Mycila::VirtualDisplay4x14 virtual5;
Mycila::VirtualDisplay3x13 virtual6;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    continue;

  virtual1.println("1234567890123456789012345");
  virtual1.println("V1 2 vgpjy");
  virtual1.println("V1 3 vgpjy");
  virtual1.println("V1 4 vgpjy");
  virtual1.println("V1 5 vgpjy");
  virtual1.println("V1 6 vgpjy");
  virtual1.println("V1 7 vgpjy");

  virtual2.println("123456789012345678901");
  virtual2.println("V2 2 vgpjy");
  virtual2.println("V2 3 vgpjy");
  virtual2.println("V2 4 vgpjy");
  virtual2.println("V2 5 vgpjy");
  virtual2.println("V2 6 vgpjy");

  virtual3.println("123456789012345678");
  virtual3.println("V3 2 vgpjy");
  virtual3.println("V3 3 vgpjy");
  virtual3.println("V3 4 vgpjy");
  virtual3.println("V3 5 vgpjy");

  virtual4.println("1234567890123456");
  virtual4.println("V4 2 vgpjy");
  virtual4.println("V4 3 vgpjy");
  virtual4.println("V4 4 vgpjy");
  virtual4.println("V4 5 vgpjy");

  virtual5.println("12345678901234");
  virtual5.println("V5 2 vgpjy");
  virtual5.println("V5 3 vgpjy");
  virtual5.println("V5 4 vgpjy");

  virtual6.println("1234567890123");
  virtual6.println("V6 2 vgpjy");
  virtual6.println("V6 3 vgpjy");

  // use default pins
  display.begin();
  display.setActive(true);
}

void loop() {
  display.display(virtual1);
  delay(1500);

  display.display(virtual2);
  delay(1500);

  display.display(virtual3);
  delay(1500);

  display.display(virtual4);
  delay(1500);

  display.display(virtual5);
  delay(1500);

  display.display(virtual6);
  delay(1500);
}
```

### Carousel mode

```c++
#include <MycilaEasyDisplay.h>
#include <WiFi.h>

Mycila::EasyDisplay display;

Mycila::VirtualDisplay7x25 virtual1;
Mycila::VirtualDisplay6x21 virtual2;
Mycila::VirtualDisplay5x18 virtual3;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    continue;

  // carousel loop: home -> virtual1 -> virtual2 -> virtual3 -> home -> ...
  // home is skipped

  virtual1.println("1 SEC");
  virtual1.println("V1 2 vgpjy");
  virtual1.println("V1 3 vgpjy");
  virtual1.println("V1 4 vgpjy");
  virtual1.println("V1 5 vgpjy");
  virtual1.println("V1 6 vgpjy");
  virtual1.println("V1 7 vgpjy");
  virtual1.setDisplayTime(1000);
  virtual1.setPrev(&display.home);

  virtual2.println("2 SEC");
  virtual2.println("V2 2 vgpjy");
  virtual2.println("V2 3 vgpjy");
  virtual2.println("V2 4 vgpjy");
  virtual2.println("V2 5 vgpjy");
  virtual2.println("V2 6 vgpjy");
  virtual2.setDisplayTime(2000);
  virtual2.setPrev(&virtual1);

  virtual3.println("3 SEC");
  virtual3.println("V3 2 vgpjy");
  virtual3.println("V3 3 vgpjy");
  virtual3.println("V3 4 vgpjy");
  virtual3.println("V3 5 vgpjy");
  virtual3.setDisplayTime(3000);
  virtual3.setPrev(&virtual2);

  display.home.println("HOME IS SKIPPED");
  display.home.setDisplayTime(0); // will be skipped by the carousel
  display.home.setPrev(&virtual3);

  // use default pins
  display.begin();
  display.setActive(true);
  display.carousel(virtual3);
}

void loop() {
  display.carousel();
}
```

### Custom displays

```c++
Mycila::VirtualDisplay virtual(4, 10, 2, u8g2_font_8x13_tf);
```
