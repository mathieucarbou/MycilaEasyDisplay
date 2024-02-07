#include <MycilaEasyDisplay.h>

void setup() {
  Serial.begin(115200);
  while (!Serial)
    continue;

  Mycila::EasyDisplay.begin(Mycila::EasyDisplayType::SH1106, 22, 21, 180);

  Mycila::EasyDisplay.setPowerSaveDelay(0);
  Mycila::EasyDisplay.setActive(true);

  char lines[MYCILA_DISPLAY_LINE_COUNT][MYCILA_DISPLAY_LINE_LENGTH];
  // fill buffer

  Mycila::EasyDisplay.print(lines);
}

void loop() {
  delay(1000);
}
