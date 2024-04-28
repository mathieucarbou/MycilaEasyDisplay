#include <MycilaEasyDisplay.h>
#include <WiFi.h>

Mycila::EasyDisplay display;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    continue;

  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin("IoT");

  // use default pins
  display.begin();
  display.setActive(true);
  display.setPowerSaveDelay(5);
}

void loop() {
  display.home.printf("%-6.6s %-3.3s %-8.8s %02ld:%02ld\n", "YaSolR", "Pro", "v1.2.3", random(10, 30), random(10, 30));
  display.home.printf("%-17.17s %-5.5s C\n", WiFi.localIP().toString().c_str(), "24.05");
  display.home.printf("G: %5ld W      R: %4ld W\n", random(1000, 2000), random(1000, 2000));
  display.home.printf("1\n");
  display.home.printf("2\n");
  display.home.printf("3\n");
  display.home.printf("4\n");
  display.home.printf("5\n");
  display.display();
  delay(2000);

  if (!display.isActive()) {
    display.setActive(true);
  }
}
