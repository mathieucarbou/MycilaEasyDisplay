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
