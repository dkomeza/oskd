#include "controller.h"

#include <EEPROM.h>

#include "screen/views/dashboard.h"
#include "settings/settings.h"

// #define TESTING 1

#define CONTROLLER_BAUD_RATE 9600
#ifdef TESTING
#define CONTROLLER_RX_PIN 3
#define CONTROLLER_TX_PIN 1
#else
#define CONTROLLER_RX_PIN 16
#define CONTROLLER_TX_PIN 17
#endif

#define POWER_PIN 19

#define GEAR_ADDRESS 0x01  // EEPROM address

#define UPDATE_INTERVAL 100

#define DISPLAY_PACKET_SIZE 13
#define CONTROLLER_PACKET_SIZE 12

#define RPM_CONSTANT 0.1885

Controller controller;

/**
 * @brief Set up the controller
 * Create controller Serial and turn on the controller
 */
void Controller::setup() {
  serial = new HardwareSerial(2);
  serial->begin(CONTROLLER_BAUD_RATE, SERIAL_8N1, CONTROLLER_RX_PIN, CONTROLLER_TX_PIN);

  // Turn on the power
  gpio_hold_dis((gpio_num_t)POWER_PIN);
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH);

  // Load the gear from the EEPROM
  loadGear();
}

/**
 * @brief Shut down the controller
 * Send a packet to the controller so it remembers to load the legal mode.
 * Turn off the power
 */
void Controller::shutdown() {
  // Send a packet to the controller so it remembers to load the legal mode
  // setLegalMode(true);

  delay(50);  // Wait for the controller to process the packet
  digitalWrite(POWER_PIN, LOW);
  delete serial;

  delay(50);
  gpio_hold_en((gpio_num_t)POWER_PIN);
}

/**
 * @brief Load the gear from the EEPROM
 * If the gear is out of bounds, set it to the maximum or minimum value
 */
void Controller::loadGear() {
  gear = EEPROM.read(GEAR_ADDRESS);

  int MAX_GEAR = this->legalMode ? 2 : 5;

  if (gear > MAX_GEAR)
    gear = MAX_GEAR;

  if (gear < 0)
    gear = 0;
}

/**
 * @brief Set the gear
 * If the gear is out of bounds, do nothing
 * @param gear The gear to set
 */
void Controller::setGear(int gear) {
  int MAX_GEAR = this->legalMode ? 2 : 5;

  if (gear < 0 || gear > MAX_GEAR)
    return;

  this->gear = gear;

  // Save the gear to the EEPROM
  EEPROM.write(GEAR_ADDRESS, gear);
  EEPROM.commit();
}

void Controller::setWalkMode(bool walkMode) {
  if (walkMode) {
    _gear = gear;
    gear = 6;
  } else {
    setGear(_gear);
  }
}

/**
 * @brief Set the legal mode
 * @param legalMode The legal mode to set
 */
void Controller::setLegalMode(bool legalMode)  // TODO: Check if the function sets all the necessary variables
{
  this->legalMode = legalMode;

  maxPower = legalMode ? 250 : 1500;  // TODO: Get max power from the settings
  maxSpeed = legalMode ? 25 : 50;

  loadGear();
  setGear(gear);

  DashboardBounds bounds;
  bounds.power = maxPower;
  bounds.speed = maxSpeed;

  dashboard.setBounds(bounds);
}

/**
 * @brief Update the controller
 * Read the serial and send the packet to the controller
 */
void Controller::update() {
  long now = millis();

  if (now < lastUpdate)
    lastUpdate = now;

  if (now - lastUpdate < UPDATE_INTERVAL)
    return;

  lastUpdate = now;

  byte displayPacket[DISPLAY_PACKET_SIZE] = {0};
  calculatePacket(displayPacket);

  serial->write(displayPacket, DISPLAY_PACKET_SIZE);

  if (serial->available() < CONTROLLER_PACKET_SIZE)
    return;

  byte controllerPacket[CONTROLLER_PACKET_SIZE] = {0};

  byte header = serial->peek();
  while (header != 0x41 && serial->available() > 0) {
    serial->read();
    header = serial->peek();
  }

  if (serial->available() < CONTROLLER_PACKET_SIZE)
    return;

  serial->readBytes(controllerPacket, CONTROLLER_PACKET_SIZE);

  parsePacket(controllerPacket);
}

