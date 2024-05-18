// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2024 Mathieu Carbou and others
 */
#include <MycilaEasyDisplay.h>

#include <assert.h>

#define TAG "EASY-DISP"

#ifndef GPIO_IS_VALID_OUTPUT_GPIO
#define GPIO_IS_VALID_OUTPUT_GPIO(gpio_num) ((gpio_num >= 0) && \
                                             (((1ULL << (gpio_num)) & SOC_GPIO_VALID_OUTPUT_GPIO_MASK) != 0))
#endif

void Mycila::EasyDisplay::begin(EasyDisplayType type, int8_t clkPin, int8_t dataPin, uint16_t rotation) {
  if (_enabled)
    return;

  if (GPIO_IS_VALID_OUTPUT_GPIO(clkPin)) {
    _clkPin = (gpio_num_t)clkPin;
  } else {
    ESP_LOGE(TAG, "Invalid Clock pin: %" PRId8, clkPin);
    _clkPin = GPIO_NUM_NC;
    return;
  }

  if (GPIO_IS_VALID_OUTPUT_GPIO(dataPin)) {
    _dataPin = (gpio_num_t)dataPin;
  } else {
    ESP_LOGE(TAG, "Invalid Data pin: %" PRId8, dataPin);
    _dataPin = GPIO_NUM_NC;
    return;
  }

  ESP_LOGI(TAG, "Enable EasyDisplay...");
  ESP_LOGD(TAG, "- Type: %u", type);
  ESP_LOGD(TAG, "- Clock Pin: %u", _clkPin);
  ESP_LOGD(TAG, "- Data Pin: %u", _dataPin);
  ESP_LOGD(TAG, "- Rotation: %" PRIu16 "°", rotation);
  ESP_LOGD(TAG, "- Power Save Delay: %" PRIu16 " seconds", _powerSaveDelay);

  switch (type) {
    case EasyDisplayType::SSD1306:
      switch (rotation) {
        case 90:
          _display = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R1, U8X8_PIN_NONE, _clkPin, _dataPin);
          break;
        case 180:
          _display = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R2, U8X8_PIN_NONE, _clkPin, _dataPin);
          break;
        case 270:
          _display = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R3, U8X8_PIN_NONE, _clkPin, _dataPin);
          break;
        default:
          _display = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE, _clkPin, _dataPin);
          break;
      }
      break;
    case EasyDisplayType::SH1106:
      switch (rotation) {
        case 90:
          _display = new U8G2_SH1106_128X64_NONAME_F_HW_I2C(U8G2_R1, U8X8_PIN_NONE, _clkPin, _dataPin);
          break;
        case 180:
          _display = new U8G2_SH1106_128X64_NONAME_F_HW_I2C(U8G2_R2, U8X8_PIN_NONE, _clkPin, _dataPin);
          break;
        case 270:
          _display = new U8G2_SH1106_128X64_NONAME_F_HW_I2C(U8G2_R3, U8X8_PIN_NONE, _clkPin, _dataPin);
          break;
        default:
          _display = new U8G2_SH1106_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE, _clkPin, _dataPin);
          break;
      }
      break;
    case EasyDisplayType::SH1107:
      switch (rotation) {
        case 90:
          _display = new U8G2_SH1107_64X128_F_HW_I2C(U8G2_R2, U8X8_PIN_NONE, _clkPin, _dataPin);
          break;
        case 180:
          _display = new U8G2_SH1107_64X128_F_HW_I2C(U8G2_R3, U8X8_PIN_NONE, _clkPin, _dataPin);
          break;
        case 270:
          _display = new U8G2_SH1107_64X128_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE, _clkPin, _dataPin);
          break;
        default:
          _display = new U8G2_SH1107_64X128_F_HW_I2C(U8G2_R1, U8X8_PIN_NONE, _clkPin, _dataPin);
          break;
      }
      break;
    default:
      assert(false);
      break;
  }

  // init table to empty strings
  for (int i = 0; i < MYCILA_DISPLAY_LINE_COUNT; i++)
    _lines[i][0] = '\0';

  _display->begin();
  _display->setFont(MYCILA_DISPLAY_FONT);
  _display->setPowerSave(true);
  _enabled = true;
}

void Mycila::EasyDisplay::end() {
  if (!_enabled)
    return;
  ESP_LOGI(TAG, "Disable EasyDisplay...");
  _enabled = false;
  _active = false;
  _display->clear();
  delete _display;
  _dataPin = GPIO_NUM_NC;
  _clkPin = GPIO_NUM_NC;
}

void Mycila::EasyDisplay::setPowerSaveDelay(uint16_t seconds) {
  _powerSaveDelay = seconds;
  if (_powerSaveDelay == 0)
    setActive(true);
}

void Mycila::EasyDisplay::setActive(bool active) {
  if (!_enabled)
    return;

  if (_active && !active) {
    ESP_LOGD(TAG, "Deactivating display...");
    _powerSaveTicker.detach();
    _display->setPowerSave(true);
    _active = false;
    return;
  }

  if (active) {
    if (!_active) {
      ESP_LOGD(TAG, "Activating display...");
      _display->setPowerSave(false);
      _active = true;
    }
    _powerSaveTicker.detach();
    if (_powerSaveDelay > 0)
      _powerSaveTicker.once(
        _powerSaveDelay, +[](EasyDisplay* instance) { instance->setActive(false); }, this);
    return;
  }
}

void Mycila::EasyDisplay::print(const char lines[MYCILA_DISPLAY_LINE_COUNT][MYCILA_DISPLAY_LINE_LENGTH]) {
  if (!_enabled)
    return;

  bool changed = false;
  for (int i = 0; i < MYCILA_DISPLAY_LINE_COUNT; i++) {
    if (strcmp(_lines[i], lines[i]) != 0) {
      snprintf(_lines[i], MYCILA_DISPLAY_LINE_LENGTH, "%s", lines[i]);
      changed = true;
    }
  }

  if (changed) {
    _display->clearBuffer();
    for (int i = 0; i < MYCILA_DISPLAY_LINE_COUNT; i++)
      _display->drawStr(1, (i + 1) * MYCILA_DISPLAY_LINE_HEIGHT + i, _lines[i]);
    _display->sendBuffer();

    setActive(true);
  }
}
