#include "Logger.hpp"

void Logger::Logger::start()
{
    error = LoggerError::None;
#if LOG_TO_SERIAL
    if (!Serial)
    {
        Serial.begin(9600);
    };

#endif
    if (!SD.begin(SD_CS_PIN))
    {
        error = LoggerError::SDStartError;
#if LOG_TO_SERIAL
        // sd.initErrorPrint(&Serial);
#endif // LOG_TO_SERIAL
#if LOG_TO_BT
        if (BTInterface.isConnected())
        {
            // sd.initErrorPrint(&BT_SERIAL);
        }
#endif
        return;
    }
    if (!sdCard.init(SPI_HALF_SPEED, SD_CS_PIN))
    {
        logError("SD initialisation failed.");
    }
    logDebug(String(sdCard.type()));
    // SD_CARD_TYPE_SDHC
    bool foundFileName = false;
    String filenameStr;
    logDebug("Trying to find file name that doesn't exist yet");
    for (int i = 0; i < MAX_FILENAME_TRIES; i++)
    {
        filenameStr = "MLog" + String(i) + ".txt";
        if (SD.exists(filenameStr))
        {
            logInfo("Filename " + String(filenameStr) + " exists, trying next.");
            continue;
        }
        else
        {
            foundFileName = true;
            break;
        }
    }
    if (!foundFileName)
    {
        error = LoggerError::TooManyFilesError;
        return;
    }
    logDebug(String("Filename: ") + String(filenameStr));
    file = SD.open(filenameStr, O_WRITE | O_CREAT);
    file.flush();
    // if (!file)
    // {
    //     error = LoggerError::FileOpenError;
    //     // Serial.println("open failed\n");
    //     return;
    // }
    // if (!SD.preAllocate(LOG_FILE_SIZE))
    // {
    //     file.close();
    //     error = LoggerError::PreAllocateError;
    //     return;
    // }

    // rb.begin(&file);
}

void Logger::Logger::logData(DataPacket &data)
{
#if DEBUG_NO_SD
    return;
#endif
    error = LoggerError::None;
    // bytesUsed = rb.bytesUsed();
    // if ((bytesUsed + file.curPosition()) > (LOG_FILE_SIZE - packetSize * 2))
    // {
    //     error = LoggerError::FileFullError;
    //     return;
    // }
    file.write((const uint8_t *)&data, packetSize);
    // if (rb.getWriteError())
    // {
    //     error = LoggerError::SDBufferError;
    //     return;
    // }

    if (saveTimer > FLUSH_SD_INTERVAL)
    {
        saveTimer = 0;
        file.flush();
    }

#if LOG_PACKETS_TO_SERIAL
    Serial.println(data.toString());
#endif
}

void Logger::Logger::logInfo(String msg)
{
    logMessage(msg, LogLevel::Info);
}

void Logger::Logger::logError(String msg)
{
    logMessage(msg, LogLevel::Error);
}

void Logger::Logger::logDebug(String msg)
{
    logMessage(msg, LogLevel::Debug);
}

void Logger::Logger::logWarning(String msg)
{
    logMessage(msg, LogLevel::Warning);
}

void Logger::Logger::flush()
{
#if DEBUG_NO_SD
    return;
#endif
    file.flush();
}

void Logger::Logger::logMessage(String msg, LogLevel level)
{
#if LOG_TO_SERIAL | LOG_TO_BT
    if (level >= LOG_LEVEL)
    {
        switch (level)
        {
        case LogLevel::Debug:
            msg = "DEBUG : " + msg;
            break;
        case LogLevel::Info:
            msg = "INFO : " + msg;
            break;
        case LogLevel::Warning:
            msg = "WARNING : " + msg;
            break;
        case LogLevel::Error:
            msg = "ERROR : " + msg;
            break;
        }
#if LOG_TO_SERIAL
        Serial.println(msg);
#endif
#if LOG_TO_BT
        if (BTInterface.isStarted())
        {
            BTInterface.sendBluetoothMessage(msg);
        }

#endif
#endif // LOG_TO_SERIAL | LOG_TO_BT
    }
}

void Logger::Logger::endLog()
{
#if DEBUG_NO_SD
    return;
#endif
    file.flush();
    file.close();
}

void Logger::Logger::logMessageCode(unsigned long time, OutputMessageCode messageCode)
{
    errorPacket.elapsedTime = time;
    errorPacket.message = messageCode;
    logData(errorPacket);
#if LOG_TO_SERIAL | LOG_TO_BT
    logMessage("Message code " + String(messageCode), LogLevel::Info);
    logMessage("Message at " + String(time), LogLevel::Info);
#endif
}

bool Logger::Logger::tryWrite()
{
    error = LoggerError::None;
    // if (rb.bytesUsed() >= 512 && !file.isBusy())
    // {
    //     // Not busy only allows one sector before possible busy wait.
    //     // Write one sector from RingBuf to file.
    //     if (512 != rb.writeOut(512))
    //     {
    //         error = LoggerError::SDWriteError;
    //         return false;
    //     }
    // }
    file.flush();
    return true;
}
