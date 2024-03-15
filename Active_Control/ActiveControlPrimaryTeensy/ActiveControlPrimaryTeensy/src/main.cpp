#include <Arduino.h>
#include "StateMachine.hpp"
#include "SensorInterface.hpp"
#include "ServoInterface.hpp"
#include "ReadVoltage.hpp"
#include "ValueDefinitions.hpp"
#include "MonitorComm.hpp"
#include "LQRController.hpp"
#include "BluetoothInterface.hpp"
#include "BuzzerInterface.hpp"
#include "LEDInterface.hpp"
#include "Logger.hpp"
#include "DataPacket.hpp"
#include "OutputMessageCodes.hpp"
#include "Watchdog_t4.h"
#include "GroundTests.hpp"
#include "LaunchRodTests.hpp"
#include "Profiler.hpp"

void startUp();
void offLaunchRodTest();
void offLaunchRodCalibration();
void idle();
void launchRodCalibration();
void launchRodTest();
void arm();
void armed();
void belowVmc();
void activeControlEnabled();
void descent();
void shutdown();
void failsafe();

FunctionArray stateFunctions[NUM_STATES] = {startUp, offLaunchRodTest, offLaunchRodCalibration, idle, launchRodCalibration, launchRodTest, arm, armed, belowVmc, activeControlEnabled, descent, shutdown, failsafe};
StateMachine::StateMachine stateMachine{stateFunctions};
Bluetooth::BluetoothInterface BTInterface;
#if LOG_TO_BT
Logger::Logger logger{BTInterface};
#else
Logger::Logger logger;
#endif
Servo::ServoInterface servoInterface;
SensorInterface::SensorInterface sensorInterface{logger};
VoltageReader voltageReader;
LQRController controller;
MonitorComm::MonitorComm monitorComm;

Buzzer::BuzzerInterface buzzerInterface;
LED::LEDInterface ledInterface;

DataPacket packet;
bool packetUpdated;

WDT_T4<WDT3> wdt;
bool wdtStarted = false;
WDT_timings_t wdtConfig;

float targetDeflections[NUM_SERVOS];
float actualDeflections[NUM_SERVOS];
float stateSpace[STATE_SPACE_SIZE];
GainVariables gainVars{0.0};

Bluetooth::BluetoothMessage btMsg;
bool btMsgAvailable = false;
;

struct Timers
{
  elapsedMillis totalElapsedTime;
  elapsedMillis armTime;
  elapsedMicros launchElapsedTime;
  elapsedMicros dt;
  elapsedMillis gainTimer;
  elapsedMillis monitorCheckTimer;
  elapsedMillis readDeflectionTimer;
  elapsedMillis readVoltageTimer;
  unsigned long fixedDt;
};

Timers timers;

// Fast start up
extern "C" void startup_middle_hook(void);
extern "C" volatile uint32_t systick_millis_count;

void startup_middle_hook(void)
{
  // force millis() to be 300 to skip startup delays
  // systick_millis_count = 300;
}

void haltError(OutputMessageCode errorMessageCode)
{
  servoInterface.setZeroDeflections();
  logger.logMessageCode(timers.totalElapsedTime, errorMessageCode);
  logger.endLog();
  buzzerInterface.startProgram(Buzzer::BuzzerProgram::ContinuousBeep);
  ledInterface.startProgram(LED::LEDProgram::ContinuousBlink);
  while (true)
  {
    // stop a reset as zero deflections already set
    if (wdtStarted)
    {
      wdt.feed();
    }
    if (!servoInterface.checkZeroDeflection())
    {
      servoInterface.setZeroDeflections();
    }
    buzzerInterface.update();
    ledInterface.update();
  };
}

void wdtCallback()
{
  haltError(OutputMessageCode::PrimaryWatchdogNotFed);
}

