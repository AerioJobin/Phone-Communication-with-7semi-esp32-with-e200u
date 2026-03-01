#ifndef SIM7600MANAGER_H
#define SIM7600MANAGER_H

#include <HardwareSerial.h>

enum class ModemState {
  POWERED_OFF = 0,
  INITIALIZING = 1,
  ATTACHING_NETWORK = 2,
  READY = 3,
  ERROR = 4
};

class Sim7600Manager {
public:
  /**
   * @brief Constructor for Sim7600Manager
   * @param serial Reference to HardwareSerial (typically Serial1)
   * @param pwrPin GPIO pin controlling modem power (PWR_KEY)
   * @param rstPin GPIO pin controlling modem reset (RST)
   */
  Sim7600Manager(HardwareSerial& serial, int pwrPin, int rstPin = -1);

  /**
   * @brief Initialize modem and begin communication
   * @param baud Baud rate (default: 115200)
   * @return true if initialization started successfully
   */
  bool begin(unsigned long baud = 115200);

  /**
   * @brief Non-blocking state machine tick
   * Call this from loop() at regular intervals
   */
  void tick();

  /**
   * @brief Check if modem is ready for operations
   * @return true if state == READY
   */
  bool isReady() const { return _state == ModemState::READY; }

  /**
   * @brief Get current modem state
   */
  ModemState state() const { return _state; }

  /**
   * @brief Send raw AT command and wait for response
   * @param cmd Command string (e.g., "AT+CREG?")
   * @param expect Expected response substring
   * @param timeoutMs Timeout in milliseconds
   * @return true if response received
   */
  bool sendAT(const char* cmd, const char* expect, uint32_t timeoutMs = 1000);

  /**
   * @brief Get last received response from modem
   */
  String getResponse() const { return _response; }

  /**
   * @brief Force power cycle (reconnect)
   */
  void powerCycle();

  /**
   * @brief Reset state machine (soft reset)
   */
  void reset();

private:
  HardwareSerial& _serial;
  int _pwrPin, _rstPin;
  ModemState _state;
  unsigned long _lastActionMs;
  unsigned long _stateEnteredMs;
  String _response;
  uint8_t _initRetries;
  
  static const uint32_t INIT_TIMEOUT = 3000;
  static const uint32_t ATTACH_TIMEOUT = 10000;
  static const uint32_t MAX_RETRIES = 3;

  void _readSerial();
  void _processState();
  bool _checkNetworkAttach();
};

#endif // SIM7600MANAGER_H
