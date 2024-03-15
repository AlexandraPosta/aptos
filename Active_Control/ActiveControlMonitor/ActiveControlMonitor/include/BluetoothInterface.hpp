#ifndef BLUETOOTH_INTERFACE_H
#define BLUETOOTH_INTERFACE_H

#include <Arduino.h>
#include "HardwareBLESerial.h"
#define BT_STATE_PIN 9
#define BT_ENABLE_PIN 6
#define TERMINATOR '\r\n'
#define MAX_SEND_MESSAGE_LENGTH 40
namespace Bluetooth
{
    enum class BluetoothError
    {
        None,
        StartError,
        NotConnected,

    };

    enum BluetoothMessage : int
    {
        // numbers are specified so that BT app doesn't have to change

        // program control
        GoToIdle = 0x00,
        StartUp = 0x12,
        StartOffLaunchRodTests = 0x01,
        StartOffLaunchRodCalibration = 0x02,
        StartLaunchRodTests = 0x03,
        StartLaunchRodCalibration = 0x04,

        ArmModule = 0x05,

        TestSD = 0x13,
        TestIMU = 0x14,

    };

    class BluetoothInterface
    {
    public:
        BluetoothError error = BluetoothError::None;
        bool isMessageAvailable();
        bool isConnected();
        bool isStarted();
        void readBluetoothMessage(BluetoothMessage &msg);
        void sendBluetoothMessage(String &msg);
        void start();
        void poll();

    private:
        HardwareBLESerial &BTSerial = HardwareBLESerial::getInstance();
        bool started;
    };

}

#endif