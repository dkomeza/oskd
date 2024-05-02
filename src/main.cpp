#include <Arduino.h>
#include <EEPROM.h>
#include <TFT_eSPI.h>

#include "connection/OTA.h"
#include "controller/controller.h"
#include "io/button.h"
#include "io/io.h"
#include "screen/screen.h"
#include "screen/views/dashboard.h"
#include "screen/views/settings.h"
#include "settings/settings.h"

const int BUTTON_DOWN_PIN = 14;
const int BUTTON_POWER_PIN = 27;
const int BUTTON_UP_PIN = 12;

const int EEPROM_SIZE = 64;

Button pButton = Button(BUTTON_POWER_PIN);
Button uButton = Button(BUTTON_UP_PIN);
Button dButton = Button(BUTTON_DOWN_PIN);

enum class View {
  Dashboard,
  Settings,
};

View view = View::Dashboard;
long lastActive = 0;

void startup();
void setupButtons();

void setup() {
  pinMode(BUTTON_POWER_PIN, INPUT_PULLUP);
  esp_sleep_enable_ext0_wakeup((gpio_num_t)BUTTON_POWER_PIN, 0);

  EEPROM.begin(EEPROM_SIZE);

  controller.setup();
  io.setup();
  connection::setup();
  screen::setup();

  startup();
  setupButtons();

  lastActive = millis();
}

void loop() {
  io.update();
  connection::loop();
  controller.update();
  if (view == View::Dashboard) {
    dashboard.update();

    // Convert minutes to milliseconds
    int shutdownTime = (*settings.autoSleep) * 60000;

    if (shutdownTime > 0 && lastActive + shutdownTime < millis())
      shutdown();
  }

  uButton.update();
  dButton.update();
  pButton.update();
}

void startup() {
  bool legalMode = false;
  long start = millis();

  // startup routine
  while (start + 1000 > millis()) {
    if (!pButton.isPressed())
      esp_deep_sleep_start();

    io.update();
    delay(10);
  }

  settings.setup();

  // check for settings
  if (pButton.isPressed() && dButton.isPressed()) {
    view = View::Settings;

    settingsView.draw();

    screen::lightUp();

    controller.setLegalMode(false);
    return;
  }

  dashboard.setup();

  screen::lightUp();

  start = millis();

  // legal mode routine
  while (start + 3000 > millis()) {
    if (!pButton.isPressed() || !uButton.isPressed()) {
      legalMode = true;
      break;
    }

    delay(10);
  }

  controller.setLegalMode(legalMode);

  dashboard.update();

  while (uButton.isPressed()) {
    delay(10);
  }
}

void shutdown() {
  controller.shutdown();
  screen::shutdown();

  delay(100);
  esp_deep_sleep_start();
}

void onClick(BUTTON button) {
  if (view == View::Settings) {
    settingsView.handleClick(button);
    return;
  }

  switch (button) {
    case BUTTON::Power:
      break;
    case BUTTON::Up:
      controller.setGear(controller.gear + 1);
      break;
    case BUTTON::Down:
      controller.setGear(controller.gear - 1);
      break;
  }
}

void onLongPress(BUTTON button) {
  if (view == View::Settings) {
    return;
  }

  switch (button) {
    case BUTTON::Power:
      shutdown();
      break;
    case BUTTON::Up:
      controller.setLegalMode(true);
      break;
    case BUTTON::Down:
      controller.setWalkMode(true);
      break;
  }
}

void onLongPressRepeat(BUTTON button) {
  if (view == View::Dashboard)
    return;

  settingsView.handleClick(button);
}

void onLongPressRelease(BUTTON button) {
  switch (button) {
    case BUTTON::Power:
      break;
    case BUTTON::Up:
      break;
    case BUTTON::Down:
      controller.setWalkMode(false);
      break;
  }
}

void setupButtons() {
  uButton.onClick([]() { onClick(BUTTON::Up); });
  dButton.onClick([]() { onClick(BUTTON::Down); });
  pButton.onClick([]() { onClick(BUTTON::Power); });

  uButton.onLongPress([]() { onLongPress(BUTTON::Up); });
  dButton.onLongPress([]() { onLongPress(BUTTON::Down); });
  pButton.onLongPress([]() { onLongPress(BUTTON::Power); });

  dButton.onLongPressRelease([]() { onLongPressRelease(BUTTON::Down); });

  uButton.onLongPressRepeat([]() { onLongPressRepeat(BUTTON::Up); });
  dButton.onLongPressRepeat([]() { onLongPressRepeat(BUTTON::Down); });
}