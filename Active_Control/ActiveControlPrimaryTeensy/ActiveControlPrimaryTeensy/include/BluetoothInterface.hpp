#ifndef BLUETOOTH_INTERFACE_H
#define BLUETOOTH_INTERFACE_H

#include <Arduino.h>
#define BT_SERIAL Serial2
#define BT_STATE_PIN 9
#define BT_ENABLE_PIN 6
#define TERMINATOR '\r\n'
namespace Bluetooth
{
    enum class BluetoothError
    {
        None,
        StartError,
        NotConnected,

    };

    enum BluetoothMessage : char
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

        // test commands
        TestZeroDeflection = 0x06,
        TestMaxDeflection = 0x07,
        TestMaxNegativeDeflection = 0x08,
        TestRapidDeflection = 0x09,

        TestWDT = 0x10,
        TestSD = 0x13,
        TestIMU = 0x14,
        TestBMP = 0x15,

        ShowPIDs = 0x24,

        // misc
        GetBatteryVoltage = 0x11,

        // servo zero offset
        X1Plus = 0x16,
        X1Sub = 0x17,
        X2Plus = 0x18,
        X2Sub = 0x19,
        Y1Plus = 0x20,
        Y1Sub = 0x21,
        Y2Plus = 0x22,
        Y2Sub = 0x23,

    };

    class BluetoothInterface
    {
    public:
        BluetoothError error = BluetoothError::None;
        bool isMessageAvailable();
        bool isConnected();
        void readBluetoothMessage(BluetoothMessage &msg);
        void sendBluetoothMessage(String &msg);
        void start();
        bool isStarted();

    private:
        HardwareSerial &BTSerial = BT_SERIAL;
        bool started = false;
    };

}

#endif