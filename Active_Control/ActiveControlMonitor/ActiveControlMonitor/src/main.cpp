#include <Arduino.h>
#include <elapsedMillis.h>
#include "StateMachine.hpp"
#include "SensorInterface.hpp"

#include "ValueDefinitions.hpp"
#include "PrimaryComm.hpp"

#include "BluetoothInterface.hpp"
#include "BuzzerInterface.hpp"
#include "LEDInterface.hpp"
#include "Logger.hpp"
#include "DataPacket.hpp"
#include "OutputMessageCodes.hpp"

#include "GroundTests.hpp"
#include "LaunchRodTests.hpp"
#include "Watchdog.hpp"

void startUp();
void offLaunchRodTest();
void offLaunchRodCalibration();
void idle();
void launchRodCalibration();
void launchRodTest();
void arm();
void armed();
void activeControlEnabled();

FunctionArray stateFunctions[NUM_STATES] = {
    startUp,
    offLaunchRodTest,
    offLaunchRodCalibration,
    idle,
    launchRodCalibration,
    launchRodTest,
    arm,
    armed,
    activeControlEnabled};
StateMachine::StateMachine stateMachine{stateFunctions};

PrimaryComm::PrimaryComm primaryComm;
Bluetooth::BluetoothInterface BTInterface;
Logger::Logger logger{BTInterface};
SensorInterface::SensorInterface sensorInterface{logger};
Bluetooth::BluetoothMessage btMsg;
bool btMsgAvailable;
Buzzer::BuzzerInterface buzzerInterface;
LED::LEDInterface ledInterface;
#if LOG_TO_BT

#else
Logger::Logger logger;
#endif
DataPacket packet;
bool packetUpdated;

Watchdog wdt;
bool wdtStarted = false;

int serialAvailable;

struct Timers
{
  elapsedMillis totalElapsedTime;
  elapsedMillis armTime;
  elapsedMicros launchElapsedTime;
  elapsedMicros dt;
  unsigned long fixedDt;
};

Timers timers;

void haltError(OutputMessageCode errorMessageCode)
{
  primaryComm.endNoFailureSignal();
  primaryComm.endWatchdogSignal();
  logger.logMessageCode(timers.totalElapsedTime, errorMessageCode);
  logger.endLog();
  buzzerInterface.startProgram(Buzzer::BuzzerProgram::ContinuousBeep);
  ledInterface.startProgram(LED::LEDProgram::ContinuousBlink);
  while (true)
  {
    // stop a reset
    if (wdtStarted)
    {
      wdt.feed();
    }

    buzzerInterface.update();
    ledInterface.update();
  };
}

void startAllPeripherals()
{
  logger.start();
  switch (logger.error)
  {
  case Logger::LoggerError::None:
    break;
  case Logger::LoggerError::FileOpenError:

    logger.logError("Could not open file.");
#if !DEBUG_NO_SD
    haltError(OutputMessageCode::LoggerError);
#endif
    break;
  case Logger::LoggerError::FileFullError:
    logger.logError("File full!");

    haltError(OutputMessageCode::LoggerError);

    break;
  case Logger::LoggerError::PreAllocateError:
    logger.logError("Could not preallocate file.");
    haltError(OutputMessageCode::LoggerError);
    break;
  case Logger::LoggerError::TooManyFilesError:
    logger.logError("Too many files!");
    haltError(OutputMessageCode::LoggerError);
    break;
  default:
    logger.logError("Other logger error...");
    haltError(OutputMessageCode::LoggerError);
    break;
  }

  // start buzzer and led
  buzzerInterface.start(Buzzer::BuzzerProgram::Off, NOTE_E4);
  ledInterface.start(LED::LEDProgram::On);

  // play start up song
  buzzerInterface.startProgramDelay(Buzzer::BuzzerProgram::Startup);

  // start bluetooth
  logger.logMessage("Starting Bluetooth interface...", Logger::LogLevel::Info);
  // BTInterface.start();
  // if (BTInterface.error == Bluetooth::BluetoothError::StartError)
  // {
  //   logger.logMessage("Error starting Bluetooth interface.", Logger::LogLevel::Error);
  //   haltError(OutputMessageCode::BTStartUpError);
  // }
  // BTInterface.poll();
  logger.logInfo("Started Bluetooth interface");

  // start sensors
  logger.logInfo("Starting Sensor interface...");
  sensorInterface.start();
  switch (sensorInterface.error)
  {
  case (SensorInterface::SensorInterfaceError::IMUStartUpError):
    logger.logError("Error starting IMU.");
    haltError(OutputMessageCode::IMUStartUpError);
    break;

  case (SensorInterface::SensorInterfaceError::BarometerStartUpError):
    logger.logError("Error starting Barometer.");
    haltError(OutputMessageCode::BarometerStartUpError);
    break;

  case (SensorInterface::SensorInterfaceError::None):
    break;

  default:
    logger.logError("Unknown error starting IMU.");
    haltError(OutputMessageCode::UnknownError);
    break;
  }
  logger.logInfo("Started Sensor interface");

  logger.logInfo("Starting monitor comm...");
  primaryComm.start();
  logger.logInfo("Started monitor comm.");

  logger.logInfo("Start up complete.");
  logger.logInfo("Changing state to idle...");
  logger.logMessageCode(timers.totalElapsedTime, OutputMessageCode::StateChangedToIdle);
  logger.flush();
  stateMachine.changeState(StateMachine::State::Idle);
}

