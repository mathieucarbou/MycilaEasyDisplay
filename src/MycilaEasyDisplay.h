// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2024 Mathieu Carbou
 */
#pragma once

#include <Ticker.h>

#include <U8g2lib.h>

#include <mutex>
#include <vector>

#define MYCILA_EASY_DISPLAY_VERSION          "3.0.3"
#define MYCILA_EASY_DISPLAY_VERSION_MAJOR    3
#define MYCILA_EASY_DISPLAY_VERSION_MINOR    0
#define MYCILA_EASY_DISPLAY_VERSION_REVISION 3

#ifndef MYCILA_DISPLAY_LINE_LENGTH
#define MYCILA_DISPLAY_LINE_LENGTH 25
#endif

#ifndef MYCILA_DISPLAY_LINES
#define MYCILA_DISPLAY_LINES 7
#endif

#ifndef MYCILA_DISPLAY_LINE_HEIGHT_OFFSET
#define MYCILA_DISPLAY_LINE_HEIGHT_OFFSET 2
#endif

#ifndef MYCILA_DISPLAY_FONT
#define MYCILA_DISPLAY_FONT u8g2_font_5x8_tf
#endif

namespace Mycila {
  enum EasyDisplayType {
    SH1106 = 0,
    SH1107,
    SSD1306
  };

  class VirtualDisplay : public U8G2LOG {
    public:
      VirtualDisplay(const uint8_t lines = MYCILA_DISPLAY_LINES,
                     const uint8_t lineLength = MYCILA_DISPLAY_LINE_LENGTH,
                     const int8_t lineHeightOffset = MYCILA_DISPLAY_LINE_HEIGHT_OFFSET,
                     const uint8_t* font = MYCILA_DISPLAY_FONT) : _lines(lines),
                                                                  _lineLength(lineLength),
                                                                  _font(font),
                                                                  _buffer(new uint8_t[lines * lineLength]) {
        if (lines) {
          memset(_buffer, 0, _lines * _lineLength);
          begin(_lineLength, _lines, _buffer);
          setLineHeightOffset(lineHeightOffset);
        }
      }

      VirtualDisplay(const VirtualDisplay& display) : _lines(display._lines),
                                                      _lineLength(display._lineLength),
                                                      _font(display._font),
                                                      _buffer(new uint8_t[display._lines * display._lineLength]) {
        memcpy(_buffer, display._buffer, _lines * _lineLength);
        begin(_lineLength, _lines, _buffer);
        setLineHeightOffset(display.getLineHeightOffset());
      };

      VirtualDisplay(VirtualDisplay&& display) : _lines(display._lines),
                                                 _lineLength(display._lineLength),
                                                 _font(display._font),
                                                 _buffer(display._buffer) {
        display._buffer = nullptr;
        display._font = nullptr;
        setLineHeightOffset(display.getLineHeightOffset());
      }

      virtual ~VirtualDisplay() {
        delete[] _buffer;
        _buffer = nullptr;
      }

      uint8_t getLines() const { return _lines; }
      uint8_t getLineLength() const { return _lineLength; }
      int8_t getLineHeightOffset() const { return u8log.line_height_offset; }
      const uint8_t* getFont() const { return _font; }
      VirtualDisplay* getNext() const { return _next; }
      VirtualDisplay* getPrev() const { return _prev; }

      void setFont(const uint8_t* font) { _font = font; }
      void setNext(VirtualDisplay* next) {
        if (_next)
          _next->_prev = nullptr;
        _next = next;
        if (next)
          next->_prev = this;
      }
      void setPrev(VirtualDisplay* prev) {
        if (_prev)
          _prev->_next = nullptr;
        _prev = prev;
        if (prev)
          prev->_next = this;
      }

      void clear() { memset(_buffer, 0, _lines * _lineLength); }
      void setDisplayTime(uint32_t millis) { _displayTime = millis; }

    private:
      friend class EasyDisplay;

