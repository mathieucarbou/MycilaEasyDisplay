// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2024 Mathieu Carbou and others
 */
#pragma once

#include <Ticker.h>
#include <U8g2lib.h>
#include <WString.h>

#define MYCILA_EASY_DISPLAY_VERSION "1.0.4"
#define MYCILA_EASY_DISPLAY_VERSION_MAJOR 1
#define MYCILA_EASY_DISPLAY_VERSION_MINOR 0
#define MYCILA_EASY_DISPLAY_VERSION_REVISION 4

#ifndef MYCILA_DISPLAY_LINE_COUNT
#define MYCILA_DISPLAY_LINE_COUNT 6
#endif

#ifndef MYCILA_DISPLAY_LINE_LENGTH
#define MYCILA_DISPLAY_LINE_LENGTH 26
#endif

#ifndef MYCILA_DISPLAY_FONT
#define MYCILA_DISPLAY_FONT u8g2_font_5x8_tr
#endif

#ifndef MYCILA_DISPLAY_LINE_HEIGHT
#define MYCILA_DISPLAY_LINE_HEIGHT 9
#endif

namespace Mycila {
  enum EasyDisplayType {
    SH1106 = 0,
    SH1107,
    SSD1306
  };

  class EasyDisplayClass {
    public:
      void begin(EasyDisplayType type, uint8_t clkPin, uint8_t dataPin, uint16_t rotation = 0);
      void end();

      void setPowerSaveDelay(uint16_t seconds);
      uint16_t getPowerSaveDelay() const { return _powerSaveDelay; }

      void setActive(bool active);

      // Up to 6 lines supported, 25 chars long
      void print(const char lines[MYCILA_DISPLAY_LINE_COUNT][MYCILA_DISPLAY_LINE_LENGTH]);

      bool isEnabled() const { return _enabled; }
      // true if on, false if off or power saving
      bool isActive() const { return _active; }

      gpio_num_t getClockPin() const { return _clkPin; }
      gpio_num_t getDataPin() const { return _dataPin; }

    private:
      bool _enabled = false;
      volatile bool _active = false;
      EasyDisplayType _display_type = EasyDisplayType::SH1106;
      U8G2* _display;
      volatile uint16_t _powerSaveDelay;
      Ticker _powerSaveTicker;
      gpio_num_t _clkPin = GPIO_NUM_NC;
      gpio_num_t _dataPin = GPIO_NUM_NC;
      char _lines[MYCILA_DISPLAY_LINE_COUNT][MYCILA_DISPLAY_LINE_LENGTH];
  };

  extern EasyDisplayClass EasyDisplay;
} // namespace Mycila