void bluetoothFuncHeader()
{
  BTInterface.poll();
  btMsgAvailable = BTInterface.isMessageAvailable();
  switch (BTInterface.error)
  {
  case Bluetooth::BluetoothError::NotConnected:
    logger.logError("Bluetooth not connected. Waiting 5 seconds.");
    delay(5000);
    break;
  case Bluetooth::BluetoothError::None:
    break;
  default:
    logger.logError("Error recieving bluetooth message!");
    // logger.logMessageCode(timers.totalElapsedTime, OutputMessageCode::BTConnectionError);
    break;
  }
}

void bluetoothFuncTail()
{
  buzzerInterface.update();
  logger.flush();
}

// ---- START UP STATE ---- //
void startUp()
{
  bluetoothFuncHeader();
  if (btMsgAvailable)
  {
    BTInterface.readBluetoothMessage(btMsg);
    switch (btMsg)
    {
    case Bluetooth::BluetoothMessage::StartUp:
      logger.logInfo("Starting all modules...");
      startAllPeripherals();
      buzzerInterface.startProgram(Buzzer::BuzzerProgram::SlowContinuousBeep);
      stateMachine.changeState(StateMachine::State::Idle);
      break;
    default:
      logger.logWarning("Bluetooth message code " + String(btMsg) + " not allowed at this time.");
      logger.logInfo("Module has not been started yet, send start command.");
      break;
    }
  }
  bluetoothFuncTail();
}

// ---- OFF LAUNCH ROD TEST STATE ---- //
void offLaunchRodTest()
{
  bluetoothFuncHeader();

  if (btMsgAvailable)
  {
    BTInterface.readBluetoothMessage(btMsg);
    switch (btMsg)
    {

    case Bluetooth::BluetoothMessage::GoToIdle:
      logger.logInfo("Changing state to idle...");
      logger.logMessageCode(timers.totalElapsedTime, OutputMessageCode::StateChangedToIdle);
      stateMachine.changeState(StateMachine::State::Idle);
      break;

    case Bluetooth::BluetoothMessage::StartOffLaunchRodCalibration:
      buzzerInterface.endProgram();
      logger.logInfo("Changing state to off launch rod calibration...");
      logger.logMessageCode(timers.totalElapsedTime, OutputMessageCode::StateChangedToOffLaunchRodCalibration);
      stateMachine.changeState(StateMachine::State::OffLaunchRodCalibration);
      break;

    case Bluetooth::BluetoothMessage::TestSD:
      buzzerInterface.endProgram();
      GroundTests::sdTest(logger);
      buzzerInterface.startProgram(Buzzer::BuzzerProgram::SlowContinuousBeep);
      break;

    case Bluetooth::BluetoothMessage::TestIMU:
      buzzerInterface.endProgram();
      GroundTests::imuTest(sensorInterface, logger);
      buzzerInterface.startProgram(Buzzer::BuzzerProgram::SlowContinuousBeep);
      break;

    default:
      logger.logWarning("Bluetooth message code " + String(btMsg) + " not allowed at this time.");
      break;
    }
  }
  bluetoothFuncTail();
}

// ---- OFF LAUNCH ROD CALIBRATION STATE ----
void offLaunchRodCalibration()
{
  float gxOffset, gyOffset, gzOffset;
  float axOffset, ayOffset, azOffset;

  logger.logInfo("Calibrating gyroscope DO NOT MOVE MODULE!");
  sensorInterface.calibrateGyroscope(gxOffset, gyOffset, gzOffset);
  logger.logInfo("Done calibrating gyroscope.");
  logger.logInfo("Calibrating accelerometer DO NOT MOVE MODULE!");
  sensorInterface.calibrateAccelerometer(axOffset, ayOffset, azOffset);
  logger.logInfo("Done calibrating accelerometer.");
  logger.logInfo("Gyroscope (deg/s) and accelerometer (g) offsets:");
  logger.logInfo("gx: " + String(gxOffset, 5));
  logger.logInfo("gy: " + String(gyOffset, 5));
  logger.logInfo("gz: " + String(gzOffset, 5));
  logger.logInfo("ax: " + String(axOffset, 5));
  logger.logInfo("ay: " + String(ayOffset, 5));
  logger.logInfo("az: " + String(azOffset, 5));
  logger.logInfo("Changing state to idle...");
  logger.logMessageCode(timers.totalElapsedTime, OutputMessageCode::StateChangedToIdle);
  logger.flush();
  buzzerInterface.startProgram(Buzzer::BuzzerProgram::SlowContinuousBeep);
  stateMachine.changeState(StateMachine::State::Idle);
}