    private:
      const uint8_t _lines;
      const uint8_t _lineLength;
      const uint8_t* _font;
      uint8_t* _buffer;
      uint32_t _displayTime = 0;
      VirtualDisplay* _prev = nullptr;
      VirtualDisplay* _next = nullptr;
  };

  class VirtualDisplay7x25 : public VirtualDisplay {
    public:
      VirtualDisplay7x25() : VirtualDisplay(7, 25, 2, u8g2_font_5x8_tf) {}
  };

  class VirtualDisplay6x21 : public VirtualDisplay {
    public:
      VirtualDisplay6x21() : VirtualDisplay(6, 21, 1, u8g2_font_6x10_tf) {}
  };

  class VirtualDisplay5x21 : public VirtualDisplay {
    public:
      VirtualDisplay5x21() : VirtualDisplay(5, 21, 4, u8g2_font_6x12_tf) {}
  };

  class VirtualDisplay5x18 : public VirtualDisplay {
    public:
      VirtualDisplay5x18() : VirtualDisplay(5, 18, 1, u8g2_font_7x13_tf) {}
  };

  class VirtualDisplay5x16 : public VirtualDisplay {
    public:
      VirtualDisplay5x16() : VirtualDisplay(5, 16, 1, u8g2_font_8x13_tf) {}
  };

  class VirtualDisplay4x14 : public VirtualDisplay {
    public:
      VirtualDisplay4x14() : VirtualDisplay(4, 14, 2, u8g2_font_9x15_tf) {}
  };

  class VirtualDisplay3x13 : public VirtualDisplay {
    public:
      VirtualDisplay3x13() : VirtualDisplay(3, 13, 3, u8g2_font_10x20_tf) {}
  };

  class VirtualDisplayBlank : public VirtualDisplay {
    public:
      VirtualDisplayBlank() : VirtualDisplay(0, 0, 0, nullptr) {}
  };

  class EasyDisplay {
    public:
      enum CarouselState {
        // Carousel is disabled because EasyDisplay is not enabled or because it was not started
        CAROUSEL_OFF,
        // Carousel has started
        CAROUSEL_STARTED,
        // Carousel has switched to next display
        CAROUSEL_NEXT,
        // Carousel is still displaying the current display
        CAROUSEL_STILL,
        // Carousel end reached (blank screen)
        CAROUSEL_ENDED,
      };

    public:
      EasyDisplay(const uint8_t lines = MYCILA_DISPLAY_LINES,
                  const uint8_t lineLength = MYCILA_DISPLAY_LINE_LENGTH,
                  const int8_t lineHeightOffset = MYCILA_DISPLAY_LINE_HEIGHT_OFFSET,
                  const uint8_t* font = MYCILA_DISPLAY_FONT) : home(lines, lineLength, lineHeightOffset, font) {}

      ~EasyDisplay() { end(); }

      void begin(EasyDisplayType type = EasyDisplayType::SH1106,
                 int8_t clkPin = SCL,
                 int8_t dataPin = SDA,
                 uint16_t rotation = 0);

      void end() {
        if (!_enabled)
          return;
        std::lock_guard<std::mutex> lock(_mutex);
        _enabled = false;
        _active = false;
        _display->clear();
        delete _display;
        _dataPin = GPIO_NUM_NC;
        _clkPin = GPIO_NUM_NC;
      }

      bool isEnabled() const { return _enabled; }

      // true if on, false if off or power saving
      bool isActive() const { return _active; }
      void setActive(bool active) {
        if (!_enabled)
          return;

        std::lock_guard<std::mutex> lock(_mutex);

        if (_active && !active) {
          _powerSaveTicker.detach();
          _display->setPowerSave(true);
          _active = false;
          return;
        }

        if (active) {
          if (!_active) {
            _display->setPowerSave(false);
            _active = true;
          }
          _powerSaveTicker.detach();
          if (_powerSaveDelay > 0)
            _powerSaveTicker.once(
              _powerSaveDelay,
              +[](EasyDisplay* instance) { instance->setActive(false); },
              this);
          return;
        }
      }