/**
 * @brief Calculate the packet to send to the controller
 * @return The packet to send to the controller
 */
void Controller::calculatePacket(byte *packet)  // TODO: Implement this function
{
  int powerMonitoring = settings.settingsArray[POWER_MONITORING_ADDRESS];
  int speed = legalMode ? 25 : settings.settingsArray[SPEED_LIMIT_ADDRESS];
  int wheelSize = settings.settingsArray[WHEEL_SIZE_ADDRESS];
  int pp = settings.settingsArray[MOTOR_POLE_PAIRS_ADDRESS];
  int throttleStartup = settings.settingsArray[THROTTLE_STARTUP_ADDRESS];
  int powerAssistControlMode = settings.settingsArray[POWER_ASSIST_CONTROL_MODE_ADDRESS];
  int wheelSpeedPulses = settings.settingsArray[WHEEL_SPEED_PULSES_ADDRESS];
  int pasParameterSelect = settings.settingsArray[PAS_PARAMETER_SELECT_ADDRESS];
  int motorPhaseClassification = settings.settingsArray[MOTOR_PHASE_CLASSIFICATION_ADDRESS];
  int powerAssistTuning = settings.settingsArray[POWER_ASSIST_TUNING_ADDRESS];
  int controlerMaximumCurrent = settings.settingsArray[CONTROLLER_MAXIMUM_CURRENT_ADDRESS];
  int throttleFunction = settings.settingsArray[THROTTLE_FUNCTION_ADDRESS];
  int controllerMinimumVoltage = settings.settingsArray[CONTROLLER_MINIMUM_VOLTAGE_ADDRESS];
  int throttleSpeedLimit = settings.settingsArray[THROTTLE_SPEED_LIMIT_ADDRESS];
  int regenBrake = settings.settingsArray[CONTROLLER_REGEN_BRAKE_ADDRESS];

  packet[0] = powerMonitoring;
  packet[1] = gear;
  packet[2] = (((speed - 10) & 31) << 3) | (wheelSize >> 2);
  packet[3] = pp;
  packet[4] = ((wheelSize & 3) << 6) | ((speed - 10) & 32) |
              (throttleStartup << 4) | (powerAssistControlMode << 3) | wheelSpeedPulses;
  packet[5] = 0;
  packet[6] = (pasParameterSelect << 3) | motorPhaseClassification;
  packet[7] = (powerAssistTuning << 5) | controlerMaximumCurrent | 128;
  packet[8] = (throttleFunction << 5) | controllerMinimumVoltage;
  packet[9] = 20;
  packet[10] = (regenBrake << 2) | 1;
  packet[11] = 50;
  packet[12] = 14;

  for (int i = 0; i < DISPLAY_PACKET_SIZE; i++) {
    if (i == 5)
      continue;

    packet[5] ^= packet[i];
  }

  packet[5] ^= 3;
}

/**
 * @brief Parse the packet received from the controller
 * @param packet The packet received from the controller
 */
void Controller::parsePacket(byte *packet)  // TODO: Implement this function
{
  byte checksum = 0;
  for (int i = 1; i < CONTROLLER_PACKET_SIZE; i++) {
    if (i == 6)  // skip checksum byte
      continue;

    checksum ^= packet[i];
  }

  if (checksum != packet[6])
    return;

  speed = 0;

  if (packet[3] + packet[4] > 0)
    speed = round(60000 / (packet[3] << 8 | packet[4]) * RPM_CONSTANT * 0.66);  // TODO: 0.66 is a magic number (26" wheel)

  if (speed < 2)
    speed = 0;

  power = packet[8] * 13;
  temperature = int8_t(packet[9]) + 15;
  brake = (packet[7] & 32) == 32;
}