// ---- IDLE STATE ---- //
void idle()
{

  bluetoothFuncHeader();

  if (btMsgAvailable)
  {
    BTInterface.readBluetoothMessage(btMsg);
    switch (btMsg)
    {
    case Bluetooth::BluetoothMessage::ArmModule:
      buzzerInterface.endProgram();
      logger.logInfo("Changing state to armed...");
      logger.logMessageCode(timers.totalElapsedTime, OutputMessageCode::StateChangedToArm);
      stateMachine.changeState(StateMachine::State::Arm);
      buzzerInterface.startProgram(Buzzer::BuzzerProgram::ContinuousBeep);
      break;

    case Bluetooth::BluetoothMessage::StartOffLaunchRodTests:
      logger.logInfo("Changing state to off launch rod tests...");
      logger.logMessageCode(timers.totalElapsedTime, OutputMessageCode::StateChangedToOffLaunchRodTest);
      stateMachine.changeState(StateMachine::State::OffLaunchRodTest);
      break;

    case Bluetooth::BluetoothMessage::StartOffLaunchRodCalibration:
      buzzerInterface.endProgram();
      logger.logInfo("Changing state to start off launch rod calibration...");
      logger.logMessageCode(timers.totalElapsedTime, OutputMessageCode::StateChangedToOffLaunchRodCalibration);
      stateMachine.changeState(StateMachine::State::OffLaunchRodCalibration);
      break;

    case Bluetooth::BluetoothMessage::StartLaunchRodCalibration:
      buzzerInterface.endProgram();
      logger.logInfo("Changing state to start launch rod calibration...");
      logger.logMessageCode(timers.totalElapsedTime, OutputMessageCode::StateChangedToLaunchRodCalibration);
      stateMachine.changeState(StateMachine::State::LaunchRodCalibration);
      break;

    case Bluetooth::BluetoothMessage::StartLaunchRodTests:
      logger.logInfo("Changing state to launch rod tests...");
      logger.logMessageCode(timers.totalElapsedTime, OutputMessageCode::StateChangedToLaunchRodTest);
      stateMachine.changeState(StateMachine::State::LaunchRodTest);
      break;

    default:
      logger.logWarning("Bluetooth message code " + String(btMsg) + " not allowed at this time.");
      break;
    }
  }

  bluetoothFuncTail();
}

// ---- LAUNCH ROD CALIBRATION STATE ----
void launchRodCalibration()
{
  float rodRoll, rodPitch, rodYaw;
  float rodAltitude;
  logger.logInfo("Calculating launch rod orientation DO NOT MOVE ROCKET");
  sensorInterface.calculateLaunchRodOrientation(rodRoll, rodPitch, rodYaw);
  logger.logInfo("Done calculating launch rod orientation.");
  logger.logInfo("Calculating launch rod altitude...");
  // sensorInterface.calculateLaunchRodAltitude(rodAltitude);
  // logger.logInfo("Done calculating launch rod altitude.");
  logger.logInfo("Launch rod orientation (rad):");
  logger.logInfo("roll: " + String(rodRoll, 5));
  logger.logInfo("pitch: " + String(rodPitch, 5));
  logger.logInfo("yaw: " + String(rodYaw, 5));
  // logger.logInfo("Launch rod altitude: ");
  // logger.logInfo(String(rodAltitude) + "m");
  logger.logInfo("Changing state to launch rod tests...");
  logger.logMessageCode(timers.totalElapsedTime, OutputMessageCode::StateChangedToLaunchRodTest);
  stateMachine.changeState(StateMachine::State::LaunchRodTest);
  buzzerInterface.startProgram(Buzzer::BuzzerProgram::SlowContinuousBeep);
  logger.flush();
}

// ---- LAUNCH ROD TEST STATE ----
void launchRodTest()
{
  bluetoothFuncHeader();

  if (btMsgAvailable)
  {
    BTInterface.readBluetoothMessage(btMsg);
    switch (btMsg)
    {

    case Bluetooth::BluetoothMessage::GoToIdle:
      logger.logInfo("Changing state to idle...");
      logger.logMessageCode(timers.totalElapsedTime, OutputMessageCode::StateChangedToIdle);
      stateMachine.changeState(StateMachine::State::Idle);
      break;

    case Bluetooth::BluetoothMessage::StartLaunchRodCalibration:
      buzzerInterface.endProgram();
      logger.logInfo("Changing state to start launch rod calibration...");
      logger.logMessageCode(timers.totalElapsedTime, OutputMessageCode::StateChangedToLaunchRodCalibration);
      stateMachine.changeState(StateMachine::State::LaunchRodCalibration);
      break;

    case Bluetooth::BluetoothMessage::TestSD:
      buzzerInterface.endProgram();
      LaunchRodTests::sdTest(logger);
      buzzerInterface.startProgram(Buzzer::BuzzerProgram::SlowContinuousBeep);
      break;

    default:
      logger.logWarning("Bluetooth message code " + String(btMsg) + " not allowed at this time.");
      break;
    }
  }

  bluetoothFuncTail();
}

