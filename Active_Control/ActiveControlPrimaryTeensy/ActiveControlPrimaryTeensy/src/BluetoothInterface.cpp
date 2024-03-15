#include "BluetoothInterface.hpp"

bool Bluetooth::BluetoothInterface::isConnected()
{
    return digitalRead(BT_STATE_PIN);
}

void Bluetooth::BluetoothInterface::start()
{
    error = BluetoothError::None;
    BTSerial.begin(9600);
    if (!BTSerial)
    {
        error = BluetoothError::StartError;
        return;
    };
    pinMode(BT_STATE_PIN, INPUT);
    pinMode(BT_ENABLE_PIN, OUTPUT);
    started = true;
}

bool Bluetooth::BluetoothInterface::isStarted()
{
    return started;
}

bool Bluetooth::BluetoothInterface::isMessageAvailable()
{
    error = BluetoothError::None;
    if (!isConnected())
    {
        error = BluetoothError::NotConnected;
        return false;
    }

    return BTSerial.available();
}

void Bluetooth::BluetoothInterface::readBluetoothMessage(BluetoothMessage &msg)
{
    error = BluetoothError::None;
    if (!isConnected())
    {
        error = BluetoothError::NotConnected;
        return;
    }
    msg = (BluetoothMessage)BTSerial.read();
}

void Bluetooth::BluetoothInterface::sendBluetoothMessage(String &msg)
{
    error = BluetoothError::None;
    if (!isConnected())
    {
        error = BluetoothError::NotConnected;
        return;
    }
    BTSerial.println(msg);
}
