#include "BluetoothInterface.hpp"

bool Bluetooth::BluetoothInterface::isConnected()
{
    return BLE.connected();
}

bool Bluetooth::BluetoothInterface::isStarted()
{
    return started;
}

void Bluetooth::BluetoothInterface::start()
{
    error = BluetoothError::None;
    BTSerial.beginAndSetupBLE("AptosMonitor");
    // if (BTSerial.)
    // {
    //     error = BluetoothError::StartError;
    //     return;
    // };
    started = true;
}

void Bluetooth::BluetoothInterface::poll()
{
    BTSerial.poll();
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
    char arr[MAX_SEND_MESSAGE_LENGTH] = {};
    msg.toCharArray(arr, MAX_SEND_MESSAGE_LENGTH);
    for (int i = 0; i < MAX_SEND_MESSAGE_LENGTH; i++)
    {
        BTSerial.print(arr[i]);
    }
    BTSerial.println(' ');
    // BTSerial.println(msg.toCharArray());
}
