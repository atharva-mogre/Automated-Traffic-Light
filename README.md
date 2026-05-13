# 🚦 Automated Traffic Light Signal Detector

An intelligent traffic light control system that uses an **HC-SR04 ultrasonic sensor** and **Arduino Uno** to detect traffic based on real-time distance measurements. The system features a modern web interface for monitoring traffic light status and sensor readings.

## 🎯 Features

- **Real-time Traffic Detection**: Uses HC-SR04 sensor to measure distance and control traffic lights
- **Smart Traffic Logic**: Automatically adjusts lights based on traffic density
- **Live Web Interface**: Monitor traffic lights and sensor data in real-time
- **WebSocket Communication**: Instant updates with Socket.IO
- **Modern UI Design**: Beautiful, responsive interface with animated LEDs
- **Serial Communication**: Seamless Arduino-to-server data transfer

## 📊 Traffic Light Logic

| Distance Range | Traffic Light | Traffic Condition |
|----------------|---------------|-------------------|
| < 50 cm        | 🔴 RED        | Heavy Traffic     |
| 50-100 cm      | 🟡 YELLOW     | Moderate Traffic  |
| > 100 cm       | 🟢 GREEN      | Clear Road        |

## 🔧 Hardware Requirements

- **Arduino Uno** (or compatible board)
- **HC-SR04 Ultrasonic Sensor**
- **3x LEDs** (Red, Yellow, Green)
- **3x 220Ω Resistors** (for LEDs)
- **Breadboard & Jumper Wires**
- **USB Cable** (for Arduino)

## 🔌 Hardware Wiring

### HC-SR04 Sensor Connections
```
HC-SR04 Pin    →    Arduino Pin
─────────────────────────────────
VCC            →    5V
GND            →    GND
Trig           →    Pin 9
Echo           →    Pin 10
```

### Traffic Light LED Connections
```
LED Color      →    Arduino Pin    (with 220Ω resistor)
─────────────────────────────────────────────────────────
Red LED        →    Pin 5
Yellow LED     →    Pin 6
Green LED      →    Pin 7
```

### Wiring Diagram
```
                        Arduino Uno
                    ┌───────────────┐
    HC-SR04         │               │
    ┌─────┐         │               │
    │ VCC ├─────────┤ 5V            │
    │ Trig├─────────┤ 9             │
    │ Echo├─────────┤ 10            │
    │ GND ├─────────┤ GND           │
    └─────┘         │               │
                    │               │
                    │ 5  ├─[220Ω]─→ Red LED → GND
                    │ 6  ├─[220Ω]─→ Yellow LED → GND
                    │ 7  ├─[220Ω]─→ Green LED → GND
                    │               │
                    └───────────────┘
```

## 💻 Software Requirements

- **Arduino IDE** (for uploading firmware)
- **Node.js** (v14 or higher)
- **npm** (comes with Node.js)

## 🚀 Installation & Setup

### Step 1: Upload Arduino Code

