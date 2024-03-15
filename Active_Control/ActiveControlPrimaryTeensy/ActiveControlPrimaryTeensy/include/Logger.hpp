#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <SdFat.h>
#include <SD.h>
#include <RingBuf.h>
#include "DataPacket.hpp"
#include "BluetoothInterface.hpp"
#include "ValueDefinitions.hpp"

#define LOG_TO_SERIAL 1
#define LOG_PACKETS_TO_SERIAL 0
#define LOG_TO_SD 1
#define LOG_TO_BT 1
// 0: All
// 1: Exclude debug
// 2: Exclude debug and info
// 3: Exclude debug, info and warning
#define LOG_LEVEL 0

// change to allow for more entries of data packets (currently 90 000 000)
#define LOG_FILE_SIZE (sizeof(DataPacket) * 25000 * 3600)
#define LOG_FILE_SIZE (sizeof(DataPacket) * 3600)
#define RING_BUF_CAPACITY (sizeof(DataPacket))
#define SD_CONFIG SdioConfig(FIFO_SDIO)

#define DEFAULT_FILENAME "AptosLog00.log"
#define FILENAME_LENGTH 15
#define FILENAME_UNITS_POSITION 9
#define FILENAME_TENS_POSITION 8

// static const String ROOT_FILE_NAME = "AptosLog";
// static const String FILE_EXTENSION = ".log";
#define MAX_FILENAME_TRIES 99
namespace Logger

{
    enum LogLevel
    {
        Debug,
        Info,
        Warning,
        Error
    };
    enum class LoggerError
    {
        None,
        SDWriteError,
        SDStartError,
        SerialWriteError,
        SerialStartError,
        TooManyFilesError,
        PreAllocateError,
        FileOpenError,
        FileFullError,
        SDBufferError,
        BTWriteError,

    };

    class Logger
    {
    public:
        LoggerError error = LoggerError::None;
        void start();
        void logData(DataPacket &data);
        void logMessage(String msg, LogLevel level);
        void endLog();
        void logMessageCode(unsigned long time, OutputMessageCode errorCode);
        void logInfo(String msg);
        void logError(String msg);
        void logDebug(String msg);
        void logWarning(String msg);
        void flush();
#if LOG_TO_BT
        Logger(Bluetooth::BluetoothInterface &BTInterface_) : BTInterface{BTInterface_} {};
#endif
    private:
#if LOG_TO_BT
        Bluetooth::BluetoothInterface &BTInterface;
#endif
        // SdFs sd;
        // FsFile file;
        File file;
        // RingBuf<FsFile, RING_BUF_CAPACITY> rb;
        char filename[FILENAME_LENGTH] = DEFAULT_FILENAME;
        size_t maxUsed = 0;
        size_t bytesUsed;
        size_t packetSize = sizeof(DataPacket);
        bool tryWrite();
        DataPacket errorPacket;
        elapsedMillis saveTimer;
    };
}

#endif