      uint16_t getPowerSaveDelay() const { return _powerSaveDelay; }
      void setPowerSaveDelay(uint16_t seconds) {
        _powerSaveDelay = seconds;
        if (_powerSaveDelay == 0 || _active)
          setActive(true);
      }

      gpio_num_t getClockPin() const { return _clkPin; }
      gpio_num_t getDataPin() const { return _dataPin; }

      U8G2& u8g2() { return *_display; }

      const VirtualDisplay* getCurrentDisplay() const { return _currentDisplay; }

      void clearDisplay() {
        if (!_enabled)
          return;
        std::lock_guard<std::mutex> lock(_mutex);
        _display->clear();
        _currentDisplay = nullptr;
        _lastDisplayMillis = 0;
      }

      void display() { display(home); }
      void display(const VirtualDisplay& vDisplay) {
        if (!_enabled)
          return;
        std::lock_guard<std::mutex> lock(_mutex);
        if (vDisplay._lines) {
          _display->clearBuffer();
          _display->firstPage();
          do {
            if (vDisplay._font)
              _display->setFont(vDisplay._font);
            // const_cast because VirtualDisplay members are not to be modified
            _display->drawLog(0, _display->getMaxCharHeight(), *const_cast<VirtualDisplay*>(&vDisplay));
          } while (_display->nextPage());
        } else {
          _display->clear();
        }
        _currentDisplay = &vDisplay;
        _lastDisplayMillis = millis();
      }

      // start the carousel, with the given display as starting point
      CarouselState carousel(const VirtualDisplay& start) {
        if (!_enabled)
          return CAROUSEL_OFF;

        const VirtualDisplay* from = &start;

        // loop until we find a display with a display time
        while (from && !from->_displayTime) {
          from = from->_next;
          // prevents infinite loops
          if (from && from == &start)
            from = nullptr;
        }

        // 2 possibilities here:
        // - we found a display with a display time
        // - we did not found any display with a display time, and or next display was null (end of carousel), or we broke an infinite loop of displays without display time
        if (from) {
          display(*from);
          return CAROUSEL_STARTED;
        } else {
          clearDisplay();
          return CAROUSEL_ENDED;
        }
      }

      // continue the carousel
      CarouselState carousel() {
        if (!_enabled)
          return CAROUSEL_OFF;

        if (!_currentDisplay)
          return CAROUSEL_OFF;

        // if we are currently displaying something, we check its display time
        if (_currentDisplay && millis() - _lastDisplayMillis < _currentDisplay->_displayTime)
          return CAROUSEL_STILL;

        const VirtualDisplay* from = _currentDisplay->_next;

        // loop until we find a display with a display time
        while (from && !from->_displayTime) {
          from = from->_next;
          // prevents infinite loops
          if (from && from == _currentDisplay)
            from = nullptr;
        }

        // 2 possibilities here:
        // - we found a display with a display time
        // - we did not found any display with a display time, and or next display was null (end of carousel), or we broke an infinite loop of displays without display time
        if (from) {
          display(*from);
          return CAROUSEL_NEXT;
        } else {
          clearDisplay();
          return CAROUSEL_ENDED;
        }
      }

    public:
      VirtualDisplay home;

    private:
      static const VirtualDisplayBlank BLANK;
      EasyDisplayType _display_type = EasyDisplayType::SH1106;
      gpio_num_t _clkPin = GPIO_NUM_NC;
      gpio_num_t _dataPin = GPIO_NUM_NC;
      U8G2* _display;
      bool _enabled = false;
      volatile bool _active = false;
      volatile uint16_t _powerSaveDelay;
      Ticker _powerSaveTicker;
      const VirtualDisplay* _currentDisplay = nullptr;
      uint32_t _lastDisplayMillis = 0;
      std::mutex _mutex;
  };
} // namespace Mycila
