SYSTEM_THREAD(ENABLED);

#include "MQTT.h"
#include "oled-wing-adafruit.h"
#include <blynk.h>

void callback(char* topic, byte* payload, unsigned int length);

MQTT client("lab.thewcl.com", 1883, callback);
OledWingAdafruit display;
String message;

BLYNK_WRITE(V1) {
  message = param.asStr();
  Serial.println(message);
  client.publish("christianity", message);
}

void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;

  if (String(p).equals("1")) {
    digitalWrite(D7, HIGH);
  } else if (String(p).equals("0")) {
    digitalWrite(D7, LOW);
  } else if (!(String(p).equals(message))) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(2);
    display.println(String(p));
    display.display();
  }
}

void setup() {
  Serial.begin(9600);
  display.setup();
  Blynk.begin("gev9UlWHupS8yCBJ2wpDXDNm6THMu0oS", IPAddress(167, 172, 234, 162), 8080);

  pinMode(D7, OUTPUT);

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(4);
  display.display();
}

void loop() {
  display.loop();
  Blynk.run();

  if (client.isConnected()) {
    client.loop();
  } else {
    client.connect(System.deviceID());
    client.subscribe("cade/led");
    client.subscribe("cade/buttons");
    client.subscribe("christianity");
  }

  if (display.pressedA()) {
    Serial.println("A");
    client.publish("cade/buttons", "A");
  }
  if (display.pressedB()) {
    client.publish("cade/buttons", "B");
  }
  if (display.pressedC()) {
    client.publish("cade/buttons", "C");
  }
}