// ---- FAIL SAFE STATE ---- /
void failsafe()
{
  servoInterface.getDeflections(actualDeflections);
  logger.logDebug(actualDeflections[0]);
  if (!servoInterface.checkZeroDeflection())
  {
    servoInterface.setZeroDeflections();
    return;
  }

  // if zero deflections, go to normal startup
  logger.logInfo("Changing state to startup...");
  BTInterface.start();
  // BTInterface.sendBluetoothMessage("hello");
  stateMachine.changeState(StateMachine::State::Startup);
  // logger.logInfo("Changed state.");
}

void bluetoothFuncHeader()
{

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

void startAllPeripherals()
{
  logger.logDebug("Start up state");
  logger.start();
  switch (logger.error)
  {
  case Logger::LoggerError::None:
    break;
  case Logger::LoggerError::FileOpenError:
    logger.logError("Could not open file.");
    haltError(OutputMessageCode::LoggerError);
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
  logger.logInfo("Starting buzzer and led...");
  buzzerInterface.start(Buzzer::BuzzerProgram::Off, NOTE_A4);
  ledInterface.start(LED::LEDProgram::On);

  // play start up song
  buzzerInterface.startProgramDelay(Buzzer::BuzzerProgram::Startup);

  // start bluetooth
  logger.logMessage("Starting Bluetooth interface...", Logger::LogLevel::Info);
  BTInterface.start();
  if (BTInterface.error == Bluetooth::BluetoothError::StartError)
  {
    logger.logMessage("Error starting Bluetooth interface.", Logger::LogLevel::Error);
    haltError(OutputMessageCode::BTStartUpError);
  }
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

  // start other bits (these shouldn't have errors)
  logger.logInfo("Starting controller...");
  controller.start();
  logger.logInfo("Started controller.");

  logger.logInfo("Starting voltage reader...");
  voltageReader.start();
  logger.logInfo("Started voltage reader.");

  logger.logInfo("Starting monitor comm...");
  monitorComm.start();
  logger.logInfo("Started monitor comm.");

  logger.logInfo("Start up complete.");
  logger.logInfo("Changing state to idle...");
  logger.logMessageCode(timers.totalElapsedTime, OutputMessageCode::StateChangedToIdle);
  logger.flush();
  stateMachine.changeState(StateMachine::State::Idle);
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

void plusOffset(uint8_t servoNum, String servoName)
{
  int16_t offset = servoInterface.addZeroOffset(servoNum);
  logger.logInfo("New offset for " + servoName + " = " + String(offset));
  servoInterface.setZeroDeflections();
}

void subOffset(uint8_t servoNum, String servoName)
{
  int16_t offset = servoInterface.subZeroOffset(servoNum);
  logger.logInfo("New offset for " + servoName + " = " + String(offset));
  servoInterface.setZeroDeflections();
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
      // buzzerInterface.endProgram();
      logger.logInfo("Changing state to idle...");
      logger.logMessageCode(timers.totalElapsedTime, OutputMessageCode::StateChangedToIdle);
      stateMachine.changeState(StateMachine::State::Idle);
      buzzerInterface.startProgram(Buzzer::BuzzerProgram::SlowContinuousBeep);
      break;

    case Bluetooth::BluetoothMessage::StartOffLaunchRodCalibration:
      buzzerInterface.endProgram();
      logger.logInfo("Changing state to off launch rod calibration...");
      logger.logMessageCode(timers.totalElapsedTime, OutputMessageCode::StateChangedToOffLaunchRodCalibration);
      stateMachine.changeState(StateMachine::State::OffLaunchRodCalibration);
      break;

    case Bluetooth::BluetoothMessage::TestWDT:
      buzzerInterface.endProgram();
      GroundTests::wdtTest(servoInterface, logger);
      buzzerInterface.startProgram(Buzzer::BuzzerProgram::SlowContinuousBeep);
      break;

    case Bluetooth::BluetoothMessage::TestMaxDeflection:
      buzzerInterface.endProgram();
      GroundTests::maxDeflectionTest(servoInterface, logger);
      buzzerInterface.startProgram(Buzzer::BuzzerProgram::SlowContinuousBeep);
      break;

    case Bluetooth::BluetoothMessage::TestMaxNegativeDeflection:
      buzzerInterface.endProgram();
      GroundTests::maxNegativeDeflectionTest(servoInterface, logger);
      buzzerInterface.startProgram(Buzzer::BuzzerProgram::SlowContinuousBeep);
      break;

    case Bluetooth::BluetoothMessage::TestZeroDeflection:
      buzzerInterface.endProgram();
      GroundTests::zeroDeflectionTest(servoInterface, logger);
      buzzerInterface.startProgram(Buzzer::BuzzerProgram::SlowContinuousBeep);
      break;

    case Bluetooth::BluetoothMessage::TestRapidDeflection:
      buzzerInterface.endProgram();
      GroundTests::rapidDeflectionTest(servoInterface, logger);
      buzzerInterface.startProgram(Buzzer::BuzzerProgram::SlowContinuousBeep);
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

    case Bluetooth::BluetoothMessage::TestBMP:
      buzzerInterface.endProgram();
      GroundTests::bmpTest(sensorInterface, logger);
      buzzerInterface.startProgram(Buzzer::BuzzerProgram::SlowContinuousBeep);
      break;

    case Bluetooth::BluetoothMessage::X1Plus:
      plusOffset(0, "X1");

      break;

    case Bluetooth::BluetoothMessage::X1Sub:
      subOffset(0, "X1");
      break;

    case Bluetooth::BluetoothMessage::X2Plus:
      plusOffset(1, "X2");
      break;

    case Bluetooth::BluetoothMessage::X2Sub:
      subOffset(1, "X2");
      break;

    case Bluetooth::BluetoothMessage::Y1Plus:
      plusOffset(2, "Y1");
      break;

    case Bluetooth::BluetoothMessage::Y1Sub:
      subOffset(2, "Y1");
      break;

    case Bluetooth::BluetoothMessage::Y2Plus:
      plusOffset(3, "Y2");
      break;

    case Bluetooth::BluetoothMessage::Y2Sub:
      subOffset(3, "Y2");
      break;

    case Bluetooth::BluetoothMessage::GetBatteryVoltage:
      voltageReader.readAndLog(logger);
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
  // sensorInterface.calibrateGyroscope();
  logger.logInfo("Done calibrating gyroscope.");
  logger.logInfo("Calibrating accelerometer DO NOT MOVE MODULE!");
  sensorInterface.calibrateAccelerometer(axOffset, ayOffset, azOffset);
  logger.logInfo("Done calibrating accelerometer.");
  logger.logInfo("Gyroscope (rad/s) and accelerometer (ms-2) offsets:");
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
  // buzzerInterface.startProgram(Buzzer::BuzzerProgram::SlowContinuousBeep);

  bluetoothFuncHeader();

  if (btMsgAvailable)
  {
    BTInterface.readBluetoothMessage(btMsg);
    switch (btMsg)
    {
    case Bluetooth::BluetoothMessage::ArmModule:

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

    case Bluetooth::BluetoothMessage::GetBatteryVoltage:
      voltageReader.readAndLog(logger);
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
  sensorInterface.calculateLaunchRodAltitude(rodAltitude);
  logger.logInfo("Done calculating launch rod altitude.");
  logger.logInfo("Launch rod orientation (rad):");
  logger.logInfo("roll: " + String(rodRoll));
  logger.logInfo("pitch: " + String(rodPitch));
  logger.logInfo("yaw: " + String(rodYaw));
  logger.logInfo("Launch rod altitude: ");
  logger.logInfo(String(rodAltitude) + "m");
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

    case Bluetooth::BluetoothMessage::TestMaxDeflection:
      buzzerInterface.endProgram();
      LaunchRodTests::maxDeflectionTest(servoInterface, logger);
      buzzerInterface.startProgram(Buzzer::BuzzerProgram::SlowContinuousBeep);
      break;

    case Bluetooth::BluetoothMessage::TestMaxNegativeDeflection:
      buzzerInterface.endProgram();
      LaunchRodTests::maxNegativeDeflectionTest(servoInterface, logger);
      buzzerInterface.startProgram(Buzzer::BuzzerProgram::SlowContinuousBeep);
      break;

    case Bluetooth::BluetoothMessage::TestZeroDeflection:
      buzzerInterface.endProgram();
      LaunchRodTests::zeroDeflectionTest(servoInterface, logger);
      buzzerInterface.startProgram(Buzzer::BuzzerProgram::SlowContinuousBeep);
      break;

    case Bluetooth::BluetoothMessage::TestRapidDeflection:
      buzzerInterface.endProgram();
      LaunchRodTests::rapidDeflectionTest(servoInterface, logger);
      buzzerInterface.startProgram(Buzzer::BuzzerProgram::SlowContinuousBeep);
      break;

    case Bluetooth::BluetoothMessage::TestSD:
      buzzerInterface.endProgram();
      LaunchRodTests::sdTest(logger);
      buzzerInterface.startProgram(Buzzer::BuzzerProgram::SlowContinuousBeep);
      break;

    case Bluetooth::BluetoothMessage::GetBatteryVoltage:
      voltageReader.readAndLog(logger);
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
  monitorComm.startLaunchSignal();
  while (timers.armTime < MAX_ARM_WAIT_TIME)
  {
#if !DEBUG_NO_MONITOR
    if ((!monitorComm.readFailureSignal()) & (!monitorComm.readWatchdogSignal()))
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
      wdtStarted = true;
      wdtConfig.window = WDT_WINDOW;
      wdtConfig.timeout = WDT_TIMEOUT;
      wdtConfig.trigger = WDT_TRIGGER;
      wdtConfig.callback = wdtCallback;
      wdt.begin(wdtConfig);
      stateMachine.changeState(StateMachine::Armed);
      return;
    }
  }
  logger.logError("Didn't get signal from Monitor! Aborting.");
  haltError(OutputMessageCode::ArmFailed);
}

void armed()
{
  timers.fixedDt = (unsigned long)timers.dt;
  timers.dt = 0;
  sensorInterface.update(timers.fixedDt);
#if !DEBUG_NO_MONITOR
  if (monitorComm.readWatchdogSignal())
  {
    haltError(OutputMessageCode::RecievedWatchdogFromMonitor);
  }
  if (monitorComm.readFailureSignal())
  {
    haltError(OutputMessageCode::RecievedFailureFromMonitor);
  }
#endif
#if DEBUG_ACTIVE_CONTROL_STATE | ZERO_DEFLECTIONS_FLIGHT
  if (true)
#else
  if (sensorInterface.detectLaunch())
#endif // DEBUG_ACTIVE_CONTROL_STATE
  {
    sensorInterface.resetToLaunchRodOrientation();
    logger.logMessageCode(timers.launchElapsedTime, OutputMessageCode::StateChangedToActiveControlEnabled);

    buzzerInterface.endProgram();
    // ledInterface.endProgram();
    timers.gainTimer = 0;
    timers.launchElapsedTime = 0;
    timers.dt = 0;
    timers.monitorCheckTimer = 0;
    timers.readDeflectionTimer = 0;
    timers.readVoltageTimer = 0;
    servoInterface.setMode(Servo::ServoMode::Enabled);
    stateMachine.changeState(StateMachine::State::ActiveControlEnabled);
    return;
  }
  wdt.feed();
}

void belowVmc()
{
}

void activeControlEnabled()
{
  timers.fixedDt = (unsigned long)timers.dt;
  timers.dt = 0;
  // profileIterationStart();
  // logger.logDebug("Timer reset.");
  packetUpdated = false;
  // logger.logDebug("Updating sensor interface");
  // profileOrientationStart();
  sensorInterface.update(timers.fixedDt);
  // profileOrientationEnd();
  // logger.logDebug("Sensor interface updated");
  if (sensorInterface.detectLaunch())
  {
    packet.message = OutputMessageCode::LaunchDetected;
    packetUpdated = true;
  }
  if (sensorInterface.wasOrientationUpdated())
  {
    // logger.logDebug("Setting state space");
    sensorInterface.setStateSpace(stateSpace);
    // logger.logDebug("Doing LQR");
    controller.performLQR(stateSpace, targetDeflections);
    // logger.logDebug("Setting deflections");
    servoInterface.setDeflections(targetDeflections);
    // logger.logDebug("Deflections set");
    // logger.logDebug("Updating packet");
    if (abs(sensorInterface.angleToVertical()) > MAX_ALLOWABLE_ORIENTATION_ANGLE)
    {
      servoInterface.setMode(Servo::ServoMode::ZeroDeflection);
      packet.message = OutputMessageCode::MaxOrientationReached;
    }
    packet.x1Target = targetDeflections[0];
    packet.x2Target = targetDeflections[1];
    packet.y1Target = targetDeflections[2];
    packet.y2Target = targetDeflections[3];
    packet.roll = stateSpace[0];
    packet.pitch = stateSpace[1];
    packet.yaw = stateSpace[2];
    packet.rollRate = stateSpace[3];
    packet.pitchRate = stateSpace[4];
    packet.yawRate = stateSpace[5];

    sensorInterface.getLastGyroReading(packet.gx, packet.gy, packet.gz);
    // Serial.println(String(packet.gx) + "," + String(packet.gy) + "," + String(packet.gz));
    sensorInterface.getLastOrientation(packet.w, packet.x, packet.y, packet.z);
    sensorInterface.getLastOrientationRate(packet.wRate, packet.xRate, packet.yRate, packet.zRate);
    packetUpdated = true;
  }
  if (sensorInterface.wasAccelUpdated())
  {
    // logger.logDebug("Updating packet accel");
    sensorInterface.getLastAccelReading(packet.ax, packet.ay, packet.az);
    packetUpdated = true;
  }
  if (sensorInterface.wasBarometerUpdated())
  {
    // logger.logDebug("Updating packet baro");
    sensorInterface.getLastAltitudeReading(packet.altitude);
    sensorInterface.getLastVelocityReading(packet.verticalVelocity);
#if !DEBUG_ACTIVE_CONTROL_STATE
    if (sensorInterface.detectApogee())
    {
      servoInterface.setZeroDeflections();
      packet.message = OutputMessageCode::StateChangedToDescent;
      logger.logData(packet);
      packet = {};
#if !ZERO_DEFLECTIONS_FLIGHT
      stateMachine.changeState(StateMachine::Descent);
      return;
#endif // ZERO_DEFLECTIONS_FLIGHT
    }
    packetUpdated = true;
#endif // DEBUG ACTIVE CONTROL STATE
  }
  if (timers.readDeflectionTimer > READ_DEFLECTIONS_INTERVAL)
  {
    // logger.logDebug("Reading deflections");
    servoInterface.getDeflections(actualDeflections);
    // logger.logDebug("Got deflections");
    packet.x1Actual = actualDeflections[0];
    packet.x2Actual = actualDeflections[1];
    packet.y1Actual = actualDeflections[2];
    packet.y2Actual = actualDeflections[3];
    packetUpdated = true;
    timers.readDeflectionTimer = 0;
  }
  if (timers.gainTimer > GAIN_UPDATE_INTERVAL)
  {
    // logger.logDebug("Updating gain");
    sensorInterface.getLastVelocityReading(gainVars.velocity);
#if DEBUG_ACTIVE_CONTROL_STATE
    gainVars.velocity = DEBUG_ACTIVE_CONTROL_VELOCITY;
#endif
    // profileGainStart();
    controller.updateGain(stateSpace, gainVars);
    // profileGainEnd();
    packet.message = OutputMessageCode::GainUpdated;
    packetUpdated = true;
    timers.gainTimer = 0;
  }
  if (timers.readVoltageTimer > READ_VOLTAGE_INTERVAL)
  {
    // logger.logDebug("reading voltage");
    voltageReader.readVoltage(packet.voltage);
    packetUpdated = true;
    timers.readVoltageTimer = 0;
  }

#if !DEBUG_NO_MONITOR
  if (timers.monitorCheckTimer > MONITOR_CHECK_INTERVAL)
  {
    if (monitorComm.readWatchdogSignal())
    {
      servoInterface.setMode(Servo::ServoMode::ZeroDeflection);
      packet.message = OutputMessageCode::RecievedWatchdogFromMonitor;
      packetUpdated = true;
    }
    if (monitorComm.readFailureSignal())
    {
      servoInterface.setMode(Servo::ServoMode::ZeroDeflection);
      packet.message = OutputMessageCode::RecievedFailureFromMonitor;
      packetUpdated = true;
    }
  }
#endif // DEBUG_NO_MONITOR

  if (packetUpdated)
  {
    // logger.logDebug("Logging to file")

    packet.elapsedTime = timers.fixedDt;
    // to account for write time
    wdt.feed();
    logger.logData(packet);
    logger.flush();
    // reset packet
    packet = {};
    // logger.logDebug("Loggedto file");
  }
  wdt.feed();
  // profileIterationEnd();
}

void descent()
{
  servoInterface.setMode(Servo::ServoMode::Disabled);
  timers.fixedDt = (unsigned long)timers.dt;
  timers.dt = 0;
  packetUpdated = false;
  sensorInterface.update(timers.fixedDt);
  if (sensorInterface.wasOrientationUpdated())
  {
    sensorInterface.setStateSpace(stateSpace);
    packet.roll = stateSpace[0];
    packet.pitch = stateSpace[1];
    packet.yaw = stateSpace[2];
    packet.rollRate = stateSpace[3];
    packet.pitchRate = stateSpace[4];
    packet.yawRate = stateSpace[5];
    sensorInterface.getLastOrientation(packet.w, packet.x, packet.y, packet.z);
    sensorInterface.getLastOrientationRate(packet.wRate, packet.xRate, packet.yRate, packet.zRate);
    packetUpdated = true;
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
    if (sensorInterface.detectLanding())
    {
      packet.message = OutputMessageCode::StateChangedToShutdown;
      logger.logData(packet);
      packet = {};
      stateMachine.changeState(StateMachine::Shutdown);
      return;
    }
    packetUpdated = true;
  }
  if (timers.readVoltageTimer > READ_VOLTAGE_INTERVAL)
  {
    voltageReader.readVoltage(packet.voltage);
    packetUpdated = true;
    timers.readVoltageTimer = 0;
  }
  wdt.feed();
}

void shutdown()
{
  logger.endLog();
  buzzerInterface.startProgram(Buzzer::BuzzerProgram::EndSong);
  buzzerInterface.update();
  wdt.feed();
}

void setup()
{
  // perform only necessary setup for failsafe mode
  // begin in failsafe mode
  Serial.begin(9600);
  // delay(2000);
  logger.logDebug("Started");
  // Serial.println("Starting");
  // servoInterface.setMode(Servo::ServoMode::ZeroDeflection);
  //
  // float testDeflections[4] = {2.0, 2.0, 2.0, 2.0};
  // servoInterface.setDeflections(testDeflections);
  // Serial.println("Deflections set");
  // delay(5000);
  servoInterface.quickStartAndDeflect();
  stateMachine.changeState(StateMachine::State::Failsafe);
}

void loop()
{
  stateMachine.run();
}