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
