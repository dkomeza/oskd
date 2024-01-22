#include "controller.h"
#include "data/data.h"

void Controller::setup()
{
    serial = new HardwareSerial(2);

    serial->begin(CONTROLLER_BAUD_RATE, SERIAL_8N1, CONTROLLER_RX_PIN, CONTROLLER_TX_PIN);
}

void Controller::update()
{
    long now = millis();

    if (now < lastUpdate)
        lastUpdate = now;

    if (now - lastUpdate < UPDATE_INTERVAL)
        return;

    lastUpdate = now;

    sendPacket();
    receivePacket();
}

void Controller::setLegalMode(bool legalMode)
{
    this->legalMode = legalMode;

    this->maxSpeed = legalMode ? 25 : 50;
    this->maxPower = legalMode ? 250 : this->_maxPower;

    sendPacket();
}

int Controller::getMaxSpeed()
{
    return this->maxSpeed;
}

int Controller::getMaxPower()
{
    return this->maxPower;
}

void Controller::sendPacket()
{
}

void Controller::receivePacket()
{
    if (serial->available() < PACKET_DOWN_SIZE)
        return;

    byte packet[PACKET_DOWN_SIZE];

    byte header = serial->peek();

    while (header != 0x41 && serial->available() > 0)
    {
        serial->read();
        header = serial->peek();
    }

    if (serial->available() < PACKET_DOWN_SIZE)
        return;

    serial->readBytes(packet, PACKET_DOWN_SIZE);

    parsePacket(packet);
}

void Controller::parsePacket(byte *packet)
{
    bool valid = isPacketValid(packet);

    if (!valid)
        return;

    if (packet[3] + packet[4] <= 0)
        data::speed = 0;
    else
        data::speed = round(60000 / (packet[3] << 8 | packet[4]) * RPM_CONSTANT * 0.66); // TODO: 0.66 is a magic number (26" wheel)

    if (data::speed < 2)
        data::speed = 0;

    data::power = packet[8] * 13; // TODO: 13 is a magic number (controller multiplier)
    data::engineTemperature = int8_t(packet[9]) + 15;
    data::brake = (packet[7] & 32) == 32;
}

bool Controller::isPacketValid(byte *packet)
{
    byte checksum = 0;
    for (int i = 1; i < PACKET_DOWN_SIZE; i++)
    {
        if (i == 6) // skip checksum byte
            continue;

        checksum ^= packet[i]; // XOR
    }

    return checksum == packet[6];
}