// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2024 Mathieu Carbou and others
 */
#include <MycilaEasyDisplay.h>

#ifdef MYCILA_LOGGER_SUPPORT
#include <MycilaLogger.h>
extern Mycila::Logger logger;
#define LOGD(tag, format, ...) logger.debug(tag, format, ##__VA_ARGS__)
#define LOGI(tag, format, ...) logger.info(tag, format, ##__VA_ARGS__)
#define LOGW(tag, format, ...) logger.warn(tag, format, ##__VA_ARGS__)
#define LOGE(tag, format, ...) logger.error(tag, format, ##__VA_ARGS__)
#else
#define LOGD(tag, format, ...) ESP_LOGD(tag, format, ##__VA_ARGS__)
#define LOGI(tag, format, ...) ESP_LOGI(tag, format, ##__VA_ARGS__)
#define LOGW(tag, format, ...) ESP_LOGW(tag, format, ##__VA_ARGS__)
#define LOGE(tag, format, ...) ESP_LOGE(tag, format, ##__VA_ARGS__)
#endif

#define TAG "DISPLAY"

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
    LOGE(TAG, "Invalid Clock pin: %" PRId8, clkPin);
    _clkPin = GPIO_NUM_NC;
    return;
  }

  if (GPIO_IS_VALID_OUTPUT_GPIO(dataPin)) {
    _dataPin = (gpio_num_t)dataPin;
  } else {
    LOGE(TAG, "Invalid Data pin: %" PRId8, dataPin);
    _dataPin = GPIO_NUM_NC;
    return;
  }

  LOGI(TAG, "Enable EasyDisplay on Clock pin %" PRId8 " and Data pin %" PRId8 "...", clkPin, dataPin);

  pinMode(_clkPin, OUTPUT);
  pinMode(_dataPin, OUTPUT);
  digitalWrite(_clkPin, 0);
  digitalWrite(_dataPin, 0);

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
      return;
      break;
  }

  _display->begin();
  _display->enableUTF8Print();
  _display->setPowerSave(true);
  _enabled = true;
}
