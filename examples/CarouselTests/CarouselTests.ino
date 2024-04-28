#include <MycilaEasyDisplay.h>
#include <WiFi.h>

#include <assert.h>

Mycila::EasyDisplay display;

Mycila::VirtualDisplay7x25 virtual1;
Mycila::VirtualDisplay6x21 virtual2;
Mycila::VirtualDisplay5x18 virtual3;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    continue;

  virtual1.println("1 SEC");
  virtual1.println("V1 2 vgpjy");
  virtual1.println("V1 3 vgpjy");
  virtual1.println("V1 4 vgpjy");
  virtual1.println("V1 5 vgpjy");
  virtual1.println("V1 6 vgpjy");
  virtual1.println("V1 7 vgpjy");

  virtual2.println("2 SEC");
  virtual2.println("V2 2 vgpjy");
  virtual2.println("V2 3 vgpjy");
  virtual2.println("V2 4 vgpjy");
  virtual2.println("V2 5 vgpjy");
  virtual2.println("V2 6 vgpjy");

  virtual3.println("3 SEC");
  virtual3.println("V3 2 vgpjy");
  virtual3.println("V3 3 vgpjy");
  virtual3.println("V3 4 vgpjy");
  virtual3.println("V3 5 vgpjy");

  display.home.println("HOME IS SKIPPED");

  // use default pins
  display.begin();
  display.setActive(true);

  // TEST 1
  assert(display.carousel() == Mycila::EasyDisplay::CarouselState::CAROUSEL_OFF);

  // TEST 2: empty carousel
  display.home.setDisplayTime(0);
  assert(display.carousel(display.home) == Mycila::EasyDisplay::CarouselState::CAROUSEL_ENDED);
  assert(display.carousel() == Mycila::EasyDisplay::CarouselState::CAROUSEL_OFF);

  // TEST3: infinite loop
  display.home.setNext(&virtual1);
  display.home.setDisplayTime(0);
  virtual1.setNext(&display.home);
  virtual1.setDisplayTime(0);
  assert(display.carousel(display.home) == Mycila::EasyDisplay::CarouselState::CAROUSEL_ENDED);
  assert(display.carousel() == Mycila::EasyDisplay::CarouselState::CAROUSEL_OFF);

  // TEST 4: start in middle
  display.home.setNext(&virtual1);
  virtual1.setNext(&virtual2);
  virtual2.setNext(&virtual3);
  virtual3.setNext(&display.home);
  display.home.setDisplayTime(0);
  virtual1.setDisplayTime(2000);
  virtual2.setDisplayTime(2000);
  virtual3.setDisplayTime(2000);
  assert(display.carousel(virtual2) == Mycila::EasyDisplay::CarouselState::CAROUSEL_STARTED);
  while (true) {
    if (display.carousel() == Mycila::EasyDisplay::CarouselState::CAROUSEL_NEXT && display.getCurrentDisplay() == &virtual2) {
      break;
    }
  }
}

void loop() {
}
