# Phone Communication with 7Semi ESP32 EC200U

![ESP32](https://img.shields.io/badge/ESP32-S3-blue)
![EC200U](https://img.shields.io/badge/EC200U-4G%20Module-green)
![Language](https://img.shields.io/badge/language-C%2B%2B-orange)
![Status](https://img.shields.io/badge/status-active-success)

## 📋 Overview

This project enables voice call functionality using the ESP32-S3 microcontroller with the EC200U 4G/LTE module from 7Semi. The implementation allows IoT devices to make and receive phone calls over cellular networks, with diagnostic features for troubleshooting network connectivity issues.

## 🎯 Features

- ☎️ **Voice Call Capability** - Make and receive phone calls
- 📊 **Signal Diagnostics** - Check signal strength (CSQ)
- 🌐 **Network Registration** - Monitor network status (CREG)
- 🔊 **Volume Control** - Adjust audio channel volume
- 🔋 **Power Management** - Automatic modem recovery and heartbeat monitoring
- 🚫 **VoLTE Support** - Compatible with VoLTE-enabled carriers

## 🔧 Hardware Requirements

- **ESP32-S3** Development Board
- **EC200U 4G/LTE Module** (7Semi)
- **4G SIM Card** with voice plan enabled
- **External Power Supply** (5V-26V recommended for stable voice calls)
- **High-quality USB cable** or dedicated power adapter
- **Antenna** for cellular connectivity

## 📌 Pin Configuration

According to the hardware manual, the processors are internally linked via specific pins:

| Pin | Function |
|-----|----------|
| **GPIO 13** | ESP32 TX (Transmit) |
| **GPIO 12** | ESP32 RX (Receive) |
| **GPIO 10** | Power Key (ON/OFF) |

## 🚀 Getting Started

### 1. Arduino IDE Setup

```bash
# Install ESP32 board support
# File > Preferences > Additional Board Manager URLs:
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

### 2. Board Configuration

- **Board:** ESP32S3 Dev Module
- **Upload Speed:** 115200
- **USB CDC On Boot:** Enabled
- **Port:** Select your COM port

### 3. SIM Card Setup

Ensure your SIM card:
- ✅ Has an active voice/calling plan
- ✅ PIN lock is disabled
- ✅ Supports 4G/LTE networks
- ✅ VoLTE is enabled (if required by carrier)

### 4. Upload & Test

1. Open `PhoneCommunication.ino`
2. Update the phone number in the code
3. Upload to ESP32-S3
4. Open Serial Monitor (115200 baud)
5. Use keyboard commands to control

## 🎮 Usage & Serial Commands

### Interactive Commands

| Key | Function | Description |
|-----|----------|-------------|
| **S** | Diagnostics | Check signal strength and network registration |
| **V** | Volume | Initialize audio channel at maximum volume |
| **C** | Call | Initiate a phone call |
| **H** | Hangup | End the current call |

### Diagnostic Information

#### Signal Strength (CSQ)
```
+CSQ: XX,99
```
- **XX < 12** → Weak signal ⚠️
- **XX 12-20** → Fair signal 🟡
- **XX > 20** → Good signal ✅

#### Network Registration (CREG)
```
+CREG: 0,1 or 0,5
```
- **0,0 or 0,2** → Not registered ❌
- **0,1** → Registered (Home network) ✅
- **0,5** → Registered (Roaming) ✅

## 🐛 Troubleshooting

### Common Issues & Solutions

#### ⚡ Module Powers Down During Call

**Symptom:** LEDs go dark, modem stops responding

**Cause:** Insufficient power supply. Voice calls require significant power bursts.

**Solutions:**
1. **External Power Supply:**
   - Connect 5V-26V to external supply voltage pins
   - Board has onboard regulator for module power management

2. **Direct V4G Pin Supply:**
   - Provide regulated 3.8V (maximum) directly to V4G pin
   - Ensure stable current delivery

3. **USB Power:**
   - Use USB 3.0/3.1 high-power port
   - Use high-quality, short USB cable
   - Avoid laptop USB ports (insufficient current)
   - Use dedicated wall adapter instead

#### 📶 Weak Signal (CSQ < 12)

- Check antenna connection
- Move to location with better coverage
- Ensure antenna is properly oriented
- Verify SIM card is active

#### 🌐 Network Registration Fails

- Wait up to 60 seconds for registration
- Check if SIM card supports the network band
- Verify SIM card is not blocked
- Restart the modem

#### 🚫 NO CARRIER Error

**Even with good signal (CSQ > 20)?**

1. **VoLTE Not Enabled:**
   - Your carrier may require VoLTE
   - Enable via carrier-specific AT command
   - Contact carrier for VoLTE activation

2. **Voice Plan Not Active:**
   - Verify voice calling is enabled on SIM
   - Check with carrier

3. **Power Supply Issue:**
   - Classic symptom of voltage drop
   - Follow power supply solutions above

#### 🔇 Cannot Hear Audio

- Press 'V' to initialize audio channel
- Check speaker/headphone connections
- Verify volume settings
- Ensure provider isn't rejecting the call

## ⚙️ Advanced Features

### Heartbeat Monitoring

The code includes automatic recovery:
- Monitors modem communication status
- Detects when modem becomes unresponsive
- Automatically attempts to repower the modem
- Restores connectivity without manual intervention

### AT Commands Used

```
AT                    # Test module communication
AT+CPIN?             # Check SIM status
AT+CSQ               # Query signal quality
AT+CREG?             # Check network registration
ATD<number>;         # Dial phone number
AT+CLVL=<level>      # Set audio volume
ATH                  # Hang up call
AT+QPOWD             # Power down modem
```

## 📚 Dependencies

No external libraries required - uses built-in ESP32 Serial communication.

## 🔗 Useful Resources

- [EC200U AT Commands Manual](https://www.quectel.com/product/ec200u)
- [ESP32-S3 Datasheet](https://www.espressif.com/en/products/socs/esp32-s3)
- [7Semi Documentation](https://www.7semi.com)
- [VoLTE Configuration Guide](https://www.quectel.com)

## 💡 Tips for Best Performance

1. **Always use external power supply for voice calls**
2. Check diagnostics ('S' key) before making calls
3. Initialize volume ('V' key) before each call session
4. Monitor serial output for error messages
5. Ensure good signal strength (CSQ > 15)
6. Wait for network registration before dialing

## 🤝 Contributing

Feel free to:
- Report bugs
- Suggest new features
- Submit pull requests
- Share your use cases
- Improve documentation

## 📄 License

This project is open-source and available for educational and commercial use.

## 👨‍💻 Author

**Aerio Jobin Momin**
- GitHub: [@AerioJobin](https://github.com/AerioJobin)

## 📧 Contact

For questions or support, please open an issue on GitHub.

---

**Note:** Voice calls consume significantly more power than data operations. Ensure adequate power supply to prevent modem shutdown during calls.
