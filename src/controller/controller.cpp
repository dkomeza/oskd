#include "controller.h"
#include <EEPROM.h>

#define CONTROLLER_BAUD_RATE 9600
#define CONTROLLER_RX_PIN 16
#define CONTROLLER_TX_PIN 17

#define POWER_PIN 19

#define GEAR_ADDRESS 0x01 // EEPROM address

#define UPDATE_INTERVAL 100

#define DISPLAY_PACKET_SIZE 13
#define CONTROLLER_PACKET_SIZE 12

#define RPM_CONSTANT 0.1885

Controller controller;

/**
 * @brief Set up the controller
 * Create controller Serial and turn on the controller
 */
void Controller::setup()
{
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
void Controller::shutdown()
{
    // Send a packet to the controller so it remembers to load the legal mode
    setLegalMode(true);

    delay(50); // Wait for the controller to process the packet
    digitalWrite(POWER_PIN, LOW);
    delete serial;

    delay(50);
    gpio_hold_en((gpio_num_t)POWER_PIN);
}

/**
 * @brief Load the gear from the EEPROM
 * If the gear is out of bounds, set it to the maximum or minimum value
 */
void Controller::loadGear()
{
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
void Controller::setGear(int gear)
{
    int MAX_GEAR = this->legalMode ? 2 : 5;

    if (gear < 0 || gear > MAX_GEAR)
        return;

    this->gear = gear;

    // Save the gear to the EEPROM
    EEPROM.write(GEAR_ADDRESS, gear);
    EEPROM.commit();
}

void Controller::setWalkMode(bool walkMode)
{
    if (walkMode)
    {
        _gear = gear;
        setGear(6);
    }
    else
    {
        setGear(_gear);
    }
}

/**
 * @brief Set the legal mode
 * @param legalMode The legal mode to set
 */
void Controller::setLegalMode(bool legalMode) // TODO: Check if the function sets all the necessary variables
{
    this->legalMode = legalMode;

    maxPower = legalMode ? 250 : 1500; // TODO: Get max power from the settings
    maxSpeed = legalMode ? 25 : 50;

    loadGear();
    setGear(gear);
}

/**
 * @brief Update the controller
 * Read the serial and send the packet to the controller
 */
void Controller::update()
{
    long now = millis();

    if (now < lastUpdate)
        lastUpdate = now;

    if (now - lastUpdate < UPDATE_INTERVAL)
        return;

    lastUpdate = now;

    byte packet[DISPLAY_PACKET_SIZE] = {0};
    calculatePacket(packet);

    serial->write(packet, CONTROLLER_PACKET_SIZE);

    if (serial->available() < CONTROLLER_PACKET_SIZE)
        return;

    byte packet[CONTROLLER_PACKET_SIZE] = {0};

    byte header = serial->peek();
    while (header != 0x41 && serial->available() > 0)
    {
        serial->read();
        header = serial->peek();
    }

    if (serial->available() < CONTROLLER_PACKET_SIZE)
        return;

    serial->readBytes(packet, CONTROLLER_PACKET_SIZE);

    parsePacket(packet);
}

/**
 * @brief Calculate the packet to send to the controller
 * @return The packet to send to the controller
 */
void Controller::calculatePacket(byte *packet) // TODO: Implement this function
{
}

/**
 * @brief Parse the packet received from the controller
 * @param packet The packet received from the controller
 */
void Controller::parsePacket(byte *packet) // TODO: Implement this function
{
    byte checksum = 0;
    for (int i = 1; i < CONTROLLER_PACKET_SIZE; i++)
    {
        if (i == 6) // skip checksum byte
            continue;

        checksum ^= packet[i];
    }

    if (checksum != packet[6])
        return;

    int speed = 0;

    if (packet[3] + packet[4] > 0)
        speed = round(60000 / (packet[3] << 8 | packet[4]) * RPM_CONSTANT * 0.66); // TODO: 0.66 is a magic number (26" wheel)

    if (speed < 2)
        speed = 0;

    int power = packet[8] * 13;
    int engineTemperature = int8_t(packet[9]) + 15;
    bool brake = (packet[7] & 32) == 32;
}
