#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>

class Controller {
 public:
  void setup();
  void shutdown();

  void update();

  void setGear(int gear);
  void setWalkMode(bool walkMode);
  void setLegalMode(bool legalMode);

  int gear = 0;  // 0 - 5 (0 - 2 in legal mode) W(6) as the walk mode
  int speed = 0;
  int power = 0;
  int temperature = 0;
  int brake = 0;

 private:
  HardwareSerial *serial;

  bool legalMode = true;
  int maxSpeed = 25;
  int maxPower = 250;

  int _gear = 0;  // save the current gear before walk mode is activated

  long lastUpdate = 0;

  long lastActive = 0;

  void loadGear();

  void calculatePacket(byte *packet);
  void parsePacket(byte *packet);
};

extern Controller controller;