void arm()
{
  timers.armTime = 0;

  while (timers.armTime < MAX_ARM_WAIT_TIME)
  {
#if !DEBUG_NO_PRIMARY
    if ((primaryComm.readLaunchSignal()))
#else
    if (true)
#endif

    {
      logger.logInfo("Module armed. Waiting for acceleration. Bon voyage!");
      logger.logMessageCode(timers.totalElapsedTime, OutputMessageCode::ArmSuccess);
      if (logger.error != Logger::LoggerError::None)
      {
        haltError(OutputMessageCode::LoggerError);
      }
      buzzerInterface.startProgram(Buzzer::BuzzerProgram::On);
      primaryComm.startNoFailureSignal();
      primaryComm.startWatchdogSignal();
      wdtStarted = true;
      // wdt.start();
      stateMachine.changeState(StateMachine::Armed);
      return;
    }
  }
  logger.logError("Didn't get signal from primary! Aborting.");
  haltError(OutputMessageCode::ArmFailed);
}

void armed()
{
  timers.fixedDt = (unsigned long)timers.dt;
  timers.dt = 0;
  sensorInterface.update(timers.fixedDt);
#if DEBUG_ACTIVE_CONTROL_STATE
  if (true)
#else
  if (sensorInterface.detectLaunch())
#endif // DEBUG_ACTIVE_CONTROL_STATE
  {
    sensorInterface.resetToLaunchRodOrientation();
    timers.dt = 0;
    logger.logMessageCode(timers.launchElapsedTime, OutputMessageCode::StateChangedToActiveControlEnabled);

    buzzerInterface.endProgram();
    // ledInterface.endProgram();
    stateMachine.changeState(StateMachine::State::ActiveControlEnabled);
    logger.logDebug("State actually changed");
  }
  // wdt.feed();
}

void activeControlEnabled()
{
  timers.fixedDt = (unsigned long)timers.dt;
  timers.dt = 0;
  packetUpdated = false;
  // logger.logDebug("Updating sensor interface");
  sensorInterface.update(timers.fixedDt);
  // logger.logDebug("Updated sensor interface");
  if (sensorInterface.detectLaunch())
  {
    packet.message = OutputMessageCode::LaunchDetected;
    packetUpdated = true;
  }
  if (sensorInterface.wasOrientationUpdated())
  {
    sensorInterface.getLastGyroReading(packet.gx, packet.gy, packet.gz);
    sensorInterface.getLastOrientation(packet.w, packet.x, packet.y, packet.z);
    packetUpdated = true;
    // logger.logDebug("Checking angle to vertical");
    packet.angleToVertical = sensorInterface.angleToVertical();
    if (abs(packet.angleToVertical) > MAX_ALLOWABLE_ORIENTATION_ANGLE)
    {
      packet.message = OutputMessageCode::SentFailureToPrimary;
      primaryComm.endNoFailureSignal();
    }
  }
  if (sensorInterface.wasAccelUpdated())
  {
    sensorInterface.getLastAccelReading(packet.ax, packet.ay, packet.az);
    packetUpdated = true;
  }
  if (sensorInterface.wasBarometerUpdated())
  {
    sensorInterface.getLastAltitudeReading(packet.altitude);
    sensorInterface.getLastVelocityReading(packet.verticalVelocity);
    packetUpdated = true;
  }
  serialAvailable = Serial.available();
  if (serialAvailable)
  {
    Serial.readBytes(packet.servoReturnData, serialAvailable);

    // for (int i = 0; i < serialAvailable; i++)
    //   packet.servoReturnData[i] = Serial.read();
  }

  if (packetUpdated)
  {
    // logger.logDebug("Saving to file");

    packet.elapsedTime = timers.fixedDt;
    // to account for write time
    // wdt.feed();
    logger.logData(packet);
    // reset packet
    packet = {};
  }
  // wdt.feed();
}

void setup()
{
  // perform only necessary setup for failsafe mode
  // begin in failsafe mode
  Serial.begin(9600);
  // while (!Serial)
  // {
  // };
  BTInterface.start();
  stateMachine.changeState(StateMachine::State::Startup);
}

void loop()
{
  stateMachine.run();
}