1. Open **Arduino IDE**
2. Open the file `traffic_light_detector.ino`
3. Connect your Arduino Uno via USB
4. Select the correct board: `Tools → Board → Arduino Uno`
5. Select the correct port: `Tools → Port → COM3` (or your Arduino's port)
6. Click **Upload** (→) button
7. Wait for "Done uploading" message

### Step 2: Setup Backend Server

1. **Install Node.js dependencies**:
   ```bash
   npm install
   ```

2. **Configure Serial Port**:
   - Open `server.js`
   - Find line `const PORT_NAME = 'COM3';`
   - Update `COM3` to match your Arduino's port
   
   **To find your Arduino port**:
   - **Windows**: Check Device Manager → Ports (COM & LPT)
   - **Mac/Linux**: Run `ls /dev/tty.*` in terminal
   - Or visit: `http://localhost:3000/api/ports` after starting the server

3. **Start the server**:
   ```bash
   npm start
   ```

   You should see:
   ```
   ============================================================
   🚦 Traffic Light Detector Server Started
   ============================================================
   🌐 Web Interface: http://localhost:3000
   📡 API Endpoint: http://localhost:3000/api/status
   🔧 Port List: http://localhost:3000/api/ports
   ============================================================
   ✅ Serial Port opened on COM3 at 9600 baud
   Connected to Arduino - Receiving data...
   ```

### Step 3: Access Web Interface

1. Open your web browser
2. Navigate to: **http://localhost:3000**
3. You should see the traffic light interface
4. Test by placing your hand near the sensor

## 📁 Project Structure

```
MDP/
├── traffic_light_detector.ino   # Arduino firmware
├── server.js                     # Node.js backend server
├── package.json                  # Node.js dependencies
├── public/                       # Frontend files
│   ├── index.html               # HTML structure
│   ├── style.css                # Styling & animations
│   └── app.js                   # Frontend logic
└── README.md                    # This file
```

## 🌐 API Endpoints

### Get Current Status
```http
GET /api/status
```
Response:
```json
{
  "success": true,
  "data": {
    "distance": 75,
    "state": "YELLOW",
    "timestamp": "2026-02-04T17:35:22.123Z"
  }
}
```

### Check System Health
```http
GET /api/health
```
Response:
```json
{
  "success": true,
  "status": "running",
  "serialPort": "connected",
  "timestamp": "2026-02-04T17:35:22.123Z"
}
```

### List Available Serial Ports
```http
GET /api/ports
```
Response:
```json
{
  "success": true,
  "ports": [
    {
      "path": "COM3",
      "manufacturer": "Arduino LLC",
      "serialNumber": "ABC123"
    }
  ]
}
```

## 🐛 Troubleshooting

### Arduino Not Connecting

**Problem**: `Serial Port Error: Error: Access denied`

**Solutions**:
1. Close Arduino IDE Serial Monitor
2. Check if another program is using the port
3. Unplug and replug the Arduino
4. Update `PORT_NAME` in `server.js`

### Wrong COM Port

**Problem**: Cannot find Arduino port

**Solutions**:
1. Visit `http://localhost:3000/api/ports` to see available ports
2. Check Device Manager (Windows) or `ls /dev/tty.*` (Mac/Linux)
3. Update `PORT_NAME` in `server.js` to the correct port

### Sensor Not Working

**Problem**: Distance always shows 0 or 400

**Solutions**:
1. Verify HC-SR04 wiring (especially Trig and Echo pins)
2. Ensure 5V and GND are properly connected
3. Test sensor with Arduino Serial Monitor first
4. Check for loose connections on breadboard

### LEDs Not Lighting Up

**Problem**: Physical LEDs don't turn on

**Solutions**:
1. Verify LED polarity (long leg = positive)
2. Check resistor connections (220Ω)
3. Ensure correct pin numbers (5, 6, 7)
4. Test LEDs with a simple blink sketch

### Web Interface Not Updating

**Problem**: Browser shows old data

**Solutions**:
1. Check browser console for errors (F12)
2. Verify WebSocket connection (should show "Connected")
3. Restart the Node.js server
4. Hard refresh the page (Ctrl+F5)

## 🎨 Features Showcase

### Real-time Monitoring
- Live distance measurements updated 5 times per second
- Instant traffic light state changes
- Connection status indicator

### Smart Visuals
- Animated LED lights with realistic glow effects
- Color-coded distance bar
- Smooth transitions and animations
- Responsive design for all devices

### Backend Excellence
- Error handling and auto-recovery
- RESTful API endpoints
- WebSocket for zero-latency updates
- Comprehensive logging

## 🔮 Future Enhancements

- [ ] Multiple sensor support
- [ ] Data logging and analytics
- [ ] Configurable thresholds via UI
- [ ] Email/SMS notifications
- [ ] Mobile app integration
- [ ] Historical data charts

## 📝 License

MIT License - Feel free to use and modify!

## 🤝 Contributing

Contributions are welcome! Feel free to:
- Report bugs
- Suggest features
- Submit pull requests

## ⚡ Technologies Used

- **Hardware**: Arduino Uno, HC-SR04 Sensor
- **Backend**: Node.js, Express.js, SerialPort, Socket.IO
- **Frontend**: HTML5, CSS3, JavaScript (Vanilla)
- **Real-time**: WebSocket (Socket.IO)

---

**Made with ❤️ for Smart Traffic Management**

(This file is AI curated)
