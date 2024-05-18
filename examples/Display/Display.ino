#include <MycilaEasyDisplay.h>

Mycila::EasyDisplay display;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    continue;

  display.begin(Mycila::EasyDisplayType::SH1106, 22, 21, 180);

  display.setPowerSaveDelay(0);
  display.setActive(true);

  char lines[MYCILA_DISPLAY_LINE_COUNT][MYCILA_DISPLAY_LINE_LENGTH];
  // fill buffer

  display.print(lines);
}

void loop() {
  delay(1000);
}
