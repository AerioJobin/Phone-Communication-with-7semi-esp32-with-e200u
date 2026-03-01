#include "Sim7600Manager.h"

Sim7600Manager::Sim7600Manager(HardwareSerial& serial, int pwrPin, int rstPin)
    : _serial(serial), _pwrPin(pwrPin), _rstPin(rstPin),
      _state(ModemState::POWERED_OFF), _initRetries(0) {}

bool Sim7600Manager::begin(unsigned long baud) {
  _serial.begin(baud);
  pinMode(_pwrPin, OUTPUT);
  if (_rstPin >= 0) {
    pinMode(_rstPin, OUTPUT);
  }
  _state = ModemState::INITIALIZING;
  _lastActionMs = millis();
  _stateEnteredMs = millis();
  return true;
}

void Sim7600Manager::tick() {
  _readSerial();
  _processState();
}

void Sim7600Manager::_readSerial() {
  while (_serial.available()) {
    char c = _serial.read();
    _response += c;
    if (_response.length() > 512) {
      _response = _response.substring(_response.length() - 256);
    }
  }
}

void Sim7600Manager::_processState() {
  unsigned long now = millis();
  unsigned long stateAge = now - _stateEnteredMs;

  switch (_state) {
    case ModemState::POWERED_OFF:
      powerCycle();
      _state = ModemState::INITIALIZING;
      _stateEnteredMs = now;
      _initRetries = 0;
      break;

    case ModemState::INITIALIZING:
      if (stateAge > INIT_TIMEOUT) {
        if (sendAT("AT", "OK", 1000)) {
          if (sendAT("ATE0", "OK", 1000)) {
            if (sendAT("AT+CPIN?", "READY", 2000)) {
              _state = ModemState::ATTACHING_NETWORK;
              _stateEnteredMs = now;
              _initRetries = 0;
            }
          }
        } else {
          _initRetries++;
          if (_initRetries >= MAX_RETRIES) {
            _state = ModemState::ERROR;
            _stateEnteredMs = now;
          } else {
            powerCycle();
            _stateEnteredMs = now;
          }
        }
      }
      break;

    case ModemState::ATTACHING_NETWORK:
      if (stateAge > ATTACH_TIMEOUT) {
        if (_checkNetworkAttach()) {
          _state = ModemState::READY;
          _stateEnteredMs = now;
        } else {
          _initRetries++;
          if (_initRetries >= MAX_RETRIES) {
            _state = ModemState::ERROR;
            _stateEnteredMs = now;
          } else {
            _state = ModemState::INITIALIZING;
            _stateEnteredMs = now;
          }
        }
      }
      break;

    case ModemState::READY:
      // Periodic network check every 30 seconds
      if (stateAge > 30000) {
        if (!_checkNetworkAttach()) {
          _state = ModemState::ATTACHING_NETWORK;
          _stateEnteredMs = now;
        } else {
          _stateEnteredMs = now; // Reset timer
        }
      }
      break;

    case ModemState::ERROR:
      if (stateAge > 10000) {
        _state = ModemState::POWERED_OFF;
        _stateEnteredMs = now;
      }
      break;
  }
}

bool Sim7600Manager::sendAT(const char* cmd, const char* expect, uint32_t timeoutMs) {
  _response = "";
  _serial.println(cmd);
  unsigned long start = millis();
  
  while (millis() - start < timeoutMs) {
    _readSerial();
    if (_response.indexOf(expect) >= 0) {
      return true;
    }
    delay(10);
  }
  return false;
}

bool Sim7600Manager::_checkNetworkAttach() {
  if (!sendAT("AT+CREG?", "+CREG: 0,1", 1000) &&
      !sendAT("AT+CREG?", "+CREG: 0,5", 1000)) {
    return false;
  }
  return sendAT("AT+CGATT?", "CGATT: 1", 1000);
}

void Sim7600Manager::powerCycle() {
  digitalWrite(_pwrPin, HIGH);
  delay(100);
  digitalWrite(_pwrPin, LOW);
  delay(1000);
  digitalWrite(_pwrPin, HIGH);
  delay(1500);
}

void Sim7600Manager::reset() {
  _response = "";
  _state = ModemState::POWERED_OFF;
  _stateEnteredMs = millis();
  _initRetries = 0;
}
